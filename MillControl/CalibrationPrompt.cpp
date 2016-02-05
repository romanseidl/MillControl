#include "CalibrationPrompt.h"
#include "MillControl.h"

CalibrationPrompt::CalibrationPrompt() : Prompt(2, 3) {
#ifndef PORTRAIT_DISPLAY
    prompt_position = 2;
#endif
}

bool CalibrationPrompt::open() {
    position = 2;
    MillControl::open(MillControl::CALIBRATION_TIME_EDITOR);
    return true;
}

void CalibrationPrompt::encoderClick() {
    Prompt::encoderClick();

    if (position == 2)
        MillControl::open(MillControl::CALIBRATION_TIME_EDITOR);
}

void CalibrationPrompt::draw() {
    Prompt::draw();
    draw(false);
}


void CalibrationPrompt::draw(bool editor) {
    Prompt::draw();

    UI::drawSubtitle(UI::CALIBRATION_TITLE);
    UI::drawRunWeightLine(weight, position == 2 ? (editor ? 1 : 2) : 0);
}

void CalibrationPrompt::drawEditor() {
    draw(true);
}

int *CalibrationPrompt::getData() {
    return &weight;
}

void CalibrationPrompt::cancel() {
    weight = 0;
    Prompt::cancel();
}
