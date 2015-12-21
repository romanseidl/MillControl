//
// Created by roman on 03.12.15.
//

#include "CalibrationRun.h"
#include "MillControl.h"

void CalibrationRun::start() {
    updateTime = 0;
    startTime = millis();
    MillControl::RUN.startMill();
    redraw();
}

void CalibrationRun::loop() {
#ifdef MILL_BUTTON
    if(UI::millButton.depressed())
        encoderClick();
#endif

    if (millis() > (updateTime + 250)) {
        updateTime = millis();
        redraw();
    }
}

void CalibrationRun::stop() {
    stopTime = millis();
    MillControl::RUN.stopMill();
}

void CalibrationRun::millClick(unsigned char i) {
    encoderClick();
}


void CalibrationRun::encoderClick() {
    MillControl::setState(MillControl::CALIBRATION_PROMPT);
}

void CalibrationRun::draw() {
    State::draw();
    UI::drawSubtitle(UI::CALIBRATION_TITLE);

#ifdef PORTRAIT_DISPLAY
    const char x= UI::DISPLAY_WIDTH - UI::SMALL_LINE_HEIGHT - UI::LINE_HEIGHT - UI::BORDER_WIDTH;
    const char y= UI::DISPLAY_HEIGHT - UI::BORDER_WIDTH - 1;
#else
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT - 1; //89
    const char y = UI::DISPLAY_HEIGHT - 3 * UI::BORDER_WIDTH - 1;//57;
#endif

    UI::drawRunTime(x, y, (millis() - startTime) / 1000);
}

unsigned long CalibrationRun::getRunMillis() {
    return stopTime - startTime;
}

#ifdef BREW_BUTTON
//No brew timer here
void CalibrationRun::brewClick() {
}
#endif