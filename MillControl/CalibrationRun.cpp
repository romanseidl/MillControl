//
// Created by roman on 03.12.15.
//

#include "CalibrationRun.h"
#include "State.h"

void CalibrationRun::draw() {
    State::draw();
#ifdef PORTRAIT_DISPLAY
    UI::u8g.setFont(UI::FONT_SMALL);
    const char* text = "Calibration";
    const char x = UI::DISPLAY_WIDTH -1;
    const char y = UI::DISPLAY_HEIGHT - UI::BORDER_WIDTH - 1;
    UI::u8g.drawStr270(x, y, text);
#else
    char x = 25;
    char y = 18;
    UI::u8g.setFont(UI::FONT_REGULAR);
    UI::u8g.drawStr(UI::LINE_HEIGHT * 2, UI::LINE_HEIGHT, "Calibration"); //25,18
#endif
}
