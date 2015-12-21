//
// Created by roman on 03.12.15.
//

#include "CalibrationPrompt.h"
#include "MillControl.h"

void CalibrationPrompt::start() {
    setEncoderMode(1900, deciGrams);
    redraw();
}

void CalibrationPrompt::encoderClick() {
    long centiSecondsPerGram = MillControl::WEIGHT_CALIBRATOR.getRunMillis() / deciGrams;
    if(centiSecondsPerGram > TimeMode::SPECIAL_DATA)
        centiSecondsPerGram = TimeMode::SPECIAL_DATA;
    MillControl::TIME_MODE_SELECTOR.getMode().centiSecondsPerGram = centiSecondsPerGram;
    MillControl::setState(MillControl::EDITOR);
}

void CalibrationPrompt::encoderChanged(int encoderPos) {
    deciGrams = encoderPos < 1000 ? encoderPos : 1000 + (encoderPos - 1000) * 10;
    redraw();
}

void CalibrationPrompt::draw() {
    State::draw();
    UI::drawSubtitle(UI::CALIBRATION_TITLE);

    char c_t[6] = "";
    if(deciGrams < 1000)
        sprintf(c_t, "%i.%i", deciGrams / 10, deciGrams % 10);
    else
        sprintf(c_t, "%i", deciGrams / 10);

#ifdef PORTRAIT_DISPLAY
    const char x=UI::DISPLAY_WIDTH - UI::SMALL_LINE_HEIGHT - UI::LINE_HEIGHT - UI::BORDER_WIDTH;
    const char y=UI::DISPLAY_HEIGHT - UI::BORDER_WIDTH - 1;
#else
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT -1; //100
    const char y = UI::DISPLAY_HEIGHT - 3 * UI::BORDER_WIDTH - 1;
#endif

    UI::u8g.setFont(UI::FONT_LARGE_NUMERIC);
    const char w = UI::u8g.getStrWidth(c_t);
    UI::u8g.drawStr(x - w, y, c_t);
#ifdef PORTRAIT_DISPLAY
    UI::u8g.setFont(UI::FONT_REGULAR);
#else
    UI::u8g.setFont(UI::FONT_REGULAR);
#endif
    UI::u8g.drawStr(x, y, "g");
    UI::u8g.drawHLine(x - w, y + UI::BORDER_WIDTH, w);
}
