#include "TimeEditor.h"
#include "MillControl.h"
bool TimeEditor::start() {
    time = MillControl::EDITOR.getTime();
    setEncoderMode(1901, *time < 1000 ?  *time : 1000 + (*time - 1000) / 10) ;
    return true;
}

void TimeEditor::encoderClick() {
    close();
}

void TimeEditor::encoderChanged(int encoderPos) {
    (*time) = encoderPos < 1000 ? encoderPos : 1000 + (encoderPos - 1000) * 10;
    redraw();
}

void TimeEditor::draw() {
    MillControl::EDITOR.drawEditor();
}
