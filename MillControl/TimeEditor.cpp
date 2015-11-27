#include "TimeEditor.h"
#include "MillControl.h"

void TimeEditor::start() {
    time = MillControl::EDITOR.getTime();
    setEncoderMode(1000, *time);
    redraw();
}

void TimeEditor::encoderClick() {
    MillControl::setState(MillControl::EDITOR);
}

void TimeEditor::encoderChanged(int encoderPos) {
    (*time) = encoderPos;
    redraw();
}

void TimeEditor::draw() {
    MillControl::EDITOR.drawEditor();
}
