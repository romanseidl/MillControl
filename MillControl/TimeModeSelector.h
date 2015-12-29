#pragma once

#include "State.h"
#include "TimeMode.h"


class TimeModeSelector : public State {
    int selectedMode;
    TimeModeList timeModes;
    unsigned long updateTime;
public:
    virtual bool start() override;
    virtual void stop() override;

    virtual void loop() override;

    TimeModeSelector();

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

#ifdef DEBUG
    virtual char*  getClassName() {return "TimeModeSelector"; }
#endif
};

