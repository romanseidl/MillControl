//
// Created by roman on 15.01.16.
//

#include "RotatingButtons.h"

RotatingButtons::RotatingButtons(unsigned char pin1, unsigned char pin2) {
    MultiTimer1::add(this);
    button1 = new Button(pin1);
    button2 = new Button(pin2);
}

int RotatingButtons::getPosition() {
    return pos;
}

bool RotatingButtons::updated() {
    if (pos_updated) {
        pos_updated = false;
        return true;
    } else
        return false;
}

void RotatingButtons::setPosititon(int p) {
    pos = p;
}

void RotatingButtons::setMode(int _ticks, int _pos) {
    pos = _pos;
    ticks = _ticks;
}

void RotatingButtons::run() {
    if (button1->getClicks()) {
        pos = pos > 0 ? pos - 1 : pos = ticks - 1;
        pos_updated = true;
//        DEBUG_PRINTLN("<")
    }
    else if (button2->getClicks()) {
        pos = pos < ticks - 1 ? pos + 1 : 0;
        pos_updated = true;
//        DEBUG_PRINTLN(">")
    }
}
