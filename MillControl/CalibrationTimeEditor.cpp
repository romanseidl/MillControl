#include "TimeEditor.h"
#include "MillControl.h"
#include "CalibrationTimeEditor.h"

bool CalibrationTimeEditor::start() {
    time = MillControl::CALIBRATION_PROMPT.getTime();
    setEncoderMode(1901, *time < 1000 ?  *time : 1000 + (*time - 1000) / 10) ;
    return true;
}

void CalibrationTimeEditor::draw() {
    MillControl::CALIBRATION_PROMPT.drawEditor();
}

void CalibrationTimeEditor::encoderClick() {
    close();
}