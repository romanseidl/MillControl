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
 * - Encoder (v1.4) - http://www.pjrc.com/teensy/td_libs_Encoder.html
 * - TimerOne (v1.1) - https://github.com/PaulStoffregen/TimerOne
 * - U8Glib - https://github.com/olikraus/u8glib
 *
 * (c) 2015 by Roman Seidl
 * The bean and hand symbols (c) by @mamu - www.kaffee-netz.de
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
#include <TimerOne.h>

#include "helvB14r.h"
#include "helvB10s.h"
#include "helvR14n.h"
#include "fub20bn.h"

//=================================================
// ENCODER - has two pins
// only change pins if necessary, som pins have hardware interrupts and are thus faster
// on most platforms do NOT use pin 13 (led pin)

// Pins with HW-interrupts for different boards (s. http://www.pjrc.com/teensy/td_libs_Encoder.html)
// Teensy 3.0	All Digital Pins
// Teensy 2.0	5, 6, 7, 8	11
// Teensy 1.0	0, 1, 2, 3, 4, 6, 7, 16
// Teensy++ 2.0	0, 1, 2, 3, 18, 19, 36, 37	6
// Teensy++ 1.0	0, 1, 2, 3, 18, 19, 36, 37
// Arduino Due	All Digital Pins
// Arduino Uno	2, 3
// Arduino Leonardo	0, 1, 2, 3
// Arduino Mega	2, 3, 18, 19, 20, 21
// Sanguino	2, 10, 11

//Default for Ardiuno Pro Micro and Uno
Rotator *UI::rotator = new RotatingEncoder(2, 3);

//Default for Ardiuno Pro Micro and Uno when using spi (they are on pin 2 & 3)
//Rotator* UI::rotator = new RotatingEncoder(0, 1);

//Button Encoder - up and down button
//Rotator *UI::rotator = new RotatingButtons(2, 3);


#define ENCODER_BUTTON 4

//=================================================
// BUTTONS

//Buttons are in UI.h

//=================================================
// SCALE
// to activate the scale uncomment the SCALE define in UI.h
// Setup is scale(data_pin, clock_pin, power_pin)

#ifdef SCALE
Scale UI::scale(A1, A2, A3);
#endif

//=================================================
// Pause time
// when stopping a timedrun by pressing the button the mill will wait for
// the given amount of seconds to allow for a restart
const unsigned char UI::PAUSE_TIME = 5;

//=================================================
// Brew timer Timeout
// when in the main selector (time mode selector) and the brew timer is active
// after the given amount of milliseconds until the brew timer is shown again if it was interrupted
#ifdef BREW_TIMER
const long UI::BREW_TIMER_TIMEOUT = 3000;
#endif

//=================================================
// Display

// Orientation of the display can be changed in UI.h

// SPI - Bus (only applies if you use a SPI Display
// These are sensible defaults for Arduino Mini pro. If you use different Hardware and use Hardware SPI you might need to adapt them
// Somehow HArdware SPI with MISO on pin 12 was not working even though this is the default according to the arduino.cc.
// SPI is hell because there is only a very weak norm on how the connectors are called. Producers seem keep inventing new names...
#define OLED_RESET 8 // also called: RES, RST
#define OLED_CS 10   // also called: SS, ST, CE
#define OLED_MOSI 11 // also called: SDA, SID, Din
#define OLED_MISO 9  // also called: a0, D/C,  DC, RS
#define OLED_SCK 13  // also called: CLK, SCL, SCLK

// Pro Micro settings:
/*#define OLED_RESET 8 // also called: RES, RST
#define OLED_CS 10   // also called: SS, ST, CE
#define OLED_MOSI 16 // also called: SDA, SID, Din
#define OLED_MISO 9  // also called: a0, D/C,  DC, RS
#define OLED_SCK 15  // also called: CLK, SCL, SCLK
*/
//=================================================
// SSD 1306 Display

// Uncomment this if you are using the SSD1306
#define DISPLAY_128x64

// You can either connect a SPI or a I2C display. SPI is much faster but may be a bit annoying to find th right pinout

// i2c
//U8GLIB UI::u8g = *new U8GLIB_SSD1306_128X64_2X(U8G_I2C_OPT_NO_ACK);

// Software SPI
//U8GLIB UI::u8g = *new U8GLIB_SSD1306_128X64_2X(OLED_SCK, OLED_MOSI, OLED_CS, OLED_MISO, OLED_RESET);

// Hardware SPI (recommended, if not working try software spi)
U8GLIB UI::u8g = *new U8GLIB_SSD1306_128X64_2X(OLED_CS, OLED_MISO, OLED_RESET);

//=================================================
// PCD 8544 Display

// Uncomment this if you are using the PCD 8544
//U8GLIB UI::u8g = U8GLIB_PCD8544(OLED_SCK, OLED_MOSI, OLED_CS, OLED_MISO, OLED_RESET);

#ifdef DISPLAY_128x64
  #ifdef PORTRAIT_DISPLAY
      const unsigned char UI::DISPLAY_WIDTH = 64;
      const unsigned char UI::DISPLAY_HEIGHT = 128;
  #else
      const unsigned char UI::DISPLAY_WIDTH = 128;
      const unsigned char UI::DISPLAY_HEIGHT = 64;
  #endif
const u8g_fntpgm_uint8_t *UI::FONT_SMALL{helvB10s};
const u8g_fntpgm_uint8_t *UI::FONT_NUMERIC{helvR14n};
const u8g_fntpgm_uint8_t *UI::FONT_REGULAR{helvB14r};
//const u8g_fntpgm_uint8_t* UI::FONT_REGULAR{helvB10s};
const u8g_fntpgm_uint8_t *UI::FONT_LARGE_NUMERIC{fub20bn};
//const u8g_fntpgm_uint8_t* UI::FONT_LARGE_NUMERIC{helvR14n};
  const unsigned char UI::BORDER_WIDTH = 2;

  const unsigned char UI::LINE_HEIGHT = 14;
  const unsigned char UI::SMALL_LINE_HEIGHT = 10;
  const unsigned char UI::LARGE_LINE_HEIGHT = 20;
#else
  #ifdef PORTRAIT_DISPLAY
      const unsigned char UI::DISPLAY_WIDTH = 48;
      const unsigned char UI::DISPLAY_HEIGHT = 84;
  #else
      const unsigned char UI::DISPLAY_WIDTH = 84;
      const unsigned char UI::DISPLAY_HEIGHT = 48;
  #endif
  const u8g_fntpgm_uint8_t* UI::FONT_SMALL{u8g_font_6x12r};
  const u8g_fntpgm_uint8_t* UI::FONT_NUMERIC{u8g_font_helvR08n};
  const u8g_fntpgm_uint8_t* UI::FONT_REGULAR{u8g_font_helvB08r};
  const u8g_fntpgm_uint8_t* UI::FONT_LARGE_NUMERIC{u8g_font_helvB14n};
  const unsigned char UI::LINE_HEIGHT = 8;
  const unsigned char UI::SMALL_LINE_HEIGHT = 6;
  const unsigned char UI::LARGE_LINE_HEIGHT = 14;
  const unsigned char UI::BORDER_WIDTH = 1;
#endif

//This will change the button settings - do not change
Button UI::encoderButton(ENCODER_BUTTON, Button::SINGLE_CLICK);
#ifdef MILL_BUTTON
    Button UI::millButton(MILL_BUTTON, Button::MULTI_CLICK);
    #ifdef MILL_BUTTON_2
        Button UI::millButton2(MILL_BUTTON_2, Button::MULTI_CLICK);
    #endif
#endif

#ifdef BREW_BUTTON
    Button UI::brewButton(BREW_BUTTON, Button::SINGLE_CLICK);
#endif

void setup() {
#ifdef PORTRAIT_DISPLAY
    //Rotate the screen
    UI::u8g.setRot90();
#endif

#ifdef DEBUG
    Serial.begin(9600);
#endif
    MultiTimer1::start();
    MillControl::setup();
    DEBUG_PRINTLN("Setup completed");
}

void loop() {
    //DEBUG_PRINT(".");
    MillControl::loop();
}

