#ifndef MILLCONTROL_CHAREDITOR_H
#define MILLCONTROL_CHAREDITOR_H

#include "State.h"

class CharEditor : public State {
    char *c;
public:

    virtual bool start() override;

    virtual void encoderClick() override;

    virtual void draw() override;

    virtual void encoderChanged(int encoderPos) override;

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "CharEditor"; }
#endif
};

#endif //MILLCONTROL_CHAREDITOR_H