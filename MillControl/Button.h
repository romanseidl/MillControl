#ifndef MILLCONTROL_BUTTON_H
#define MILLCONTROL_BUTTON_H

#include "MultiTimer1.h"

class Button : public Timed {
    char clicks = 0; //clicks if any since the last getClicks();

    //Can handle up to four buttons - change to allow for more
    static const int DEBOUNCE_TIME = 30; //Milliseconds to wait for a stable state
    static const int CLICK_TIME_MULTI = 500;
    static const int CLICK_TIME_LONG = 1000;

    unsigned char pin;        // Arduino pin connected to the button
    bool multiClick;          // Is this a multi-click button?

    bool lastState = false;   // previous button reading
    bool stableState = false;// the currently debounced button (press) state

    char clickCount = 0;      // Number of button clicks within a CLICK_TIME_MULTI
    long lastBounceTime = 0;  // the last time the button input pin was toggled, due to noise or a press
public:
    static const bool SINGLE_CLICK = false;
    static const bool MULTI_CLICK = true;

    Button(unsigned char, bool = SINGLE_CLICK);

    void setMultiClick(bool _multiClick);

    void run() override;

    char getClicks();

    bool isPressed();
};

#endif //MILLCONTROL_BUTTON_H
