#include "State.h"
#include "MillControl.h"

//--------------------------------------------------------------------------------
// State

void State::redraw() {
    UI::u8g.firstPage();
    do {
        draw();
    } while (UI::u8g.nextPage());
}

void State::setEncoderMode(int positions, int position) {
    UI::encoder.setMode(positions, position);
}

void State::millClick(unsigned char clickType) {
}

#ifdef BREW_BUTTON
//Will start the brew timer
void State::brewClick() {
    MillControl::BREW_TIMER.setReturnState(MillControl::getState());
    MillControl::setState(MillControl::BREW_TIMER);
}
#endif

void State::stop() {
}

void State::start() {
}

void State::loop() {
}

void State::encoderChanged(int encoderPos) {
}

void State::encoderClick() {
}

void State::draw() {
    UI::drawTitle(MillControl::TIME_MODE_SELECTOR.getMode().name);
}

