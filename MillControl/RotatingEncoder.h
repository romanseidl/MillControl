#pragma once

#include <Encoder.h>
#include "Rotator.h"

class RotatingEncoder : public Rotator {
    Encoder *encoder;
    int ticks;
    int startticks;
    int pos;
  
  public:
    RotatingEncoder(unsigned char, unsigned char);

    virtual int getPosition() override;

    virtual bool updated() override;

    virtual void setPosititon(int i) override;

    virtual void setMode(int ticks, int pos) override;
};
