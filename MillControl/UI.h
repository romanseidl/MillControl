#ifndef MILLCONTROL_UI_H
#define MILLCONTROL_UI_H

#include "RotatingEncoder.h"
#include "RotatingButtons.h"
#include "Button.h"
#include "Scale.h"
#include <U8glib.h>

#include "helvB10s.h"
#include "helvB14r.h"

//=================================================
// DEBUG
// If uncommented this will print debug messages to the serial port

//#define DEBUG
#ifdef DEBUG
#include <HardwareSerial.h>
#define DEBUG_PRINTLN(message)  {Serial.println(message);Serial.flush();}
#define DEBUG_PRINT(message)  {Serial.print(message);Serial.flush();}
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
#define MILL_BUTTON_2 6

// Comment out this line if there is no brew button
// Change the Pin if you want it to be somewhere else
//#define BREW_BUTTON A0


//=================================================
// SCREEN ORIENTATION

// Uncomment this to change the orientation to portrait mode
#define PORTRAIT_DISPLAY

//=================================================
// FLAT MODE
// In flat mode only one time per page is shown in the time mode selector.
//#define FLAT_MODE

//=================================================
// RESET MODE
// Set this and flash once to reset data - don't forget to comment it out again!
// You can usually also reset all data by deteling all pages

//#define RESET_MODE


//=================================================
// SCALE MODE

//#define SCALE

//=================================================
// SYMBOLS
// If you want to use symbols for run mode and flat mode - symbols are defined in UI.cpp
//#define MODE_SYMBOLS
#ifdef FLAT_MODE
#define MODE_SYMBOLS
#endif

class Symbol {
    const u8g_pgm_uint8_t *bits;
    const unsigned char multiple;
public:
    Symbol(const unsigned char width, const unsigned char height, const u8g_pgm_uint8_t *bits,
           unsigned char multiple = 1)
            : width(width), height(height), bits(bits), multiple(multiple) { }

    void draw(U8GLIB *_display, const unsigned char x, const unsigned char y);

    const unsigned char width;
    const unsigned char height;
};

class UI {
protected:
#ifdef MODE_SYMBOLS
    static Symbol SYMBOLS[];
#endif
public:
    //=================================================
    // Titles - change these titles if you would like to have different titles - be aware that size matters...
    static constexpr char *SPECIAL_TIME_START_NAME PROGMEM = (char *const) "start";
    static constexpr char *SPECIAL_TIME_HOLD_NAME PROGMEM = (char *const) "hold";

    static constexpr char *BREW_TITLE PROGMEM = (char *const) "Brewing";
    static constexpr char *CALIBRATION_TITLE PROGMEM = (char *const) "Calibration";
    static constexpr char *TARE_TITLE PROGMEM = (char *const) "Tare";

    static const unsigned char DEL_STRING        PROGMEM = 'x';
    static const unsigned char ADD_STRING        PROGMEM = '+';
    static const unsigned char MOVE_LEFT_STRING  PROGMEM = '<';
    static const unsigned char MOVE_RIGHT_STRING PROGMEM = '>';
    //=================================================
    // Relay Pin - change the pin if you want it to be somewhere else
    static const unsigned char RELAY_PIN = 7;

    static const unsigned char PAUSE_TIME;

#ifdef BREW_BUTTON
    static const long BREW_TIMER_TIMEOUT = 1000;
#endif

    static const u8g_fntpgm_uint8_t *FONT_SMALL;
    static const u8g_fntpgm_uint8_t *FONT_NUMERIC;
    static const u8g_fntpgm_uint8_t *FONT_REGULAR;
    static const u8g_fntpgm_uint8_t *FONT_LARGE_NUMERIC;

    static Button encoderButton;
#ifdef MILL_BUTTON
    static Button millButton;
#endif
#ifdef MILL_BUTTON_2
    static Button millButton2;
#endif
#ifdef BREW_BUTTON
    static Button brewButton;
    static Symbol BREW_SYMBOL;
#endif
    static Rotator *rotator;

#ifdef SCALE
    static Scale scale;
#endif

    //Display object - defined below
    static U8GLIB u8g;

    //Display & Font settings
    static const unsigned char DISPLAY_WIDTH;
    static const unsigned char DISPLAY_HEIGHT;
    static const unsigned char BORDER_WIDTH;
    static const unsigned char LINE_HEIGHT;
    static const unsigned char SMALL_LINE_HEIGHT;
    static const unsigned char LARGE_LINE_HEIGHT;

    //Menu Symbols
    static const unsigned char SCALE_SYMBOL = SYMBOL_SCALE;
    static const unsigned char BACK_SYMBOL = SYMBOL_BACK;
    static const unsigned char OK_SYMBOL = SYMBOL_OK;

    //Menu Symbols
    static const unsigned char CHAR_BEGIN = REGULAR_BEGIN;
    static const unsigned char CHAR_END = REGULAR_END;

    //Draw Routines
    static void drawSubtitle(const char *text);

    static void drawTitle(const char *title);

    static void drawDirectionSymbol(const unsigned char x, const unsigned char y, const unsigned char height,
                                    const bool back, const unsigned char weight = 1);

    static void drawSymbol(unsigned char x, unsigned char y, unsigned char symbol, unsigned char line_height,
                           const unsigned char border = 2, const unsigned char weight = 1);

    static void drawLargeSymbol(const unsigned char x, const unsigned char y, const unsigned char symbol);

    static void drawTimeLine(char line, const int data, unsigned char y, unsigned char x, bool grams, const bool small,
                             bool selected, bool editor);

    static void drawRunTime(const unsigned char x, const unsigned char y, int seconds, bool small = false);

    static void drawRunWeight(const unsigned char x, const unsigned char y, int weight, bool small = false,
                              unsigned char line = 0);

    static void drawEditPoint(const unsigned char p, const bool active, const unsigned char symbol);

    static void printData(char *c_t, const int data, const bool weightLine = false);

    static void drawProgressBar(const double progress);

    static void drawRunWeightLine(long weight, const unsigned char line);
};

#endif //MILLCONTROL_UI_H