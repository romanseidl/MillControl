#pragma once

#include "TimeMode.h"
#include "TimeLine.h"
#include "State.h"

static constexpr char SYMBOL_BACK[2] = {171, 0};

class TimeModeSelector;

class Editor : public TitledState, public TimeLine {
    static const char DEL_STRING[2];
    static const char ADD_STRING[2];
    static const char MOVE_LEFT_STRING[2];
    static const char MOVE_RIGHT_STRING[2];

    int position;
public:
    virtual void start() override;

    virtual void stop() override;

    unsigned char add = 0;
    unsigned char del = 0;
    unsigned char left = 0;
    unsigned char right = 0;
    unsigned char firstChar = 0;

    bool deleteMode = false;

    TimeMode* timeMode;
    TimeModeList* timeModes;

    void drawEditPoint(unsigned char p, const char *symbol, const unsigned char pos);
    void draw(bool editor);
public:
    void encoderClick();

    virtual void encoderChanged(int encoderPos) override;

    virtual void draw();

    int *getTime();

    char *getChar();

    void drawEditor();

};

