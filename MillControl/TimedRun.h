#ifndef MILLCONTROL_TIMEDRUN_H
#define MILLCONTROL_TIMEDRUN_H


#include "Run.h"

class TimedRun : public Run {
private:
    unsigned long stopTime;
    unsigned long pauseTime;
    int lastEncoderPos;
protected:
    virtual void loop() override;

    virtual void draw() override;

public:
    TimedRun(const unsigned char run_icon, const int run_data) : Run(run_icon, run_data) { }

    virtual void millClick(unsigned char i) override;

    virtual void encoderChanged(int encoderPos) override;

    virtual bool start() override;
};


#endif //MILLCONTROL_TIMEDRUN_H
