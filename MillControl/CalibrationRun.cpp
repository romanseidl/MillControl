//
// Created by roman on 03.12.15.
//

#include "CalibrationRun.h"
#include "State.h"

void CalibrationRun::draw() {
    State::draw();
#ifdef PORTRAIT_DISPLAY
    const char x = 63;
    const char y = 99;
    UI::u8g.setFont(UI::FONT_SMALL);
    UI::u8g.drawStr270(x, y, "Calibration");
#else
    char x = 25;
    char y = 18;
    UI::u8g.setFont(UI::FONT_REGULAR);
    UI::u8g.drawStr(25, 18, "Calibration");
#endif
}
