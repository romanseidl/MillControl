#pragma once

#include "ClickButton.h" // https://code.google.com/p/clickbutton/

class Button {
    unsigned char clicks = 0;
    ClickButton button;
//Can handle up to four buttons - change to allow for more
    static const unsigned char MAX_BUTTONS = 4;
    static const int CLICK_TIME_SHORT = 100;
    static const int CLICK_TIME_DOUBLE = 500;
    static const int CLICK_TIME_LONG = 1000;
public:
    static Button *buttons[];
    static unsigned char n_buttons;

    static const bool SINGLE_CLICK = false;
    static const bool MULTI_CLICK = true;
    static void updateButtons();

    Button(unsigned char, bool);

    void update();

    unsigned char getClicks();

    void setSingleClickButton();

    void setMultiClickButton();

    bool depressed();
};


