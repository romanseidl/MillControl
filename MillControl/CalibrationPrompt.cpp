//
// Created by roman on 03.12.15.
//

#include "CalibrationPrompt.h"
#include "MillControl.h"

bool CalibrationPrompt::open() {
    position = 0;
    MillControl::open(MillControl::CALIBRATION_TIME_EDITOR);
    return true;
}

bool CalibrationPrompt::start() {
    setEncoderMode(3, 1);
    return true;
}

void CalibrationPrompt::encoderClick() {
    switch(position){
        case 0:
            MillControl::open(MillControl::CALIBRATION_TIME_EDITOR);
            break;
        case 1:
            {
                long centiSecondsPerGram = MillControl::WEIGHT_CALIBRATOR.getRunMillis() / deciGrams;
                if (centiSecondsPerGram > Mode::SPECIAL_DATA)
                    centiSecondsPerGram = Mode::SPECIAL_DATA;
                MillControl::TIME_MODE_SELECTOR.getMode().centiSecondsPerGram = centiSecondsPerGram;
                close();
            }
            break;
        case 2:
            close();
            break;
    }
}

void CalibrationPrompt::encoderChanged(int encoderPos) {
    //deciGrams = encoderPos < 1000 ? encoderPos : 1000 + (encoderPos - 1000) * 10;
    position = encoderPos;
    redraw();
}

void CalibrationPrompt::draw(bool editor) {
    State::draw();
    UI::drawSubtitle(UI::CALIBRATION_TITLE);

    {
#ifdef PORTRAIT_DISPLAY
        const char x = UI::DISPLAY_WIDTH - UI::SMALL_LINE_HEIGHT - UI::LINE_HEIGHT - UI::BORDER_WIDTH;
        const char y = UI::DISPLAY_HEIGHT - 2 * UI::SMALL_LINE_HEIGHT - UI::LARGE_LINE_HEIGHT - UI::SMALL_LINE_HEIGHT;
#else
        const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT - UI::SMALL_LINE_HEIGHT - 3 * UI::BORDER_WIDTH -1; //100
        const char y = (UI::DISPLAY_HEIGHT - 3 * UI::BORDER_WIDTH - UI::LINE_HEIGHT - 2 * UI::LARGE_LINE_HEIGHT) / 2 + UI::LARGE_LINE_HEIGHT + UI::LINE_HEIGHT - 1;
#endif
        UI::drawRunTime(x, y, (MillControl::WEIGHT_CALIBRATOR.getRunMillis() / 1000));
    }

    char c_t[6] = "";
    if(deciGrams < 1000)
        sprintf(c_t, "%i.%i", deciGrams / 10, deciGrams % 10);
    else
        sprintf(c_t, "%i", deciGrams / 10);

#ifdef PORTRAIT_DISPLAY
    const char x=UI::DISPLAY_WIDTH - UI::SMALL_LINE_HEIGHT - UI::LINE_HEIGHT - UI::BORDER_WIDTH;
    const char y=UI::DISPLAY_HEIGHT - 2 * UI::SMALL_LINE_HEIGHT - 1;
    const char s = 0;
#else
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT - UI::SMALL_LINE_HEIGHT - 3 * UI::BORDER_WIDTH -1; //100
    const char y = UI::DISPLAY_HEIGHT - 3 * UI::BORDER_WIDTH - 1;
    const char s = 4;
#endif

    UI::u8g.setFont(UI::FONT_LARGE_NUMERIC);
    const char w = UI::u8g.getStrWidth(c_t);
    UI::u8g.drawStr(x - w, y, c_t);

    UI::u8g.setFont(UI::FONT_REGULAR);
    UI::u8g.drawStr(x, y, "g");

    if(position == 0) {
        if(editor)
            UI::u8g.drawHLine(x - w, y + UI::BORDER_WIDTH, w);
        else
            UI::u8g.drawHLine(x - w, y + UI::BORDER_WIDTH, w + UI::SMALL_LINE_HEIGHT);
    }

    UI::u8g.setFont(UI::FONT_SMALL);
    UI::drawEditPoint(s, position == 1, NULL);
    UI::drawEditPoint(s+1, position == 2, "x");
}

void CalibrationPrompt::drawEditor() {
    draw(true);
}

void CalibrationPrompt::draw() {
    draw(false);
}

int *CalibrationPrompt::getTime() {
    return &deciGrams;
}

