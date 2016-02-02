#pragma once

class Rotator {

public:
    virtual int getPosition();

    virtual bool updated();

    virtual void setPosititon(int);

    virtual void setMode(int, int);
};
