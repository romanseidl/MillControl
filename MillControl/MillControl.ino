/**
 * Mill Control
 * Timer Control for Coffee Mills
 *
 * Runs on Arduino Pro Mini & similar systems (needs 20k of storage space).
 *
 * Needs an encoder with a switch and an optional second switch.
 * If you only have one switch comment out the define in UI.h
 *
 * Output is to a 128*64 graphics display, comment out the orientation
 * define in UI.h
 *
 * Dependencies:
 * - ClickButton - https://code.google.com/p/clickbutton/
 * - Encoder (v1.2) - http://www.pjrc.com/teensy/td_libs_Encoder.html
 * - TimerOne (v1.1) - https://github.com/PaulStoffregen/TimerOne
 * - U8Glib - https://github.com/olikraus/u8glib
 *
 * (c) 2015 by Roman Seidl
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#include "MillControl.h"

//=================================================
// Encoder - has two pins
// only change pins if necessary, 2&3 have hardware interrupts and are thus faster!

//Pins - change as you like
#define ENCODER_BUTTON_PIN 4
#define MILL_BUTTON_PIN 5
#define MILL_BUTTON2_PIN 6

//The relay Pin is seht in Run.h

//If you don't have a mill button go to UI.h and comment out the define!

//This will change the button settings - do not change
#ifdef MILL_BUTTON
    #ifdef MILL_BUTTON_2
        Button UI::millButton(MILL_BUTTON_PIN, Button::MULTI_CLICK);
        Button UI::millButton2(MILL_BUTTON2_PIN, Button::MULTI_CLICK);
        Button UI::encoderButton(ENCODER_BUTTON_PIN, Button::SINGLE_CLICK);
    #else
        Button UI::millButton(MILL_BUTTON_PIN, Button::MULTI_CLICK);
        Button UI::millButton2 = UI::millButton;
        Button UI::encoderButton(ENCODER_BUTTON_PIN, Button::SINGLE_CLICK);
    #endif
#else
    Button UI::millButton(ENCODER_BUTTON_PIN, Button::MULTI_CLICK);
    Button UI::millButton2 = UI::millButton;
    Button UI::encoderButton = UI::millButton;
#endif

//=================================================
// Encoder - has two pins
// only change pins if necessary, 2&3 have hardware interrupts and are thus faster!

RotatingEncoder UI::encoder(2, 3);

//=================================================
// Pause time
// when stopping a timedrun by pressing the button the mill will wait for
// the given amount of seconds to allow for a restart
const int UI::PAUSE_TIME = 5;

//=================================================
// Display
// You can either connect a SPI or a I2C display. SPI is mcuh faster but may be a bit annoying to find th right pinout

// i2c
//U8GLIB UI::u8g = *new U8GLIB_SSD1306_128X64_2X(U8G_I2C_OPT_NO_ACK);

//SPI
//These are sensible defaults for Arduino Mini pro. If you use different Hardware and use Hardware SPI you might need to adapt them
#define OLED_SCK 13  // also called: CLK, SCL
#define OLED_MOSI 11 // also called: SDA, SID
#define OLED_MISO 9  // also called: a0, D/C,  DC, RS
#define OLED_RESET 8 // also called: RES, RST
#define OLED_CS 10   // also called: SS, ST

//Software SPI
//U8GLIB UI::u8g = *new U8GLIB_SSD1306_128X64_2X(OLED_SCK, OLED_MOSI, OLED_CS, OLED_MISO, OLED_RESET);

//Hardware SPI
U8GLIB UI::u8g = *new U8GLIB_SSD1306_128X64_2X(OLED_CS, OLED_MISO, OLED_RESET);


#ifdef PORTRAIT_DISPLAY
    const unsigned char UI::DISPLAY_WIDTH = 64;
    const unsigned char UI::DISPLAY_HEIGHT = 128;
#else
    const unsigned char UI::DISPLAY_WIDTH = 128;
    const unsigned char UI::DISPLAY_HEIGHT = 64;
#endif
//=================================================
// Fonts

const u8g_fntpgm_uint8_t* UI::FONT_SMALL{u8g_font_helvB10};
const u8g_fntpgm_uint8_t* UI::FONT_NUMERIC{u8g_font_helvR14n};
//const u8g_fntpgm_uint8_t* UI::FONT_NUMERIC{u8g_font_helvB14};
const u8g_fntpgm_uint8_t* UI::FONT_REGULAR{u8g_font_helvB14};
const u8g_fntpgm_uint8_t* UI::FONT_LARGE_NUMERIC{u8g_font_fub20n};
const char UI::SYMBOL_LONG[] = {187, 0};

void setup(void) {
#ifdef PORTRAIT_DISPLAY
    //Rotate the screen
    UI::u8g.setRot90();
#endif
    MillControl::setup();
}

void loop() {
    MillControl::loop();
}
