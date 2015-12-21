#include "TimeLine.h"
#include "UI.h"
#include "TimeMode.h"

void TimeLine::drawDirectionSymbol(const unsigned char x ,const unsigned char y, const unsigned char height, const bool back = false, const unsigned char weight) {
  const char h = height - 2*UI::BORDER_WIDTH;
  const char w0 = back ? (h / 2) : 0;
  const char w1 = back ? 0 : (h / 2);
  const char w = h / 2;
  const char y0 = y - UI::BORDER_WIDTH;
  for (unsigned char l = 0; l < 2; l++){
      for (unsigned char c  = 0; c < UI::BORDER_WIDTH * weight; c++){
        const char x0 = x + c + l*(UI::BORDER_WIDTH * weight *2);
        UI::u8g.drawLine(x0 + w0, y0,     x0 + w1, y0 - w);
        UI::u8g.drawLine(x0 + w0, y0 - h, x0 + w1, y0 - w);
      }
    }  
}


void TimeLine::drawSymbol(const unsigned char x, const unsigned char y, const unsigned char symbol, const unsigned char line_height, const unsigned char border, const unsigned char weight) {
    UI::u8g.setColorIndex(1);

    const unsigned char r = (line_height / 2) + border * UI::BORDER_WIDTH;
    UI::u8g.drawDisc(x + r, y - (line_height/2), r);

    UI::u8g.setColorIndex(0);
    if(symbol < 2) {
        const char* s = symbol ? "2" : "1";
        UI::u8g.drawStr(x + r - (UI::u8g.getStrWidth(s) / 2), y, s);
        UI::u8g.drawVLine(x + r - (UI::u8g.getStrWidth(s) / 2), y - line_height, 50);
        UI::u8g.drawVLine(x + r + (UI::u8g.getStrWidth(s) / 2), y -  line_height, 50);
    } else {
        drawDirectionSymbol(x + r - (line_height / 2) - (3 * weight) / 2 ,y, line_height, false, weight);
    }
    UI::u8g.setColorIndex(1); 
}

void TimeLine::drawTimeLine(char line, const int data, unsigned char y, unsigned char x, unsigned char grams,
                           const bool small, bool selected, bool editor) {
    char c_t[6] = "";
    const bool weightLine = line == -1;

    if(small) 
        UI::u8g.setFont(UI::FONT_SMALL);
    else
        UI::u8g.setFont(UI::FONT_REGULAR);

    if(!weightLine) 
        drawSymbol(x, y, line, small ? UI::SMALL_LINE_HEIGHT : UI::LINE_HEIGHT);

#ifdef PORTRAIT_DISPLAY
    const unsigned char line_w = UI::DISPLAY_WIDTH;
#else
    const unsigned char line_w = UI::DISPLAY_HEIGHT;
#endif    

    //Seconds

    UI::u8g.setColorIndex(1);
    const bool special_line = line != -1 && data == TimeMode::SPECIAL_DATA;
    const char* text = special_line ? 
                         (line == 2 ? SPECIAL_TIME_HOLD_NAME : SPECIAL_TIME_START_NAME)
                       : (weightLine ? "s/g" : (grams ? "g" : "s"));
   

    //Maybe one shoul use consts on that?
    UI::u8g.setFont(UI::FONT_SMALL);
    const unsigned char text_w = UI::u8g.getStrWidth(text);
    const unsigned char text_x = special_line ? (x + 2 * UI::LINE_HEIGHT + 2 * UI::BORDER_WIDTH) : (x + line_w - text_w);
    UI::u8g.drawStr(text_x, y - (special_line ? 1 : 0), text);

    //draw time
    if(!special_line) {
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

        const unsigned char data_w = UI::u8g.getStrWidth(c_t);
        const unsigned char data_x = text_x - data_w;

        UI::u8g.drawStr(data_x, y, c_t);
        if (selected && editor)
            UI::u8g.drawHLine(data_x, y + UI::BORDER_WIDTH, data_w);
    } else {
      if(selected && editor)
        UI::u8g.drawHLine(text_x, y + UI::BORDER_WIDTH, text_w);
    }

    if(selected && ! editor) {
#if defined(MILL_BUTTON) && !defined(PORTRAIT_DISPLAY)
        const unsigned char d = UI::BORDER_WIDTH;
#else
        const unsigned char d = UI::BORDER_WIDTH * 2;
#endif
            UI::u8g.drawHLine(x, y + d, line_w);
    }
}

