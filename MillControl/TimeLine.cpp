#include "TimeLine.h"
#include "UI.h"
#include "TimeMode.h"

void TimeLine::drawTimeLine(char line, const int data, unsigned char y, unsigned char x, unsigned char grams,
                           const bool small, bool selected, bool editor) {
    char c_t[6] = "";

    //Symbol
    UI::u8g.setColorIndex(1);
    const bool weightLine = line == -1;
    if(small) {
        if(!weightLine)
            UI::u8g.drawDisc(x + 9, y - 6, 7);
        UI::u8g.setFont(UI::FONT_SMALL);
        x += 1;
    } else {
        if(!weightLine)
            UI::u8g.drawDisc(x + 9, y - 7, 9);
        UI::u8g.setFont(UI::FONT_REGULAR);
    }
    UI::u8g.setColorIndex(0);

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
    const unsigned char s_pos = x + (weightLine ? (small? 44 : 44) : 56);

    //Seconds

    UI::u8g.setColorIndex(1);
    unsigned char p = x + 28;
    if (line != -1 && data == TimeMode::SPECIAL_DATA) {
        UI::u8g.setFont(UI::FONT_SMALL);
        UI::u8g.drawStr(p, y-1, line == 2 ? SPECIAL_TIME_HOLD_NAME : SPECIAL_TIME_START_NAME);
    } else {
        if(!small)
            UI::u8g.setFont(UI::FONT_NUMERIC);
        if(weightLine){
            if(data < 1000)
                sprintf(c_t, "%i.%02i", data / 100, data % 100);
            else
                sprintf(c_t, "%i.%i", data / 100, data / 10 % 10);
        } else {
            if(data < 1000)
                sprintf(c_t, "%i.%i", data / 10, data % 10);
            else
                sprintf(c_t, "%i", data / 10);
        }
        p = s_pos - UI::u8g.getStrWidth(c_t);
        UI::u8g.drawStr(p, y, c_t);
        //s
        UI::u8g.setFont(UI::FONT_SMALL);
        UI::u8g.drawStr(s_pos, y, weightLine ? "s/g" : (grams ? "g" : "s"));
    }

    if(selected) {
#if defined(MILL_BUTTON) && !defined(PORTRAIT_DISPLAY)
        unsigned char d = 2;
#else
        unsigned char d = 4;
#endif
        if (editor)
            UI::u8g.drawHLine(p - 1, y + d, (line != -1 && data == TimeMode::SPECIAL_DATA ? (line == 3 ? 31 : 40) : s_pos - p));
        else
            UI::u8g.drawHLine(x, y + d, 64);
    }
}

