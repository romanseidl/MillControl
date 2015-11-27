#pragma once

#include "UI.h"

class TimeMode {
public:
    static const int SPECIAL_TIME = 999;

    static const unsigned char MAX_CHARS = 10;

    #ifdef MILL_BUTTON
        static const unsigned char TIMES_PER_MODE = 3;
    #else
        static const unsigned char TIMES_PER_MODE = 2;
    #endif

    char name[MAX_CHARS + 1];
    int time[3] = {0, 0, 0};
};

//--------------------------------------------------------------------------------------------
// TimeModeList

class TimeModeList {
    static const unsigned char EEPROM_VERSION = 1;
    static const int INIT_TEMPLATES = 3;
    static constexpr char *INIT_NAMES[INIT_TEMPLATES] = {"Timer", "Direct", "%i"};
    static constexpr int INIT_TIMES[INIT_TEMPLATES][3] = {{30,  60, 90},
                                                                                 {999, 0,  999},
                                                                                 {0,   0,  0}};

public:
    static const unsigned char MAX_MODES = 20;
private:
    TimeMode timeModes[MAX_MODES];
    int timeModesCount=0;

    void swap(int tm_from, int tm_to);
    TimeMode &add(int);
public:
    TimeModeList();

    TimeMode &add();

    TimeMode &insertAfer(TimeMode&);

    int size();

    int pos(TimeMode &);

    TimeMode& moveLeft(TimeMode &);

    TimeMode& moveRight(TimeMode &);

    TimeMode &del(TimeMode &);

    void eepromRead();

    void eepromWrite();

    TimeMode &get(int);

};





