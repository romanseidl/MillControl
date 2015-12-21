#pragma once

#include "RotatingEncoder.h"
#include "Button.h"
#include <U8glib.h>

class UI {
public:
    static const u8g_fntpgm_uint8_t* FONT_SMALL;
    static const u8g_fntpgm_uint8_t* FONT_NUMERIC;
    static const u8g_fntpgm_uint8_t* FONT_REGULAR;
    static const u8g_fntpgm_uint8_t* FONT_LARGE_NUMERIC;

    static Button encoderButton;
    static Button millButton;
    static Button millButton2;
    static RotatingEncoder encoder;

    static const unsigned char DISPLAY_WIDTH;
    static const unsigned char DISPLAY_HEIGHT;
    static const unsigned char BORDER_WIDTH;    
    static const unsigned char LINE_HEIGHT;
    static const unsigned char SMALL_LINE_HEIGHT;
    static const unsigned char LARGE_LINE_HEIGHT;


    static U8GLIB u8g;
    static const char SYMBOL_LONG[2];

    static const int PAUSE_TIME;
};

//=================================================
// Buttons

//Comment out this line if there is no mill button
#define MILL_BUTTON

//Comment out this line if there is no second mill button
//#define MILL_BUTTON_2

//uncomment this to make the display lanscape
//#define PORTRAIT_DISPLAY


//Debugging only - will display a free memory info
//#define MEMORY_DISPLAY
