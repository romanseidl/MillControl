#pragma once
#include "UI.h"

class Mode {
    int secondsForGrams(int grams) const;

public:
    static const int SPECIAL_DATA = 10000;
    static const int MAX_DATA = 9990;
    static const unsigned char MAX_CHARS = 10;

    #ifdef MILL_BUTTON
#ifdef FLAT_MODE
    static const unsigned char DATA_PER_MODE = 2;
#else
    static const unsigned char PROGMEM DATA_PER_MODE = 3;
#endif
    #else
        static const unsigned char DATA_PER_MODE = 2;
    #endif

    char name[MAX_CHARS + 1];
    int data[DATA_PER_MODE];

    bool weightMode = false;
    int centiSecondsPerGram = 70;

    int getDeciSeconds(unsigned char time);
    void setWeightMode(bool);
};

//--------------------------------------------------------------------------------------------
// ModeList

class ModeList {
    static const unsigned char EEPROM_VERSION = 3;
    static const unsigned char INIT_TEMPLATES = 3;
    static constexpr char *INIT_NAMES[INIT_TEMPLATES] = {(char *const) "Time",
                                                         (char *const) "Weight",
                                                         (char *const) "Direct"};
    static constexpr bool INIT_WEIGHT_MODES[INIT_TEMPLATES] = {false, true, false};
    static constexpr int INIT_TIMES[INIT_TEMPLATES][3] = {{50,                 100, 150},
                                                          {70,                 140, 160},
                                                          {Mode::SPECIAL_DATA, 0,   Mode::SPECIAL_DATA}};

public:
    static const unsigned char PROGMEM MAX_MODES = 20;
    unsigned char size = 0;
private:

    void swap(unsigned char from, unsigned char to);

    Mode &add(unsigned char pos);
public:
    ModeList();

    void reset();

    Mode timeModes[MAX_MODES];

    Mode &insertAfer(Mode &);

    unsigned char pos(Mode &);

    Mode &moveLeft(Mode &);

    Mode &moveRight(Mode &);

    Mode &del(Mode &);

    void eepromRead();

    void eepromWrite();

};





