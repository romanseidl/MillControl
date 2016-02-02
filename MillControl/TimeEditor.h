#pragma once

#include "State.h"

//==================================================================================================
// EditTime State

class TimeEditor : public State {
protected:
    int *time;
public:

    virtual bool start() override;

    virtual void encoderClick() override;

    virtual void draw() override;

    virtual void encoderChanged(int encoderPos) override;

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "TimeEditor"; }
#endif
};


