#pragma once

#include "State.h"

//==================================================================================================
// EditTime State

class CharEditor : public State {
    static const char CHARACTERS[];
    static const unsigned char CHARACTERS_COUNT;

    char *c;
public:

    virtual void start() override;

    virtual void encoderClick() override;

    virtual void draw() override;

    virtual void encoderChanged(int encoderPos) override;

};


