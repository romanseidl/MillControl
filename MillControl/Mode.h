#ifndef MILLCONTROL_MODE_H
#define MILLCONTROL_MODE_H

#include "UI.h"

class Mode {
    int secondsForGrams(int grams) const;

    int gramsForSeconds(int seconds) const;

public:
    static const int SPECIAL_DATA = 10000;
    static const int MAX_DATA = 9990;
    static const unsigned char MAX_CHARS = 10;

    static const char TIME_MODE = 0;
    static const char WEIGHT_MODE = 1;
    static const char SCALE_MODE = 2;

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

    char mode = TIME_MODE;
    int calibration_data;

    int getDeciSeconds(unsigned char time);

    void setMode(char newMode);

    bool isWeightMode() const;

};

//--------------------------------------------------------------------------------------------
// ModeList

class ModeList {
    static const unsigned char EEPROM_VERSION = 5;

#ifdef SCALE
    static const unsigned char INIT_TEMPLATES = 4;
#else
    static const unsigned char INIT_TEMPLATES = 3;
#endif

    static constexpr char *INIT_NAMES[4] = {(char *const) "Direct",
                                            (char *const) "Time",
                                            (char *const) "Weight",
                                            (char *const) "Scale"};
    static constexpr char INIT_WEIGHT_MODES[4] = {Mode::TIME_MODE, Mode::TIME_MODE, Mode::WEIGHT_MODE,
                                                  Mode::SCALE_MODE};
    static constexpr int INIT_TIMES[4][3] = {{Mode::SPECIAL_DATA, 0,   Mode::SPECIAL_DATA},
                                             {50,                 100, 150},
                                             {70,                 140, 160},
                                             {70,                 140, 160}};

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

#endif //MILLCONTROL_MODE_H
