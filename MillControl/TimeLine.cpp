#include "TimeLine.h"
#include "UI.h"
#include "TimeMode.h"

void TimeLine::drawTimeLine(unsigned char line, int time, unsigned char y, unsigned char x) {
    char c_t[4] = "";

    //Symbol
    UI::u8g.setColorIndex(1);
    UI::u8g.drawDisc(x + 9, y - 7, 9);

    UI::u8g.setColorIndex(0);
    UI::u8g.setFont(UI::FONT_BOLD);

    switch (line) {
        case 0:
            UI::u8g.drawStr(x + 4, y, "1");
            break;
        case 1:
            UI::u8g.drawStr(x + 5, y, "2");
            break;
        case 2:
            UI::u8g.drawStr(x + 4, y - 1, UI::SYMBOL_LONG);
            break;
    }
    const unsigned char s_pos = x + 54;

    //Seconds
    UI::u8g.setFont(UI::FONT_REGULAR);
    UI::u8g.setColorIndex(1);
    if (time == TimeMode::SPECIAL_TIME) {
        UI::u8g.drawStr(x + 25, y, line == 2 ? SPECIAL_TIME_HOLD_NAME : SPECIAL_TIME_START_NAME);
    } else {
        sprintf(c_t, "%i", time);
        unsigned char p = s_pos - UI::u8g.getStrWidth(c_t);
        UI::u8g.drawStr(p, y, c_t);
        //s
        UI::u8g.setFont(UI::FONT_SMALL);
        UI::u8g.drawStr(s_pos, y, "s");
    }
}

