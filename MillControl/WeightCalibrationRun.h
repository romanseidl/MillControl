#pragma once
//
// Created by roman on 03.12.15.
//

#include "State.h"
#include "CalibrationRun.h"

class WeightCalibrationRun : public CalibrationRun {
    unsigned long startTime = 0;
    unsigned long stopTime = 0;
    unsigned long updateTime = 0;
    boolean holdRun;

public:

    virtual void loop() override;

    virtual void start() override;

    virtual void millClick(unsigned char i) override;

    virtual void encoderClick() override;

    unsigned long getRunMillis();

protected:
    virtual void draw() override;

public:
    virtual void stop() override;
};

