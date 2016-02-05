#include "State.h"
#include "MillControl.h"

void State::redraw() {
    DEBUG_PRINTLN("redraw()");
    UI::u8g.firstPage();
    do {
        draw();
    } while (UI::u8g.nextPage());
}

void State::setEncoderMode(int positions, int position) {
    UI::rotator->setMode(positions, position);
}

void State::millClick(unsigned char clickType) {
}

#ifdef BREW_BUTTON
//Will open the brew timer
void State::brewClick() {
    DEBUG_PRINTLN("State::brewClick()");
    MillControl::open(MillControl::BREW_TIMER);
}
#endif

// Called when ste state is stopped
void State::stop() {
}

// Called when the state is opened, defaults to doing the same as when started
bool State::open() {
    return start();
}

// Is called when the State is started
bool State::start() {
    return true;
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

//go start to the previous state
bool State::close() {
    if (!previousState == NULL) {
        MillControl::close(this);
        return true;
    } else
        return false;
}

