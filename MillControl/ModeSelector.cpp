//
// Created by roman on 06.01.16.
//

#include <avr/eeprom.h>
#include "MultiModeSelector.h"
#include "MillControl.h"

EEMEM unsigned char eeprom_time_mode;

Mode &ModeSelector::getMode() {
    return timeModes.timeModes[selectedMode];
}

ModeSelector::ModeSelector() {
    selectedMode = eeprom_read_byte(&eeprom_time_mode);
    if (selectedMode > (timeModes.size - 1)) {
        selectedMode = 0;
    }
}

bool ModeSelector::start() {
    //If there is only one button ensure it is multi-click
#ifndef MILL_BUTTON
    UI::encoderButton.setMultiClick(true);
#endif
    updateTime = millis();
    return true;
}

void ModeSelector::setMode(Mode *mode) {
    selectedMode = timeModes.pos(*mode);
    eepromWrite();
    updateTime = millis();
    redraw();
}

#ifdef BREW_BUTTON

void ModeSelector::loop() {
    //re-show the bre timer after a timeout if its running
    if (millis() - updateTime > UI::BREW_TIMER_TIMEOUT && MillControl::BREW_TIMER.isRunning()) {
        MillControl::start(MillControl::BREW_TIMER);
    }
}

#endif

/* write settings to EEPROM */
void ModeSelector::eepromWrite() {
    eeprom_write_byte(&eeprom_time_mode, (uint8_t) selectedMode);
}
