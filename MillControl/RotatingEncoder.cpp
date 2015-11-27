#include "RotatingEncoder.h"

RotatingEncoder::RotatingEncoder(unsigned char pin1, unsigned char pin2) {
    encoder = new Encoder(pin1, pin2);
}

int RotatingEncoder::getPosition() {
    const int p = (((*encoder).read() / 4) + startticks) % ticks;
    return p >= 0 ? p : ticks + p;
}

bool RotatingEncoder::updated() {
    int newpos = getPosition();
    bool change = pos != newpos;
    pos = newpos;
    return change;
}


void RotatingEncoder::setPosititon(int _startticks) {
    startticks = _startticks;
    (*encoder).write(0);
}

void RotatingEncoder::setMode(int _ticks, int _startticks) {
    ticks = _ticks;
    setPosititon(_startticks);
}
