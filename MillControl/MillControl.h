#pragma once
/*
 * App Class
 * Here everything is static.
 */
#include "State.h"
#include "TimeModeSelector.h"
#include "Editor.h"
#include "TimeEditor.h"
#include "CharEditor.h"
#include "Run.h"
#include "CalibrationRun.h"
#include "CalibrationPrompt.h"

#ifdef BREW_BUTTON
    #include "BrewTimer.h"
#endif

class MillControl{
    static State* state;
public:
    static const unsigned char CLICK = 0;
    static const unsigned char DOUBLE_CLICK = 1;
    static const unsigned char LONG_CLICK = 2;

    static TimeModeSelector     TIME_MODE_SELECTOR;
    static Editor               EDITOR;
    static TimeEditor           TIME_EDITOR;
    static CharEditor           CHAR_EDITOR;
    static CalibrationRun WEIGHT_CALIBRATOR;
    static CalibrationPrompt    CALIBRATION_PROMPT;
    static Run                  RUN;

#ifdef BREW_BUTTON
    static BrewTimer            BREW_TIMER;
#endif

    static void setup();
    static void loop();
    static void setState(State &_state);
    static State* getState();

    static void redraw();

};

