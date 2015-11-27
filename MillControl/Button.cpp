#pragma once
#include "Button.h"
#include <TimerOne.h>

unsigned char Button::n_buttons = 0;
Button* Button::buttons[MAX_BUTTONS];

Button::Button(unsigned char pin, bool multiClick) : button(pin, LOW, false) {
    //configure clickbutton
    //button = *new ClickButton(pin, LOW, false);
    button.debounceTime = 5;
    if (multiClick)
        setMultiClickButton();
    else
        setSingleClickButton();

    // Register Button
    Button::buttons[n_buttons] = this;
    n_buttons++;

    //(re)start timer - Needs microseconds
    Timer1.initialize(100 * 1000);
    Timer1.attachInterrupt(Button::updateButtons);
}

//Updates all buttons
void Button::updateButtons() {
    for (unsigned char b = 0; b < n_buttons; b++)
        (*Button::buttons[b]).update();
}


//Update clickbutton and clicks
void Button::update() {
    button.Update();
    if (button.clicks != 0) {
        clicks = button.clicks;
    }
}

//Pulls clicks - resets the counter
unsigned char Button::getClicks() {
    unsigned char c = clicks;
    clicks = 0;
    return c;
}

//Sets the button to be responsive
void Button::setSingleClickButton() {
    button.clicks = 0;
    button.multiclickTime = CLICK_TIME_SHORT;
    button.longClickTime = CLICK_TIME_SHORT;
}

//Sets the button to allow multiple clicks and long clicks
void Button::setMultiClickButton() {
    button.clicks = 0;
    button.multiclickTime = CLICK_TIME_DOUBLE;
    button.longClickTime = CLICK_TIME_LONG;
}

// Is the button up?
bool Button::depressed() {
    return !button.depressed;
}
