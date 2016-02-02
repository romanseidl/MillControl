//
// Created by roman on 24.11.15.
//

#include "MultiModeSelector.h"
#include "MillControl.h"


bool MultiModeSelector::start() {
    setEncoderMode(timeModes.size, selectedMode);
    return ModeSelector::start();
}

void MultiModeSelector::encoderClick() {
    MillControl::open(MillControl::EDITOR);
}

void MultiModeSelector::millClick(unsigned char clickType) {
    if (getMode().getDeciSeconds(clickType)) {
        MillControl::RUN.setMode(clickType, getMode().getDeciSeconds(clickType), clickType == 2);
        MillControl::open(MillControl::RUN);
    }
}

void MultiModeSelector::encoderChanged(int encoderPos) {
    if (encoderPos != selectedMode) {
        selectedMode = encoderPos;
        eepromWrite();
        redraw();
    }
}

void MultiModeSelector::draw() {
    State::draw();
    UI::u8g.setFont(UI::FONT_SMALL);

#ifndef PORTRAIT_DISPLAY
    unsigned char lines = 0;
    for (unsigned char t = 0; t < Mode::DATA_PER_MODE; t++) {
        if(timeModes.timeModes[selectedMode].data[t] > 0)
                lines++;
    }
#endif

    unsigned char line = 0;
    for (unsigned char t = 0; t < Mode::DATA_PER_MODE; t++) {
        const Mode &mode = timeModes.timeModes[selectedMode];
        int time = mode.data[t];
        if (time > 0) {
#ifdef PORTRAIT_DISPLAY
            unsigned char x = 0;
            unsigned char y = UI::LINE_HEIGHT * 2 + ((UI::DISPLAY_HEIGHT - (UI::LINE_HEIGHT * 4)) * 3 / 8) +
                              (((UI::DISPLAY_HEIGHT - (UI::LINE_HEIGHT * 4)) * 2 / 8) + UI::LINE_HEIGHT) *
                              line;   //52 + line*32
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


