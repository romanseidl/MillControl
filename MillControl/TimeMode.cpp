#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "TimeMode.h"

inline int TimeMode::secondsForGrams(int grams) const {
    const long i = (((long) grams * (long) centiSecondsPerGram) / 100l);
    return (int) (i > MAX_DATA ? MAX_DATA : i);
}

void TimeMode::setWeightMode(bool newMode) {
    if(weightMode != newMode){
        weightMode = newMode;

        if(weightMode){
            //deciseconds to decigrams
            for(unsigned char c = 0; c < DATA_PER_MODE; c++) {
                if(data[c] != SPECIAL_DATA) {
                    //Reconsider rounding problems!
                    const long i = ((long) data[c] * 100l) / (long) centiSecondsPerGram;
                    data[c] = (int) (i > MAX_DATA ? MAX_DATA : i);
                }
            }
        } else {
            //decigrams to deciseconds
            for (unsigned char c = 0; c < DATA_PER_MODE; c++){
                if(data[c] != SPECIAL_DATA)
                    data[c] = secondsForGrams(data[c]);
            }
        }
    }
}

int TimeMode::getDeciSeconds(unsigned char time) {
    if(weightMode) {
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
EEMEM int eeprom_time_modes;
EEMEM char eeprom_time_mode_names[TimeModeList::MAX_MODES][TimeMode::MAX_CHARS + 1];
EEMEM int eeprom_time_mode_times[TimeModeList::MAX_MODES][TimeMode::DATA_PER_MODE];
EEMEM unsigned char eeprom_time_mode_weight_modes[TimeModeList::MAX_MODES];
EEMEM int eeprom_time_mode_centi_seconds_per_grams[TimeModeList::MAX_MODES];

constexpr char *TimeModeList::INIT_NAMES[INIT_TEMPLATES];
constexpr bool TimeModeList::INIT_WEIGHT_MODES[INIT_TEMPLATES];
constexpr int TimeModeList::INIT_TIMES[INIT_TEMPLATES][3];

TimeModeList::TimeModeList() {
    eepromRead();
}

int TimeModeList::size() {
    return timeModesCount;
}

TimeMode &TimeModeList::get(int p) {
    return timeModes[p];
}

//clones a time mode and inserts it after the one
TimeMode &TimeModeList::insertAfer(TimeMode &mode) {
    const int newPos = pos(mode) + 1;
    TimeMode& newMode = add(newPos);

    newMode.weightMode = mode.weightMode;
    newMode.centiSecondsPerGram = mode.centiSecondsPerGram;
    for (unsigned char c = 0; c < TimeMode::DATA_PER_MODE; c++)
        newMode.data[c] = mode.data[c];
    strcpy(newMode.name, mode.name);

    char n_buf[3] ="";
    const int number = size();
    bool digits = number > 9 ? 1 : 0;

    int pos = TimeMode::MAX_CHARS-1;
    while(newMode.name[pos] == ' ' && pos > digits)
        pos--;

    pos -= digits;

    sprintf(n_buf, "%i", number);
    newMode.name[pos] = n_buf [0];
    if(digits)
        newMode.name[pos+1] = n_buf [1];

    return newMode;
}

//Add to the end
TimeMode &TimeModeList::add() {
    return add(size());
}

TimeMode &TimeModeList::add(int pos) {
    //move time modes
    for (int p = pos; p < size(); p++) {
        timeModes[p + 1] = timeModes[p];
    }

    timeModes[pos] = *new TimeMode();

    //Is there a tmeplate?
    const int selectedTemplate = size();
    if(selectedTemplate < INIT_TEMPLATES) {
        //Initialize the name
        strcpy(timeModes[pos].name, INIT_NAMES[selectedTemplate]);
        for (unsigned char c = strlen(timeModes[pos].name); c < TimeMode::MAX_CHARS; c++)
            timeModes[pos].name[c] = 32;
        timeModes[pos].name[TimeMode::MAX_CHARS] = 0;

        //initialize the data
        for (unsigned char c = 0; c < TimeMode::DATA_PER_MODE; c++)
            timeModes[pos].data[c] = INIT_TIMES[selectedTemplate][c];

        timeModes[pos].weightMode = INIT_WEIGHT_MODES[selectedTemplate];
    }

    timeModesCount++;
    return timeModes[pos];
}


void TimeModeList::swap(int from, int to) {
    TimeMode save = timeModes[from];
    timeModes[from] = timeModes[to];
    timeModes[to] = save;
}

TimeMode &TimeModeList::del(TimeMode &tm_del) {
    int del = pos(tm_del);
    for (uint8_t m = del; m < timeModesCount; m++) {
        timeModes[m] = timeModes[m + 1];
    }
    timeModesCount--;
    return get(del == size() ? (del - 1) : del);
}

int TimeModeList::pos(TimeMode &tm) {
    int p = 0;
    while (p < size() && &timeModes[p] != &tm)
        p++;
    return p;
}

void TimeModeList::eepromRead() {
    if (EEPROM_VERSION == eeprom_read_byte(&eeprom_eeprom_version)) {
        eeprom_read_block(&timeModesCount, &eeprom_time_modes, sizeof(int));
        for (unsigned char c = 0; c < timeModesCount; c++) {
            timeModes[c].weightMode = eeprom_read_byte(&eeprom_time_mode_weight_modes[c]);
            eeprom_read_block(&timeModes[c].centiSecondsPerGram, &eeprom_time_mode_centi_seconds_per_grams[c], sizeof(int));
            eeprom_read_block(timeModes[c].name, eeprom_time_mode_names[c], TimeMode::MAX_CHARS + 1);
            eeprom_read_block(timeModes[c].data, eeprom_time_mode_times[c], TimeMode::DATA_PER_MODE * sizeof(int));
        }
    } else {
        for(unsigned char c  =0 ; c< INIT_TEMPLATES; c++)
            add();
        eepromWrite();
        eeprom_write_byte(&eeprom_eeprom_version, EEPROM_VERSION);
    }
    //I had strange race conditions here - adding a delay seemed to save the machine
    _delay_ms(10);
}

/* write settings to EEPROM */
void TimeModeList::eepromWrite() {
    eeprom_update_block(&timeModesCount, &eeprom_time_modes, sizeof(int));

    for (unsigned char c = 0; c < timeModesCount; c++) {
        eeprom_write_byte(&eeprom_time_mode_weight_modes[c], timeModes[c].weightMode);
        eeprom_update_block(&timeModes[c].centiSecondsPerGram, &eeprom_time_mode_centi_seconds_per_grams[c], sizeof(int));
        eeprom_update_block(timeModes[c].name, &eeprom_time_mode_names[c], TimeMode::MAX_CHARS + 1);
        eeprom_update_block(timeModes[c].data, eeprom_time_mode_times[c], TimeMode::DATA_PER_MODE * sizeof(int));
    }
}

TimeMode &TimeModeList::moveLeft(TimeMode &mode) {
    const int p = pos(mode);
    const int p_ex = p > 0 ? p - 1 : size() - 1;
    swap(p, p_ex);
    return get(p_ex);
}

TimeMode &TimeModeList::moveRight(TimeMode &mode) {
    const int p = pos(mode);
    const int p_ex = p < size() - 1 ? p + 1 : 0;
    swap(p, p_ex);
    return get(p_ex);
}

