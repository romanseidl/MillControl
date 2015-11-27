//
// Created by roman on 24.11.15.
//

#include <avr/eeprom.h>
#include "TimeModeSelector.h"
#include "MillControl.h"

EEMEM unsigned char eeprom_time_mode;

TimeModeSelector::TimeModeSelector() {
    selectedMode = eeprom_read_byte(&eeprom_time_mode);
    if (selectedMode > (timeModes.size() - 1)) {
        selectedMode = 0;
    }
}

void TimeModeSelector::start() {
#ifndef MILL_BUTTON
    UI::millButton.setMultiClickButton();
#endif
    setEncoderMode(timeModes.size(), selectedMode);
    redraw();
}

TimeMode &TimeModeSelector::getMode() {
    return timeModes.get(selectedMode);
}

void TimeModeSelector::setMode(TimeMode *mode) {
    setEncoderPos(timeModes.pos(*mode));
    redraw();
}

void TimeModeSelector::encoderClick() {
    MillControl::setState(MillControl::EDITOR);
}

void TimeModeSelector::millClick(unsigned char clickType) {
    MillControl::RUN.setMode(clickType);
    MillControl::setState(MillControl::RUN);
}

void TimeModeSelector::encoderChanged(int encoderPos) {
    if (encoderPos != selectedMode) {
        setEncoderPos(encoderPos);
        redraw();
    }
}

void TimeModeSelector::setEncoderPos(int encoderPos) {
    selectedMode = encoderPos;
    eepromWrite();
}

void TimeModeSelector::draw() {
    TitledState::draw();
    UI::u8g.setFont(UI::FONT_SMALL);

#ifndef PORTRAIT_DISPLAY
    unsigned char lines = 0;
    for (unsigned char t = 0; t < TimeMode::TIMES_PER_MODE; t++)
        if(timeModes.get(selectedMode).time[t] > 0)
            lines++;
#endif

    unsigned char line = 0;
    for (unsigned char t = 0; t < TimeMode::TIMES_PER_MODE; t++) {
        int time = timeModes.get(selectedMode).time[t];
        if (time > 0) {
#ifdef PORTRAIT_DISPLAY
            unsigned char x = 0;
            unsigned char y = 52 + line * 32;
#else
            unsigned char x = 43;
            unsigned char y = lines > 2 ? 16 + line * 22 : 23 + line * 32;
#endif
            drawTimeLine(t, time, y, x);
            line++;
        }
    }
}


int TimeModeSelector::size() {
    return timeModes.size();
}

TimeModeList &TimeModeSelector::getTimeModes() {
    return timeModes;
}

/* write settings to EEPROM */
void TimeModeSelector::eepromWrite() {
    eeprom_write_byte(&eeprom_time_mode, selectedMode);
}

void TimeModeSelector::stop() {
#ifndef MILL_BUTTON
    UI::millButton.setSingleClickButton();
#endif
}
