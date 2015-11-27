#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "TimeMode.h"

EEMEM unsigned char eeprom_eeprom_version;
EEMEM int eeprom_time_modes;
EEMEM char eeprom_time_mode_names[TimeModeList::MAX_MODES][TimeMode::MAX_CHARS + 1];
EEMEM int eeprom_time_mode_times[TimeModeList::MAX_MODES][TimeMode::TIMES_PER_MODE];

constexpr char *TimeModeList::INIT_NAMES[INIT_TEMPLATES];
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

//insert after
TimeMode &TimeModeList::insertAfer(TimeMode &mode) {
    return add(pos(mode) + 1);
}

//Add to the end
TimeMode &TimeModeList::add() {
    return add(size());
}

TimeMode &TimeModeList::add(int pos) {
    for (int p = pos; p < size(); p++) {
        timeModes[p + 1] = timeModes[p];
    }

    timeModes[pos] = *new TimeMode();

    //Initialize the name
    const int selectedTemplate = size() > INIT_TEMPLATES - 1 ? INIT_TEMPLATES - 1 : size();

    sprintf(timeModes[pos].name, TimeModeList::INIT_NAMES[selectedTemplate], size() + 1);
    int c = strlen(timeModes[pos].name);
    for (; c < TimeMode::MAX_CHARS; c++)
        timeModes[pos].name[c] = 32;
    timeModes[pos].name[TimeMode::MAX_CHARS] = 0;

    for (c = 0; c < TimeMode::TIMES_PER_MODE; c++)
        timeModes[pos].time[c] = TimeModeList::INIT_TIMES[selectedTemplate][c];

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
            eeprom_read_block(timeModes[c].name, eeprom_time_mode_names[c], TimeMode::MAX_CHARS + 1);
            eeprom_read_block(timeModes[c].time, eeprom_time_mode_times[c], TimeMode::TIMES_PER_MODE * sizeof(int));
        }
    } else {
        add();
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
        eeprom_update_block(timeModes[c].name, &eeprom_time_mode_names[c], TimeMode::MAX_CHARS + 1);
        eeprom_update_block(timeModes[c].time, eeprom_time_mode_times[c], TimeMode::TIMES_PER_MODE * sizeof(int));
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