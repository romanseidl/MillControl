/**
 * Partly based on raron's (raronzen@gmail.com) clickbutton library.
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

#include <Arduino.h>
#include "Button.h"

Button::Button(unsigned char pin, bool multiClick) : pin(pin), multiClick(multiClick) {
    //Set pin to input
    pinMode(pin, INPUT_PULLUP);

    MultiTimer1::add(this);
}

//Update clicks
void Button::run() {
    long now = (long) millis();         // get current time
    bool btnState = !digitalRead(pin);  // current appearant button state

    // If the switch changed, due to noise or a button press, reset the debounce timer
    if (btnState != lastState) lastBounceTime = now;

    // debounce the button (Check if a stable, changed state has occured)
    const long delta = now - lastBounceTime;

    if (btnState != stableState && delta > DEBOUNCE_TIME) {
        stableState = btnState;
        //if the button was pressed then this is one more click
        if (stableState)
            clickCount++;
    }

    if (multiClick) {
        // If the button is released and the pause was long enough then report the number of clicks and restart
        if (!stableState && delta > CLICK_TIME_MULTI) {
            // positive count for released buttons
            clicks = clickCount;
            clickCount = 0;
        }

        // Check for "long click"
        if (stableState && (delta > CLICK_TIME_LONG)) {
            // negative count for long clicks
            clicks = -clickCount;
            clickCount = 0;
        }

    } else if (clickCount) {
        //immedeately propagate the click
        clicks = clickCount;
        clickCount = 0;
    }

    lastState = btnState;
}

//Pulls clicks - resets the counter
char Button::getClicks() {
    const char c = clicks;
    clicks = 0;
    return c;
}

//Sets the button to allow multiple clicks and long clicks
void Button::setMultiClick(bool _multiClick) {
    multiClick = _multiClick;
}

// Is the button pressed?
bool Button::isPressed() {
    return stableState;
}
