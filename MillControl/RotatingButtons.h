#ifndef MILLCONTROL_ROTATINGBUTTONS_H
#define MILLCONTROL_ROTATINGBUTTONS_H

#include "Rotator.h"
#include "Button.h"
#include "Timed.h"

class RotatingButtons : public Rotator, Timed {

    Button *button1;
    Button *button2;
    int ticks;
    int pos;
    bool pos_updated = false;

public:
    RotatingButtons(unsigned char pin1, unsigned char pin2);

private:
    virtual void run() override;

    virtual int getPosition() override;

    virtual bool updated() override;

    virtual void setPosititon(int i) override;

    virtual void setMode(int ticks, int pos) override;

};

#endif //MILLCONTROL_ROTATINGBUTTONS_H