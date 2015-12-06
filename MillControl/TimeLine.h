#pragma once

//==================================================================================================
// Time Line

class TimeLine {
protected:
    static constexpr char *SPECIAL_TIME_START_NAME = "start";
    static constexpr char *SPECIAL_TIME_HOLD_NAME = "hold";

    static void drawTimeLine(char line, const int data, unsigned char y, unsigned char x, unsigned char grams,
                                const bool small, bool selected, bool editor);
};

