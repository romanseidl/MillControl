#pragma once
//
// Created by roman on 03.12.15.
//

#include "State.h"

class CalibrationRun : public State {
    unsigned long startTime = 0;
    unsigned long stopTime = 0;
    unsigned long updateTime = 0;

public:
    virtual bool open() override;
    virtual bool start() override;
    virtual void stop() override;

    virtual void loop() override;

#ifdef BREW_BUTTON
    virtual void brewClick() override;
#endif

    virtual void millClick(unsigned char i) override;
    virtual void encoderClick() override;

    virtual void draw() override;

    unsigned long getRunMillis();

#ifdef DEBUG
    virtual char*  getClassName() {return "CalibrationRun"; }
#endif
};

