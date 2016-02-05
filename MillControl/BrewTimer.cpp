//
// Created by roman on 03.12.15.
//


#include <Arduino.h>
#include "BrewTimer.h"
#include "MillControl.h"

#ifdef BREW_BUTTON

//Initialize the timer
bool BrewTimer::open() {
    if (UI::brewButton.isPressed()) {
    DEBUG_PRINTLN("opened: BrewTimer");
    updateTime = 0;
    startTime = millis();
    return true;
  }
  return false;
}

//Return to the brew timer, will only start if it is alive
bool BrewTimer::start() {
    return isRunning();
}

//Checks if the Brew Timer is alive
bool BrewTimer::isRunning(){
    if (startTime > 0 && !UI::brewButton.isPressed()) {
        startTime = 0;
    }
    return startTime > 0;
}

//Update Display & stop if timer died
void BrewTimer::loop() {
    if(!isRunning())
        close();
    else if (millis() > (updateTime + 250)) {
        updateTime = millis();
        redraw();
    }
}

//Pass through the mill click
void BrewTimer::millClick(unsigned char i) {
    //Forward the click to the state below
    previousState->millClick(i);
}

//Get start to previous state
void BrewTimer::encoderChanged(int encoderPos) {
    encoderClick();
}

//Start to previous state
void BrewTimer::encoderClick() {
    MillControl::start(*previousState);
}

//simple for now
void BrewTimer::draw() {
    UI::u8g.setFont(UI::FONT_REGULAR);
#ifdef PORTRAIT_DISPLAY
    UI::u8g.drawStr270(UI::LINE_HEIGHT, UI::u8g.getStrWidth(UI::BREW_TITLE), UI::BREW_TITLE);
    UI::BREW_SYMBOL.draw(&UI::u8g, UI::DISPLAY_WIDTH - UI::BREW_SYMBOL.width, UI::BREW_SYMBOL.height);
    const char x= UI::DISPLAY_WIDTH - UI::SMALL_LINE_HEIGHT;
    const char y= UI::DISPLAY_HEIGHT - UI::BORDER_WIDTH - 1; //Bottom
#else
    UI::u8g.drawStr(0, UI::LINE_HEIGHT, UI::BREW_TITLE);
    UI::BREW_SYMBOL.draw(&UI::u8g, 0, UI::DISPLAY_HEIGHT - UI::BREW_SYMBOL.height/2);
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT - 1; //89
    const char y = UI::DISPLAY_HEIGHT - 3 * UI::BORDER_WIDTH - 1;//57;
#endif
    UI::drawRunTime(x, y, (millis() - startTime) / 1000);
}

#endif

