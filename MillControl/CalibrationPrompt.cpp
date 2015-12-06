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
    CalibrationRun::draw();
    char c_t[6] = "";
    if(deciGrams < 1000)
        sprintf(c_t, "%i.%i", deciGrams / 10, deciGrams % 10);
    else
        sprintf(c_t, "%i", deciGrams / 10);

#ifdef PORTRAIT_DISPLAY
    const char x=52;
    const char y=124;
    UI::u8g.setFont(UI::FONT_LARGE_NUMERIC);
#else
    const char x = 100;
    const char y = 55;
    UI::u8g.setFont(UI::FONT_LARGE_NUMERIC);
#endif

    const char w = UI::u8g.getStrWidth(c_t);
    UI::u8g.drawStr(x - w, y, c_t);
#ifdef PORTRAIT_DISPLAY
    UI::u8g.setFont(UI::FONT_REGULAR);
#else
    UI::u8g.setFont(UI::FONT_REGULAR);
#endif
    UI::u8g.drawStr(x, y, "g");
    UI::u8g.drawHLine(x - w, y + 3, w);
}