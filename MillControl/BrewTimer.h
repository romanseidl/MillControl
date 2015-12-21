#pragma once
//
// Created by roman on 03.12.15.
//

#include "State.h"

#ifdef BREW_BUTTON

class BrewTimer : public State {
    unsigned long startTime = 0;
    unsigned long updateTime = 0;
    State* state;

public:
    virtual void loop() override;

    virtual void start() override;

    virtual void millClick(unsigned char i) override;
    virtual void encoderClick() override;

    virtual void encoderChanged(int encoderPos) override;

    void setReturnState(State* state);
protected:
    virtual void draw() override;

};

#endif