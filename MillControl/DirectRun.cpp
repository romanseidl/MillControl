//
// Created by roman on 05.02.16.
//

#include "DirectRun.h"

bool DirectRun::start() {
    startTime = millis();
    return Run::start();
}

void DirectRun::millClick(unsigned char i) {
    close();
}

void DirectRun::draw() {
    Run::draw();
    int seconds = max(0, ((millis() + 500) - startTime) / 1000);

#ifdef PORTRAIT_DISPLAY
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT;
    const char y = UI::DISPLAY_HEIGHT - 12 * UI::BORDER_WIDTH;
#else
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT; //115;
    const char y = UI::DISPLAY_HEIGHT - 6 * UI::BORDER_WIDTH; //53;
#endif
    UI::drawRunTime(x, y, seconds);
}
