#include "RotatingEncoder.h"

RotatingEncoder::RotatingEncoder(unsigned char pin1, unsigned char pin2) {
    encoder = new Encoder(pin1, pin2);
}

int RotatingEncoder::getPosition() {
    const int p = (((*RotatingEncoder::encoder).read() / 4) + RotatingEncoder::startticks) % RotatingEncoder::ticks;
    return p >= 0 ? p : RotatingEncoder::ticks + p;
}

bool RotatingEncoder::updated() {
    int newpos = getPosition();
    bool change = RotatingEncoder::pos != newpos;
    RotatingEncoder::pos = newpos;
    return change;
}

void RotatingEncoder::setPosititon(int _startticks) {
    RotatingEncoder::startticks = _startticks;
    (*RotatingEncoder::encoder).write(0);
}

void RotatingEncoder::setMode(int _ticks, int _startticks) {
    RotatingEncoder::ticks = _ticks;
    setPosititon(_startticks);
}

