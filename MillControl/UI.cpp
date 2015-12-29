//
// Created by roman on 21.12.15.
//

#include "UI.h"
#include "TimeMode.h"

void UI::drawSubtitle(const char* text) {
#ifdef PORTRAIT_DISPLAY
    UI::u8g.setFont(UI::FONT_SMALL);
    const char x = UI::DISPLAY_WIDTH -1;
    const char y = UI::DISPLAY_HEIGHT - UI::BORDER_WIDTH - 1;
    UI::u8g.drawStr270(x, y, text);
#else
    u8g.setFont(FONT_REGULAR);
    u8g.drawStr(LINE_HEIGHT * 2, LINE_HEIGHT, text);
#endif
}

void UI::drawTitle(const char* title) {
    u8g.setFont(FONT_REGULAR);
#ifdef PORTRAIT_DISPLAY
    UI::u8g.drawStr(0, UI::LINE_HEIGHT, title);
#else
    u8g.drawStr270(LINE_HEIGHT, DISPLAY_HEIGHT - 1, title);
#endif

#ifdef DEBUG
    //Memory display
    UI::u8g.setFont(u8g_font_u8glib_4);
    char mem_str[5];

    extern int __heap_start, *__brkval;
    int v;
    const int mem = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);

    sprintf(mem_str, "%i", mem);
    UI::u8g.drawStr(0, 20, mem_str);
#endif
}

void UI::drawTimeLine(char line, const int data, unsigned char y, unsigned char x, unsigned char grams,
                      const bool small, bool selected, bool editor) {
    char c_t[6] = "";
    const bool weightLine = line == -1;

    if(small)
        u8g.setFont(FONT_SMALL);
    else
        u8g.setFont(FONT_REGULAR);

    if(!weightLine)
        drawSymbol(x, y, line, small ? SMALL_LINE_HEIGHT : LINE_HEIGHT);

#ifdef PORTRAIT_DISPLAY
    const unsigned char line_w = UI::DISPLAY_WIDTH;
#else
    const unsigned char line_w = DISPLAY_HEIGHT;
#endif

    //Seconds

    u8g.setColorIndex(1);
    const bool special_line = line != -1 && data == TimeMode::SPECIAL_DATA;
    const char* text = special_line ?
                         (line == 2 ? SPECIAL_TIME_HOLD_NAME : SPECIAL_TIME_START_NAME)
                       : (weightLine ? "s/g" : (grams ? "g" : "s"));


    //Maybe one shoul use consts on that?
    u8g.setFont(FONT_SMALL);
    const unsigned char text_w = u8g.getStrWidth(text);
    const unsigned char text_x = special_line ? (x + 2 * LINE_HEIGHT + 2 * BORDER_WIDTH) : (x + line_w - text_w);
    u8g.drawStr(text_x, y - (special_line ? 1 : 0), text);

    //draw time
    if(!special_line) {
        if(!small)
            u8g.setFont(FONT_NUMERIC);

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

        const unsigned char data_w = u8g.getStrWidth(c_t);
        const unsigned char data_x = text_x - data_w;

        u8g.drawStr(data_x, y, c_t);
        if (selected && editor)
            u8g.drawHLine(data_x, y + BORDER_WIDTH, data_w);
    } else {
      if(selected && editor)
        u8g.drawHLine(text_x, y + BORDER_WIDTH, text_w);
    }

    if(selected && ! editor) {
#if defined(MILL_BUTTON) && !defined(PORTRAIT_DISPLAY)
        const unsigned char d = BORDER_WIDTH;
#else
        const unsigned char d = UI::BORDER_WIDTH * 2;
#endif
            u8g.drawHLine(x, y + d, line_w);
    }
}

void UI::drawDirectionSymbol(const unsigned char x , const unsigned char y, const unsigned char height, const bool back = false, const unsigned char weight) {
  const char h = height - 2 * BORDER_WIDTH;
  const char w0 = back ? (h / 2) : 0;
  const char w1 = back ? 0 : (h / 2);
  const char w = h / 2;
  const char x0 = x - h/4 - weight * BORDER_WIDTH;
  const char y0 = y - BORDER_WIDTH;
  for (unsigned char l = 0; l < 2; l++){
      for (unsigned char c  = 0; c < BORDER_WIDTH * weight; c++){
        const char x1 = x0 + c + l*(BORDER_WIDTH * weight * 2);
        u8g.drawLine(x1 + w0, y0,     x1 + w1, y0 - w);
        u8g.drawLine(x1 + w0, y0 - h, x1 + w1, y0 - w);
      }
    }
}

void UI::drawSymbol(const unsigned char x, const unsigned char y, const unsigned char symbol, const unsigned char line_height, const unsigned char border, const unsigned char weight) {
    u8g.setColorIndex(1);

    const unsigned char r = (line_height / 2) + border;
    u8g.drawDisc(x + r, y - (line_height/2), r);

    u8g.setColorIndex(0);
    if(symbol < 2) {
        const char* s = symbol ? "2" : "1";
        u8g.drawStr(x + r - (u8g.getStrWidth(s) / 2) + (line_height < 18 ? 1 : 0), y, s);
    } else {
        drawDirectionSymbol(x + r, y , line_height, false, weight);
    }
    u8g.setColorIndex(1);
}

void UI::drawRunTime(const unsigned char x, const unsigned char y, int seconds){
    char c_t[6] = "";
    sprintf(c_t, "%i", seconds);
    u8g.setFont(FONT_LARGE_NUMERIC);
    u8g.drawStr(x - u8g.getStrWidth(c_t), y, c_t);

    u8g.setFont(FONT_REGULAR);
    u8g.drawStr(x, y, "s");
}

void UI::drawEditPoint(const unsigned char p, const bool active, const char *symbol){
//    UI::u8g.setFont(UI::FONT_SMALL);
#ifdef PORTRAIT_DISPLAY
    const int x = (DISPLAY_WIDTH / 6) * p + (p > 0 ? ((DISPLAY_WIDTH / 6) - u8g.getStrWidth(UI::MOVE_RIGHT_STRING)) : 0);
    const int y = DISPLAY_HEIGHT - 2 * BORDER_WIDTH;
#else
    const int x = UI::DISPLAY_WIDTH - (p == 0 ? (2 * UI::SMALL_LINE_HEIGHT + UI::BORDER_WIDTH * 4) :
                                                    (UI::SMALL_LINE_HEIGHT + UI::BORDER_WIDTH * 2));
    const int y = p > 1 ? ( (p-1) * (UI::DISPLAY_HEIGHT - 2 * UI::SMALL_LINE_HEIGHT - 2 * UI::BORDER_WIDTH -1) / 3 + UI::SMALL_LINE_HEIGHT)  : UI::SMALL_LINE_HEIGHT;
#endif

    if(symbol)
      u8g.drawStr(x, y, symbol);
    else
      drawDirectionSymbol(x+ SMALL_LINE_HEIGHT / 2 - UI::BORDER_WIDTH, y, SMALL_LINE_HEIGHT, true);

    if (active) {
      unsigned char w = symbol ? u8g.getStrWidth(symbol) : SMALL_LINE_HEIGHT;
      u8g.drawHLine(x, y + 1, w);
    }
}