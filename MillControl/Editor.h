#pragma once

#include "TimeMode.h"
#include "State.h"

static constexpr char SYMBOL_BACK[2] = {171, 0};

class TimeModeSelector;

class Editor : public State {
    int position = 0;

    unsigned char back = 0;
    unsigned char gram = 0;
    unsigned char add = 0;
    unsigned char del = 0;
    unsigned char left = 0;
    unsigned char right = 0;
    unsigned char firstChar = 0;

    bool deleteMode = false;

    TimeMode* timeMode;
    TimeModeList* timeModes;
public:
    virtual bool start() override;
    virtual void stop() override;

    void encoderClick();
    virtual void millClick(unsigned char i) override;
    virtual void encoderChanged(int encoderPos) override;

    virtual void draw();

    int *getTime();
    char *getChar();

    void drawEditPoint(unsigned char p, const unsigned char pos, const char *symbol) const;
    void draw(bool editor);
    void drawEditor();

#ifdef DEBUG
    virtual char*  getClassName() {return "Editor"; }
#endif
};


