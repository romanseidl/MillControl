#pragma once

//==================================================================================================
// Time Line

class TimeLine {
protected:
    static constexpr char *SPECIAL_TIME_START_NAME = "start";
    static constexpr char *SPECIAL_TIME_HOLD_NAME = "hold";

    static void drawTimeLine(char line, const int data, unsigned char y, unsigned char x, unsigned char grams,
                                const bool small, bool selected, bool editor);
    static void drawSymbol(unsigned char x, unsigned char y, unsigned char symbol, unsigned char line_height, const unsigned char border = 2, const unsigned char weight = 1);

    static void drawDirectionSymbol(const unsigned char x ,const unsigned char y, const unsigned char height, const bool back, const unsigned char weight = 1);
};

