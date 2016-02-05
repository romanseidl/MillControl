#ifndef MILLCONTROL_ROTATOR_H
#define MILLCONTROL_ROTATOR_H

class Rotator {

public:
    virtual int getPosition();

    virtual bool updated();

    virtual void setPosititon(int);

    virtual void setMode(int, int);
};

#endif //MILLCONTROL_ROTATOR_H