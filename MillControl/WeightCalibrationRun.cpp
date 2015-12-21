//
// Created by roman on 03.12.15.
//

#include "WeightCalibrationRun.h"
#include "MillControl.h"

void WeightCalibrationRun::start() {
    updateTime = 0;
    holdRun = !UI::millButton.depressed();
    startTime = millis();
    MillControl::RUN.startMill();
    redraw();
}

void WeightCalibrationRun::loop() {
#ifdef MILL_BUTTON
    if(holdRun && UI::millButton.depressed())
        encoderClick();
#endif

    if (millis() > (updateTime + 250)) {
        updateTime = millis();
        redraw();
    }
}

void WeightCalibrationRun::stop() {
    stopTime = millis();
    MillControl::RUN.stopMill();
}

void WeightCalibrationRun::millClick(unsigned char i) {
    encoderClick();
}


void WeightCalibrationRun::encoderClick() {
    MillControl::setState(MillControl::CALIBRATION_PROMPT);
}

void WeightCalibrationRun::draw() {
    CalibrationRun::draw();

#ifdef PORTRAIT_DISPLAY
    const char x=UI::DISPLAY_WIDTH - UI::SMALL_LINE_HEIGHT - UI::LINE_HEIGHT - UI::BORDER_WIDTH;
    const char y=UI::DISPLAY_HEIGHT - UI::BORDER_WIDTH - 1;
#else
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT - 1; //89
    const char y = UI::DISPLAY_HEIGHT - 3 * UI::BORDER_WIDTH - 1;//57;
#endif

    char c_t[6] = "";
    sprintf(c_t, "%i", (millis() - startTime) / 1000);
    UI::u8g.setFont(UI::FONT_LARGE_NUMERIC);
    UI::u8g.drawStr(x - UI::u8g.getStrWidth(c_t), y, c_t);

    UI::u8g.setFont(UI::FONT_REGULAR);
    UI::u8g.drawStr(x, y, "s");
}

unsigned long WeightCalibrationRun::getRunMillis() {
    return stopTime - startTime;
}
