#pragma once

#include "RotatingEncoder.h"
#include "Button.h"
#include "State.h"

#include <U8glib.h>

//=================================================
// DEBUG
// If uncommented this will print debug messages to the serial port

//#define DEBUG
#ifdef DEBUG
    #include <HardwareSerial.h>
    #define DEBUG_PRINTLN(message)  (Serial.println(message))
    #define DEBUG_PRINT(message)  (Serial.print(message))
#else
    #define DEBUG_PRINTLN(message)
    #define DEBUG_PRINT(message)
#endif

//=================================================
// BUTTONS

// Comment out this line if there is no mill button
// Change the Pin if you want it to be somewhere else
#define MILL_BUTTON 5

// Comment out this line if there is no second mill button
// Change the Pin if you want it to be somewhere else
//#define MILL_BUTTON_2 6

// Comment out this line if there is no brew button
// Change the Pin if you want it to be somewhere else
//#define BREW_BUTTON A0


//=================================================
// SCREEN ORIENTATION

// Uncomment this to change the orientation to portrait mode
#define PORTRAIT_DISPLAY


class UI {
protected:
    //=================================================
    // Titles - change these titles if you would like to have different titles - be aware that size matters...
    static constexpr char *SPECIAL_TIME_START_NAME = "start";
    static constexpr char *SPECIAL_TIME_HOLD_NAME = "hold";
public:
    static constexpr char *BREW_TITLE = "Brewing";
    static constexpr char *CALIBRATION_TITLE = "Calibration";

    static constexpr char *DEL_STRING = "x";
    static constexpr char *ADD_STRING = "+";
    static constexpr char *MOVE_LEFT_STRING = "<";
    static constexpr char *MOVE_RIGHT_STRING = ">";
    //=================================================
    // Relay Pin - change the pin if you want it to be somewhere else
    static const unsigned char RELAY_PIN = 7;

    static const unsigned char PAUSE_TIME;
    static const long BREW_TIMER_TIMEOUT;

    static const u8g_fntpgm_uint8_t* FONT_SMALL;
    static const u8g_fntpgm_uint8_t* FONT_NUMERIC;
    static const u8g_fntpgm_uint8_t* FONT_REGULAR;
    static const u8g_fntpgm_uint8_t* FONT_LARGE_NUMERIC;

    static Button encoderButton;
#ifdef MILL_BUTTON
    static Button millButton;
#endif
#ifdef MILL_BUTTON_2
    static Button millButton2;
#endif
#ifdef BREW_BUTTON
    static Button brewButton;
#endif
    static RotatingEncoder encoder;

    //Display object - defined below
    static U8GLIB u8g;

    //Display & Font settings
    static const unsigned char DISPLAY_WIDTH;
    static const unsigned char DISPLAY_HEIGHT;
    static const unsigned char BORDER_WIDTH;    
    static const unsigned char LINE_HEIGHT;
    static const unsigned char SMALL_LINE_HEIGHT;
    static const unsigned char LARGE_LINE_HEIGHT;

    //Draw Routines
    static void drawSubtitle(const char *text);
    static void drawTitle(const char *title);
    static void drawDirectionSymbol(const unsigned char x ,const unsigned char y, const unsigned char height, const bool back, const unsigned char weight = 1);
    static void drawSymbol(unsigned char x, unsigned char y, unsigned char symbol, unsigned char line_height, const unsigned char border = 2, const unsigned char weight = 1);
    static void drawTimeLine(char line, const int data, unsigned char y, unsigned char x, unsigned char grams,
                             const bool small, bool selected, bool editor);
    static void drawRunTime(const unsigned char x, const unsigned char y, int seconds);

    static void drawEditPoint(const unsigned char p, const bool active, const char *symbol);

};



