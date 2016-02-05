#ifndef MILLCONTROL_WEIGHTRUN_H
#define MILLCONTROL_WEIGHTRUN_H


#include "Run.h"

class WeightRun : public Run {
    int *calibration_data;
    long weight = 0;
    Mode *mode;
public:
    WeightRun(const unsigned char run_icon, const int run_data, Mode *mode) : Run(run_icon, run_data), mode(mode) { }

    virtual bool start() override;

    virtual bool close() override;

    virtual void loop() override;

    virtual void draw() override;

    virtual void millClick(unsigned char i) override;
};


#endif //MILLCONTROL_WEIGHTRUN_H
