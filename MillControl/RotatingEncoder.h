#pragma once

#include <Encoder.h>

class RotatingEncoder {
    Encoder *encoder;
    int ticks;
    int startticks;
    int pos;
  
  public:
    RotatingEncoder(unsigned char, unsigned char);
    void setPosititon(int);
    void setMode(int, int);
    int getPosition();
    bool updated();
};
