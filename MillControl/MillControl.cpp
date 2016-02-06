//
// Created by roman on 26.11.15.
//
#include "MillControl.h"

#ifdef FLAT_MODE

FlatModeSelector  MillControl::TIME_MODE_SELECTOR;
#else
MultiModeSelector MillControl::TIME_MODE_SELECTOR;
#endif

Editor                  MillControl::EDITOR;
DataEditor              MillControl::TIME_EDITOR(&MillControl::EDITOR);
CharEditor              MillControl::CHAR_EDITOR;
WeightCalibrationRun    MillControl::WEIGHT_CALIBRATOR;
CalibrationPrompt       MillControl::CALIBRATION_PROMPT;
DataEditor              MillControl::CALIBRATION_TIME_EDITOR(&MillControl::CALIBRATION_PROMPT);

#ifdef BREW_BUTTON
BrewTimer               MillControl::BREW_TIMER;
#endif

#ifdef SCALE
ScaleTarePrompt     MillControl::SCALE_CALIBRATOR;
#endif


State *MillControl::state = &TIME_MODE_SELECTOR;

void MillControl::setup() {
    state->open();
    state->redraw();
}

#ifdef aDEBUG
#define STACK() {\
        DEBUG_PRINTLN("Stack:");\
        State *s = state; \
        while (s != NULL){\
            DEBUG_PRINT("# "); \
            DEBUG_PRINTLN(s->getClassName());\
            s= s->previousState;\
        }\
    }
#else
#define STACK()
#endif

//start the previous state and remove from stack if possible
void MillControl::close(State *closeState) {
    DEBUG_PRINT("close: ");
    DEBUG_PRINTLN(closeState->getClassName());

    closeState->stop();
    if (remove(closeState) != NULL) {
        if (state->start()) {
            UI::u8g.begin();
            state->redraw();
        }
        else
            close(state);
    };
    STACK();
}

State *MillControl::remove(State *find) {
    DEBUG_PRINT("remove: ");
    DEBUG_PRINTLN(find->getClassName());

    if (find == state) {
        state = find->previousState;
        return state;
    } else {
        State *before = state;
        while (before != NULL && before->previousState != find)
            before = before->previousState;
        if (before != NULL)
            before->previousState = find->previousState;
        return NULL;
    }
}

void MillControl::open(State &newState) {
    start(newState, true);
}

void MillControl::openInBackground(State &newState) {
    DEBUG_PRINT("openInBackground: ");
    DEBUG_PRINTLN(newState.getClassName());
    bool opened = newState.open();
    if (opened) {
        //Insert state behind top
        remove(&newState);
        newState.previousState = state->previousState;
        state->previousState = &newState;
        STACK();
    }
}

void MillControl::start(State &newState, bool open) {
    DEBUG_PRINT("start: ");
    DEBUG_PRINTLN(newState.getClassName());

    //Stop the old state
    state->stop();

    //Pull it from the stack
    remove(&newState);

    //put it on top of the stack
    newState.previousState = state;
    state = &newState;

    bool started;
    if (!open) {
        started = newState.start();
    } else {
        started = newState.open();
    }

    //Do I need this Idea of a state not starting?
    if (started) {
        //redraw
        UI::u8g.begin();
        state->redraw();
    } else {
        //restart the old state
        state = newState.previousState;
        state->start();
    };
    STACK();
}


//Handle user input
void MillControl::loop() {

#ifdef BREW_BUTTON
    const char brewClicks = UI::brewButton.getClicks();
#endif

#ifdef MILL_BUTTON
    const char millClicks = UI::millButton.getClicks();
#ifdef MILL_BUTTON_2
    const char mill2Clicks = UI::millButton2.getClicks();
#endif
#else
    const char millClicks = UI::encoderButton.getClicks();
#endif

    if (millClicks != 0) {
        DEBUG_PRINTLN(millClicks);
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
#ifdef BREW_BUTTON
        // Only the long click is used
    else if (brewClicks != 0) {
        DEBUG_PRINTLN("loop::brewClick()");
        state->brewClick();
    }
#endif
    else if (UI::rotator->updated()) {
        state->encoderChanged(UI::rotator->getPosition());
    } else
        state->loop();

}


