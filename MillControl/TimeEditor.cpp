#include "TimeEditor.h"
#include "MillControl.h"

void TimeEditor::start() {
    time = MillControl::EDITOR.getTime();
    setEncoderMode(1901, *time < 1000 ?  *time : 1000 + (*time - 1000) / 10) ;
    redraw();
}

void TimeEditor::encoderClick() {
    MillControl::setState(MillControl::EDITOR);
}

void TimeEditor::encoderChanged(int encoderPos) {
    (*time) = encoderPos < 1000 ? encoderPos : 1000 + (encoderPos - 1000) * 10;
    redraw();
}

void TimeEditor::draw() {
    MillControl::EDITOR.drawEditor();
}
