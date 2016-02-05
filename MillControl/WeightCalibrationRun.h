#ifndef MILLCONTROL_WEIGHTCALIBRATONRUN_H
#define MILLCONTROL_WEIGHTCALIBRATONRUN_H

#include "BrewlessState.h"

class WeightCalibrationRun : public BrewlessState {
    unsigned long startTime = 0;
    unsigned long stopTime = 0;
    unsigned long updateTime = 0;

public:
    virtual bool open() override;
    virtual bool start() override;
    virtual void stop() override;

    virtual void loop() override;

    virtual void millClick(unsigned char i) override;
    virtual void encoderClick() override;

    virtual void draw() override;

    unsigned long getRunMillis();

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "WeightCalibrationRun"; }
#endif
};

#endif //MILLCONTROL_WEIGHTCALIBRATONRUN_H