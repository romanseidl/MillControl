#ifndef MILLCONTROL_MILLCONTROL_H
#define MILLCONTROL_MILLCONTROL_H

/*
 * App Class
 * Here everything is static.
 */
#include "State.h"
#include "ModeSelector.h"
#include "Editor.h"
#include "DataEditor.h"
#include "CharEditor.h"
#include "Run.h"
#include "WeightCalibrationRun.h"
#include "CalibrationPrompt.h"

#ifdef FLAT_MODE
#include "FlatModeSelector.h"
#else
#include "MultiModeSelector.h"
#endif

#ifdef BREW_BUTTON
    #include "BrewTimer.h"
#endif

#ifdef SCALE
#include "ScaleTarePrompt.h"
#endif

class MillControl{
    static State* state;

public:
    static const unsigned char CLICK = 0;
    static const unsigned char DOUBLE_CLICK = 1;
    static const unsigned char LONG_CLICK = 2;

#ifdef FLAT_MODE
    static FlatModeSelector     TIME_MODE_SELECTOR;
#else
    static MultiModeSelector TIME_MODE_SELECTOR;
#endif
    static Editor                   EDITOR;
    static DataEditor TIME_EDITOR;
    static CharEditor               CHAR_EDITOR;
    static WeightCalibrationRun WEIGHT_CALIBRATOR;
    static CalibrationPrompt        CALIBRATION_PROMPT;
    static DataEditor CALIBRATION_TIME_EDITOR;

#ifdef SCALE
    static ScaleTarePrompt SCALE_CALIBRATOR;
#endif

#ifdef BREW_BUTTON
    static BrewTimer BREW_TIMER;
#endif

    static void setup();
    static void loop();

    static void open(State &newState);
    static void start(State &newState, bool open = false);

    static void close(State *closeState);

    static State *remove(State *state);

    static void openInBackground(State &newState);
};

#endif //MILLCONTROL_MILLCONTROL_H