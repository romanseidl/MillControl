//
// Created by roman on 03.12.15.
//


#include "BrewTimer.h"
#include "MillControl.h"

#ifdef BREW_BUTTON

//Initialize the timer
void BrewTimer::start() {
    updateTime = 0;
    startTime = millis();
    redraw();
}

//Update Display & stop if brewer depressed
void BrewTimer::loop() {
    if(UI::brewButton.depressed())
        encoderClick();
    else if (millis() > (updateTime + 250)) {
        updateTime = millis();
        redraw();
    }
}

//Pass through the mill click
void BrewTimer::millClick(unsigned char i) {
    MillControl::setState(*state);
    (*state).millClick(i);
}

//Get back to previous state
void BrewTimer::encoderChanged(int encoderPos) {
    encoderClick();
}

//Get back to previous state
void BrewTimer::encoderClick() {
    MillControl::setState(*state);
}

//Very simple for now
void BrewTimer::draw() {
    UI::u8g.setFont(UI::FONT_REGULAR);
#ifdef PORTRAIT_DISPLAY
    UI::u8g.drawStr270(UI::LINE_HEIGHT, UI::u8g.getStrWidth(UI::BREW_TITLE), UI::BREW_TITLE);
    const char x= UI::DISPLAY_WIDTH - UI::SMALL_LINE_HEIGHT;
    const char y= UI::DISPLAY_HEIGHT - UI::BORDER_WIDTH - 1; //Bottom
#else
    UI::u8g.drawStr(0, UI::LINE_HEIGHT, UI::BREW_TITLE);
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT - 1; //89
    const char y = UI::DISPLAY_HEIGHT - 3 * UI::BORDER_WIDTH - 1;//57;
#endif

    UI::drawRunTime(x, y, (millis() - startTime) / 1000);
}

//Needed as this state does need to know where to return to
void BrewTimer::setReturnState(State *_state) {
    state = _state;
}

#endif