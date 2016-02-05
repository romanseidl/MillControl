#include "CharEditor.h"
#include "MillControl.h"

//const char CharEditor::CHARACTERS[]= " ABCDEFGHIJLKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890/-+*&!?:#'()[]";
//const unsigned char CharEditor::CHARACTERS_COUNT = sizeof(CharEditor::CHARACTERS) - 1;

bool CharEditor::start() {
    c = MillControl::EDITOR.getChar();
    int encoderPosition = *c - UI::CHAR_BEGIN + 1;
    setEncoderMode(UI::CHAR_END - UI::CHAR_BEGIN + 1, encoderPosition);
    return true;
}

void CharEditor::encoderClick() {
    close();
}

void CharEditor::encoderChanged(int encoderPos) {
    (*c) = encoderPos + UI::CHAR_BEGIN;
    redraw();
}

void CharEditor::draw() {
    MillControl::EDITOR.drawEditor();
}
