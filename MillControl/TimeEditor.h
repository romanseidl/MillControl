#pragma once

#include "State.h"

//==================================================================================================
// EditTime State

class TimeEditor : public State {
    int *time;
public:

    virtual void start() override;

    virtual void encoderClick() override;

    virtual void draw() override;

    virtual void encoderChanged(int encoderPos) override;

};


