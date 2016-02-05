#include "DataEditor.h"

bool DataEditor::start() {
    time = editor->getData();
    setEncoderMode(1901, *time < 1000 ?  *time : 1000 + (*time - 1000) / 10) ;
    return true;
}

void DataEditor::encoderClick() {
    close();
}

void DataEditor::encoderChanged(int encoderPos) {
    (*time) = encoderPos < 1000 ? encoderPos : 1000 + (encoderPos - 1000) * 10;
    redraw();
}

void DataEditor::draw() {
    editor->drawEditor();
}
