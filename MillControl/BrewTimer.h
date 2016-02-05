#ifndef MILLCONTROL_BREWTIMER_H
#define MILLCONTROL_BREWTIMER_H

#include "State.h"

#ifdef BREW_BUTTON

class BrewTimer : public State {
    unsigned long startTime = 0;
    unsigned long updateTime = 0;
protected:
    virtual void draw() override;

public:
    virtual void loop() override;

    virtual bool start() override;

    virtual void millClick(unsigned char i) override;
    virtual void encoderClick() override;

    virtual void encoderChanged(int encoderPos) override;

    bool isRunning();

    virtual bool open() override;

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "BrewTimer"; }
#endif
};

#endif

#endif //MILLCONTROL_BREWTIMER_H