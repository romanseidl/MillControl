#include <avr/eeprom.h>
#include <util/delay.h>
#include "Mode.h"

inline int Mode::secondsForGrams(int grams) const {
    return min(MAX_DATA, (((long) grams * (long) centiSecondsPerGram) / 100l));
}

//changes the mode, true is for weight mode, false is for time mode
void Mode::setWeightMode(bool newMode) {
    if (weightMode != newMode) {
        weightMode = newMode;

        for (unsigned char c = 0; c < DATA_PER_MODE; c++) {
            //special data is not converted
            if (data[c] != SPECIAL_DATA) {
                if (weightMode) {
                    //deciseconds to decigrams
                    //Reconsider rounding problems!
                    data[c] = min(MAX_DATA, ((long) data[c] * 100l) / (long) centiSecondsPerGram);
                } else {
                    //decigrams to deciseconds
                    data[c] = secondsForGrams(data[c]);
                }
            }
        }
    }
}

//Returns the seonds for the data
int Mode::getDeciSeconds(unsigned char time) {
    if (weightMode) {
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

constexpr char *ModeList::INIT_NAMES[INIT_TEMPLATES];
constexpr bool ModeList::INIT_WEIGHT_MODES[INIT_TEMPLATES];
constexpr int ModeList::INIT_TIMES[INIT_TEMPLATES][3];

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
    newMode.weightMode = mode.weightMode;
    newMode.centiSecondsPerGram = mode.centiSecondsPerGram;
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
        DEBUG_PRINTLN(INIT_NAMES[0])
        DEBUG_PRINTLN(size)
        DEBUG_PRINTLN(INIT_NAMES[size])
        DEBUG_PRINTLN("--------------")
        DEBUG_PRINTLN(timeModes[pos].name)
        DEBUG_PRINTLN(F("x"))

        for (unsigned char c = strlen(timeModes[pos].name); c < Mode::MAX_CHARS; c++)
            timeModes[pos].name[c] = 32;
        timeModes[pos].name[Mode::MAX_CHARS] = 0;

        //initialize the data
        for (unsigned char c = 0; c < Mode::DATA_PER_MODE; c++)
            timeModes[pos].data[c] = INIT_TIMES[size][c];

        timeModes[pos].weightMode = INIT_WEIGHT_MODES[size];
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
            timeModes[c].weightMode = eeprom_read_byte(&eeprom_time_mode_weight_modes[c]);
            eeprom_read_block(&timeModes[c].centiSecondsPerGram, &eeprom_time_mode_centi_seconds_per_grams[c],
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
        eeprom_write_byte(&eeprom_time_mode_weight_modes[c], timeModes[c].weightMode);
        eeprom_update_block(&timeModes[c].centiSecondsPerGram, &eeprom_time_mode_centi_seconds_per_grams[c],
                            sizeof(int));
        eeprom_update_block(timeModes[c].name, &eeprom_time_mode_names[c], Mode::MAX_CHARS + 1);
        eeprom_update_block(timeModes[c].data, eeprom_time_mode_times[c], Mode::DATA_PER_MODE * sizeof(int));
    }
}



