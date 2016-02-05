//
// Created by roman on 24.11.15.
//

#include <avr/eeprom.h>
#include "FlatModeSelector.h"
#include "MillControl.h"
#include "TimedRun.h"
#include "HoldRun.h"

#ifdef SCALE
#include "WeightRun.h"
#include "ScaleTarePrompt.h"
#endif

EEMEM unsigned char eeprom_time;

FlatModeSelector::FlatModeSelector() {
    selectedTime = eeprom_read_byte(&eeprom_time);
    if (selectedTime > 1) {
        selectedTime = 0;
    }
}

bool FlatModeSelector::start() {
    const int modes = findTimePos(timeModes.size - 1, Mode::DATA_PER_MODE);
    const int mode = findTimePos(selectedMode, selectedTime);
    setEncoderMode(modes, mode);

    return ModeSelector::start();
}

int FlatModeSelector::findTimePos(const int mode, const int time) {
    int modes = 0;
    for (unsigned char m = 0; m <= mode; m++) {
        for (unsigned char t = 0; t < (m == mode ? time : Mode::DATA_PER_MODE); t++) {
            if (timeModes.timeModes[m].data[t] != 0)
                modes++;
        }
    }
    return modes;
}


void FlatModeSelector::encoderClick() {
    MillControl::open(MillControl::EDITOR);
}

void FlatModeSelector::millClick(unsigned char clickType) {
    unsigned char run_index = (clickType == MillControl::DOUBLE_CLICK) ? MillControl::DOUBLE_CLICK : selectedTime;
    Mode &mode = getMode();
    const int data = mode.getDeciSeconds(run_index);

    if (data) {
        Run *run;

        if (data == Mode::SPECIAL_DATA) {
            if (clickType == MillControl::LONG_CLICK)
                run = new HoldRun(MillControl::LONG_CLICK, data);
            else
                run = new DirectRun(MillControl::LONG_CLICK, data);
        } else {
#ifdef SCALE
            if (mode.mode == Mode::SCALE_MODE)
                run = new WeightRun(run_index, data, &mode);
            else
#endif
            run = new TimedRun(run_index, data);
        }

        MillControl::open(*run);
    }
}

void FlatModeSelector::encoderChanged(int encoderPos) {
    int mode = 0;
    for (unsigned char m = 0; m < timeModes.size; m++) {
        for (unsigned char t = 0; t < Mode::DATA_PER_MODE; t++) {
            if (timeModes.timeModes[m].data[t] != 0) {
                if (mode == encoderPos) {
                    //Found!
                    selectedMode = m;
                    selectedTime = t;
                    redraw();
                    eepromWriteTime();
                    return;
                }
                mode++;
            }
        }
    }
}

void FlatModeSelector::draw() {
    State::draw();

    const Mode &mode = timeModes.timeModes[selectedMode];
    int time = mode.data[selectedTime];

    //Symbol - same as in run
    char c_t[5] = "";
#ifdef PORTRAIT_DISPLAY
    int x = 0;
    int y = UI::LINE_HEIGHT + (UI::DISPLAY_HEIGHT / 3) + (UI::LARGE_LINE_HEIGHT / 2);
#else
    int x = UI::LINE_HEIGHT * 2 + (UI::LARGE_LINE_HEIGHT / 2) + UI::BORDER_WIDTH * 2; //42
    int y = UI::LARGE_LINE_HEIGHT + UI::BORDER_WIDTH * 2 + UI::BORDER_WIDTH * 5; //29
#endif

    UI::u8g.setFont(UI::FONT_LARGE_NUMERIC);
    UI::drawLargeSymbol(x, y, (time == Mode::SPECIAL_DATA ? 2 : selectedTime));


#ifdef PORTRAIT_DISPLAY
    x = UI::DISPLAY_WIDTH - UI::SMALL_LINE_HEIGHT;
    y = UI::LINE_HEIGHT + (2 * UI::DISPLAY_HEIGHT / 3) + (UI::LINE_HEIGHT / 2);
#else
    x = UI::DISPLAY_WIDTH - UI::SMALL_LINE_HEIGHT; //115;
    y = UI::DISPLAY_HEIGHT - 6 * UI::BORDER_WIDTH; //53;
#endif
    if (time == Mode::SPECIAL_DATA) {
        UI::u8g.setFont(UI::FONT_REGULAR);
        char *const text = UI::SPECIAL_TIME_START_NAME;
        UI::u8g.drawStr(UI::DISPLAY_WIDTH - UI::u8g.getStrWidth(text), y, text);
    } else {
        UI::u8g.setFont(UI::FONT_NUMERIC);

        UI::printData(c_t, time);
        UI::u8g.drawStr(x - UI::u8g.getStrWidth(c_t), y, c_t);

        UI::u8g.setFont(UI::FONT_SMALL);
        UI::u8g.drawStr(x, y, mode.isWeightMode() ? "g" : "s");
    }

}

/* write settings to EEPROM */
void FlatModeSelector::eepromWriteTime() {
    eeprom_write_byte(&eeprom_time, selectedTime);
    eepromWrite();
}