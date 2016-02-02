#pragma once

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

#include "Button.h"
#include "UI.h"

Button::Button(unsigned char _pin, bool _multiClick) {
    pin = _pin;
    multiClick = _multiClick;

    //Set pin to input
    pinMode(pin, INPUT);
    // Turn on internal pullup resistor
    digitalWrite(pin, HIGH);

    MultiTimer1::add(this);
}

//Update clicks
void Button::run() {
    long now = (long) millis();      // get current time
    bool btnState = !digitalRead(pin);  // current appearant button state

    // If the switch changed, due to noise or a button press, reset the debounce timer
    if (btnState != lastState) lastBounceTime = now;

    // debounce the button (Check if a stable, changed state has occured)
    if ((now - lastBounceTime) > DEBOUNCE_TIME && btnState != stableState) {
        stableState = btnState;
        //if the button was pressed then this is one more click
        if (stableState)
            clickCount++;
    }

    if (multiClick) {
        // If the button is released and the pause was long enough then report the number of clicks and restart
        if (!stableState && (now - lastBounceTime) > CLICK_TIME_MULTI) {
            // positive count for released buttons
            clicks = clickCount;
            clickCount = 0;
        }

        // Check for "long click"
        if (stableState && (now - lastBounceTime > CLICK_TIME_LONG)) {
            // negative count for long clicks
            clicks = 0 - clickCount;
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
    char c = clicks;
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
