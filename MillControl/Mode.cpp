#include <avr/eeprom.h>
#include <util/delay.h>
#include "Mode.h"

int Mode::secondsForGrams(int grams) const {
    return min(MAX_DATA, (((long) grams * (long) calibration_data) / 100l));
}

int Mode::gramsForSeconds(int seconds) const {
    //deciseconds to decigrams
    //Reconsider rounding problems!
    return min(MAX_DATA, ((long) seconds * 100l) / (long) calibration_data);
}

//converts to another mode
void Mode::setMode(char newMode) {
    if (mode != newMode) {
        int (Mode::*converter)(int) const = NULL;

        if (calibration_data < 0)
            calibration_data = 70;

        //Conversion needed?
        if (newMode == TIME_MODE) {
            converter = &Mode::secondsForGrams;
        } else if (mode == TIME_MODE) {
            converter = &Mode::gramsForSeconds;
        }

        if (converter != NULL) {
            for (unsigned char c = 0; c < DATA_PER_MODE; c++) {
                //special data is not converted
                if (data[c] != SPECIAL_DATA) {
                    data[c] = (*this.*converter)(data[c]);
                }
            }
        }

        if (newMode == SCALE_MODE)
            calibration_data = 0;
        mode = newMode;
    }
}

//Returns the seonds for the data
int Mode::getDeciSeconds(unsigned char time) {
    if (mode == WEIGHT_MODE) {
        //data is decigrams -> deciseconds
        return secondsForGrams(data[time]);
    }
    else
        //data is in seconds -> deciseconds
        return data[time];
}

//----------------------------------------------------------------------------------
//Time Mode List

EEMEM unsigned char eeprom_eeprom_version;

//The number of modes
EEMEM unsigned char eeprom_time_modes;

//names
EEMEM char eeprom_time_mode_names[ModeList::MAX_MODES][Mode::MAX_CHARS + 1];

EEMEM int eeprom_time_mode_times[ModeList::MAX_MODES][Mode::DATA_PER_MODE];
EEMEM unsigned char eeprom_time_mode_weight_modes[ModeList::MAX_MODES];
EEMEM int eeprom_time_mode_centi_seconds_per_grams[ModeList::MAX_MODES];

constexpr char *ModeList::INIT_NAMES[4];
constexpr char ModeList::INIT_WEIGHT_MODES[4];
constexpr int ModeList::INIT_TIMES[4][3];

ModeList::ModeList() {
    eepromRead();
}

//clones a time mode and inserts it after the one
Mode &ModeList::insertAfer(Mode &mode) {
    //find the position
    const unsigned char newPos = pos(mode) + 1U;
    //add a new mode there
    Mode &newMode = add(newPos);

    //Clone data
    newMode.mode = mode.mode;
    newMode.calibration_data = mode.calibration_data;
    for (unsigned char c = 0; c < Mode::DATA_PER_MODE; c++)
        newMode.data[c] = mode.data[c];
    strcpy(newMode.name, mode.name);

    //Add marker
    unsigned char pos = Mode::MAX_CHARS - 1;
    while (newMode.name[pos] == ' ' && pos > 0)
        pos--;
    newMode.name[pos + 1] = '*';

    return newMode;
}

Mode &ModeList::add(unsigned char pos) {
    //move time modes to the right
    for (unsigned char p = pos; p < size; p++) {
        timeModes[p + 1] = timeModes[p];
    }

    //construct new mode
    timeModes[pos] = *new Mode();
    DEBUG_PRINTLN("pos")
    DEBUG_PRINTLN(pos)
    DEBUG_PRINTLN(size)
    //Is there a template?
    if (size < INIT_TEMPLATES) {
        DEBUG_PRINTLN("t")
        //Initialize the name
        strcpy(timeModes[pos].name, INIT_NAMES[size]);

        for (unsigned char c = strlen(timeModes[pos].name); c < Mode::MAX_CHARS; c++)
            timeModes[pos].name[c] = '@';
        timeModes[pos].name[Mode::MAX_CHARS] = 0;

        //initialize the data
        for (unsigned char c = 0; c < Mode::DATA_PER_MODE; c++)
            timeModes[pos].data[c] = INIT_TIMES[size][c];

        timeModes[pos].mode = INIT_WEIGHT_MODES[size];
        timeModes[pos].calibration_data = timeModes[pos].mode == Mode::SCALE_MODE ? 0 : 70;
    }

    size++;
    return timeModes[pos];
}

//---------------------------------------------
// Moving

void ModeList::swap(unsigned char from, unsigned char to) {
    Mode save = timeModes[from];
    timeModes[from] = timeModes[to];
    timeModes[to] = save;
}

Mode &ModeList::moveLeft(Mode &mode) {
    const unsigned char p = pos(mode);
    const unsigned char p_ex = p > 0 ? p - 1 : size - 1;
    swap(p, p_ex);
    return timeModes[p_ex];
}

Mode &ModeList::moveRight(Mode &mode) {
    const unsigned char p = pos(mode);
    const unsigned char p_ex = p < size - 1 ? p + 1 : 0;
    swap(p, p_ex);
    return timeModes[p_ex];
}

//-------------------------------------------
// Deleting

Mode &ModeList::del(Mode &tm_del) {
    //if this is the last mode do a reset
    if (size > 1) {
        unsigned char del = pos(tm_del);
        //Move to the right
        for (unsigned char m = del; m < size; m++) {
            timeModes[m] = timeModes[m + 1];
        }
        size--;
        return timeModes[(del == size ? (del - 1) : del)];
    } else {
        reset();
        return timeModes[0];
    }
}

//Find mode
unsigned char ModeList::pos(Mode &tm) {
    unsigned char p = 0;
    while (p < size && &timeModes[p] != &tm)
        p++;
    return p;
}

void ModeList::reset() {
    size = 0;
    for (unsigned char c = 0; c < INIT_TEMPLATES; c++) {
        add(size);
    }
    eepromWrite();
}

void ModeList::eepromRead() {
#ifdef RESET_MODE
    if (false) {
#else
    if (EEPROM_VERSION == eeprom_read_byte(&eeprom_eeprom_version)) {
#endif
        eeprom_read_block(&size, &eeprom_time_modes, sizeof(unsigned char));
        for (unsigned char c = 0; c < size; c++) {
            timeModes[c].mode = eeprom_read_byte(&eeprom_time_mode_weight_modes[c]);
            eeprom_read_block(&timeModes[c].calibration_data, &eeprom_time_mode_centi_seconds_per_grams[c],
                              sizeof(int));
            eeprom_read_block(timeModes[c].name, eeprom_time_mode_names[c], Mode::MAX_CHARS + 1);
            eeprom_read_block(timeModes[c].data, eeprom_time_mode_times[c], Mode::DATA_PER_MODE * sizeof(int));
        }
    } else {
        reset();
        eeprom_write_byte(&eeprom_eeprom_version, EEPROM_VERSION);
    }
    //I had strange race conditions here - adding a delay seemed to save the machine
    _delay_ms(10);
}

/* write settings to EEPROM */
void ModeList::eepromWrite() {
    eeprom_update_block(&size, &eeprom_time_modes, sizeof(unsigned char));

    for (unsigned char c = 0; c < size; c++) {
        eeprom_write_byte(&eeprom_time_mode_weight_modes[c], timeModes[c].mode);
        eeprom_update_block(&timeModes[c].calibration_data, &eeprom_time_mode_centi_seconds_per_grams[c],
                            sizeof(int));
        eeprom_update_block(timeModes[c].name, &eeprom_time_mode_names[c], Mode::MAX_CHARS + 1);
        eeprom_update_block(timeModes[c].data, eeprom_time_mode_times[c], Mode::DATA_PER_MODE * sizeof(int));
    }
}


bool Mode::isWeightMode() const {
    return !mode == TIME_MODE;
}
