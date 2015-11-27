//
// Created by roman on 26.11.15.
//
#include "MillControl.h"

TimeModeSelector MillControl::TIME_MODE_SELECTOR;
Editor           MillControl::EDITOR;
TimeEditor       MillControl::TIME_EDITOR;
CharEditor       MillControl::CHAR_EDITOR;
Run              MillControl::RUN;

State* MillControl::state = &TIME_MODE_SELECTOR;

void MillControl::setup() {
    state->start();
}

void MillControl::setState(State& _state) {
    state->stop();
    state = &_state;
    state->start();
}

//Handle user input
void MillControl::loop() {
    const char millClicks = UI::millButton.getClicks();
    if (millClicks != 0) {
#ifdef MILL_BUTTON
        state->millClick(millClicks > 0 ? (millClicks == 1 ? CLICK : DOUBLE_CLICK) : LONG_CLICK);
    } else if (UI::encoderButton.getClicks() != 0) {
        Serial.println("encoderClick");
        state->encoderClick();
    }
#else
        if(state == &TIME_MODE_SELECTOR){
            if(millClicks <= 0)
               state->encoderClick();
            else
                state->millClick(millClicks == 1 ? CLICK : DOUBLE_CLICK);
        } else {
            state->encoderClick();
        }
    }
#endif
    else if (UI::encoder.updated()) {
        state->encoderChanged( UI::encoder.getPosition());
    } else
        state->loop();
}



