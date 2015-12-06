#pragma once

#include "TimeLine.h"
#include "State.h"
#include "TimeMode.h"


class TimeModeSelector : public State, public TimeLine {
    int selectedMode;
    TimeModeList timeModes;
public:
    virtual void stop() override;

    TimeModeSelector();

    void start();

    virtual void draw() override;

    virtual void encoderClick() override;

    virtual void millClick(unsigned char clickType) override;

    virtual void encoderChanged(int encoderPos) override;

    TimeMode &getMode();

    void setMode(TimeMode *mode);

    int size();

    TimeModeList & getTimeModes();

    void eepromWrite();

    void setEncoderPos(int encoderPos);
};

