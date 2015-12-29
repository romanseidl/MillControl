#include "CharEditor.h"
#include "MillControl.h"

const char CharEditor::CHARACTERS[]= " ABCDEFGHIJLKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890/-+*&!?:#'()[]";
const unsigned char CharEditor::CHARACTERS_COUNT = sizeof(CharEditor::CHARACTERS) - 1;

bool CharEditor::start() {
    c = MillControl::EDITOR.getChar();

    int encoderPosition = 0;
    while (CHARACTERS[encoderPosition] != (*c) && encoderPosition < CHARACTERS_COUNT)
        encoderPosition++;

    setEncoderMode(CHARACTERS_COUNT, encoderPosition);
    return true;
}

void CharEditor::encoderClick() {
    close();
}

void CharEditor::encoderChanged(int encoderPos) {
    (*c) = CHARACTERS[encoderPos];
    redraw();
}

void CharEditor::draw() {
    MillControl::EDITOR.drawEditor();
}
