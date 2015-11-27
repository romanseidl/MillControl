#pragma once

#include "RotatingEncoder.h"
#include "Button.h"
#include <U8glib.h>

class UI {
public:
    static const u8g_fntpgm_uint8_t* FONT_SMALL;
    static const u8g_fntpgm_uint8_t* FONT_REGULAR;
    static const u8g_fntpgm_uint8_t* FONT_BOLD;
    static const u8g_fntpgm_uint8_t* FONT_LARGE;

    static Button encoderButton;
    static Button millButton;
    static RotatingEncoder encoder;

    static const unsigned char DISPLAY_WIDTH;
    static const unsigned char DISPLAY_HEIGHT;
    static U8GLIB u8g;
    static const char SYMBOL_LONG[2];

    static const int PAUSE_TIME;
};

//=================================================
// Buttons

//Comment out this line if there is no mill button
#define MILL_BUTTON

//uncomment this to make the display lanscape
#define PORTRAIT_DISPLAY
