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
    //If there is only one button ensure it is multi-click
#ifndef MILL_BUTTON
    UI::encoderButton.setMultiClickButton();
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
    if(getMode().getDeciSeconds(clickType)){
      MillControl::RUN.setMode(clickType);
      MillControl::setState(MillControl::RUN);
    }
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
    State::draw();
    UI::u8g.setFont(UI::FONT_SMALL);

#ifndef PORTRAIT_DISPLAY
    unsigned char lines = 0;
    for (unsigned char t = 0; t < TimeMode::DATA_PER_MODE; t++)
        if(timeModes.get(selectedMode).data[t] > 0)
            lines++;
#endif

    unsigned char line = 0;
    for (unsigned char t = 0; t < TimeMode::DATA_PER_MODE; t++) {
        const TimeMode &mode = timeModes.get(selectedMode);
        int time = mode.data[t];
        if (time > 0) {
#ifdef PORTRAIT_DISPLAY
            unsigned char x = 0;
            unsigned char y = UI::LINE_HEIGHT *2  + ((UI::DISPLAY_HEIGHT - (UI::LINE_HEIGHT * 4)) * 3 / 8) + (((UI::DISPLAY_HEIGHT - (UI::LINE_HEIGHT * 4)) * 2 / 8) + UI::LINE_HEIGHT) * line;   //52 + line*32
#else
            unsigned char x = (UI::DISPLAY_WIDTH - UI::DISPLAY_HEIGHT - UI::LINE_HEIGHT) / 2 + UI::LINE_HEIGHT;
            unsigned char y = lines > 2 ? ((UI::DISPLAY_HEIGHT- (UI::LINE_HEIGHT * 3)) / 6) + UI::LINE_HEIGHT + (((UI::DISPLAY_HEIGHT- (UI::LINE_HEIGHT *3)) / 3) + UI::LINE_HEIGHT ) * line :
                                          ((UI::DISPLAY_HEIGHT- (UI::LINE_HEIGHT * 2)) / 4) + UI::LINE_HEIGHT + (((UI::DISPLAY_HEIGHT- (UI::LINE_HEIGHT *2)) / 2) + UI::LINE_HEIGHT ) * line; //lines > 2 ? 16 + line * 22 : 23 + line * 32;  //16 + line * 22 : 23 + line * 32  - 
#endif
            UI::drawTimeLine(t, time, y, x, mode.weightMode, false, false, false);
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
}
