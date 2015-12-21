#pragma once
//
// Created by roman on 03.12.15.
//

#include "State.h"

class CalibrationPrompt : public State {
    int deciGrams = 70;
public:
    virtual void start() override;

    virtual void encoderClick() override;

    virtual void encoderChanged(int encoderPos) override;

protected:
    virtual void draw() override;
};

