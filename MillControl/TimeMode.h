#pragma once
#include "UI.h"

class TimeMode {
    int secondsForGrams(int grams) const;

public:
    static const int SPECIAL_DATA = 10000;
    static const int MAX_DATA = 9990;
    static const unsigned char MAX_CHARS = 10;

    #ifdef MILL_BUTTON
        static const unsigned char DATA_PER_MODE = 3;
    #else
        static const unsigned char DATA_PER_MODE = 2;
    #endif

    char name[MAX_CHARS + 1];
    int data[3] = {0, 0, 0};

    bool weightMode = false;
    int centiSecondsPerGram = 70;

    int getDeciSeconds(unsigned char time);
    void setWeightMode(bool);
};

//--------------------------------------------------------------------------------------------
// TimeModeList

class TimeModeList {
    static const unsigned char EEPROM_VERSION = 2;
    static const int INIT_TEMPLATES = 3;
    static constexpr char *INIT_NAMES[INIT_TEMPLATES] = {"Time", "Weight", "Direct"};
    static constexpr bool INIT_WEIGHT_MODES[INIT_TEMPLATES] = {false, true, false};
    static constexpr int INIT_TIMES[INIT_TEMPLATES][3] = {{50,                    100, 150},
                                                         {70,                     140, 160},
                                                         {TimeMode::SPECIAL_DATA, 0,   TimeMode::SPECIAL_DATA}};

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





