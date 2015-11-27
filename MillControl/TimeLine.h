#pragma once

//==================================================================================================
// Time Line

class TimeLine {
protected:
    static constexpr char *SPECIAL_TIME_START_NAME = "start";
    static constexpr char *SPECIAL_TIME_HOLD_NAME = "hold";

    static void drawTimeLine(unsigned char line, int time, unsigned char y, unsigned char x = 0);
};

