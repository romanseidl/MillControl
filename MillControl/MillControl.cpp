//
// Created by roman on 26.11.15.
//
#include "MillControl.h"
#include "CalibrationPrompt.h"

TimeModeSelector        MillControl::TIME_MODE_SELECTOR;
Editor                  MillControl::EDITOR;
TimeEditor              MillControl::TIME_EDITOR;
CharEditor              MillControl::CHAR_EDITOR;
Run                     MillControl::RUN;
WeightCalibrationRun    MillControl::WEIGHT_CALIBRATOR;
CalibrationPrompt       MillControl::CALIBRATION_PROMPT;

State *MillControl::state = &TIME_MODE_SELECTOR;

void MillControl::setup() {
    state->start();
}

void MillControl::setState(State &_state) {
    state->stop();
    state = &_state;
    state->start();
}

//Handle user input
void MillControl::loop() {
    const char millClicks = UI::millButton.getClicks();
#ifdef MILL_BUTTON_2
    const char mill2Clicks = UI::millButton2.getClicks();
#endif

    if (millClicks != 0) {
#ifdef MILL_BUTTON
        state->millClick(millClicks > 0 ? (millClicks == 1 ? CLICK : DOUBLE_CLICK) : LONG_CLICK);
    }
#ifdef MILL_BUTTON_2
    else if (mill2Clicks != 0) {
        state->millClick(mill2Clicks > 0 ? DOUBLE_CLICK : LONG_CLICK);
    }
#endif
    else if (UI::encoderButton.getClicks() != 0) {
        state->encoderClick();
    }
#else
        if(state == &TIME_MODE_SELECTOR){
            if(millClicks <= 0)
               state->encoderClick();
            else
                state->millClick(millClicks == 1 ? CLICK : DOUBLE_CLICK);
        } else if(state == &EDITOR){
            if(millClicks < 0)
               state->millClick(CLICK);
            else
               state->encoderClick();
        } else {
            state->encoderClick();
        }
    }
#endif
    else if (UI::encoder.updated()) {
        state->encoderChanged(UI::encoder.getPosition());
    } else
        state->loop();
}



