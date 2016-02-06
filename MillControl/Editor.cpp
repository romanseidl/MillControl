#include "MillControl.h"

bool Editor::start() {
    //get the current mode
    timeMode = &MillControl::TIME_MODE_SELECTOR.getMode();
    //get all time modes - needed for adding and deleting
    timeModes = &MillControl::TIME_MODE_SELECTOR.timeModes;

    firstChar = (unsigned char) (Mode::DATA_PER_MODE + (timeMode->mode == Mode::WEIGHT_MODE ? 1 : 0));

    const int size = timeModes->size;

    back = firstChar++;

    if (!deleteMode)
        gram = firstChar++;
    else
        gram = 0;

    if (size < ModeList::MAX_MODES && !deleteMode)
        add = firstChar++;
    else
        add = 0;

    del = firstChar++;

    if (size > 2 && !deleteMode) {
        left = firstChar++;
        right = firstChar++;
    } else {
        left = 0;
        right = 0;
    }

    if (deleteMode)
        setEncoderMode(2, 0);
    else
        setEncoderMode(firstChar + Mode::MAX_CHARS, position);

    DEBUG_PRINTLN("editor set up");
    //if there is no mill button i need a multi click button in the gram editor
#ifndef MILL_BUTTON
    //Multi click if not in timed mode (for calibration menus)
    UI::encoderButton.setMultiClick(timeMode->mode != Mode::TIME_MODE);
#endif
    return true;
}

void Editor::stop() {
}

int *Editor::getData() {
    if (timeMode->mode == Mode::WEIGHT_MODE) {
        if (position == 0)
            return &timeMode->calibration_data;
        else
            return &timeMode->data[position - 1];
    }
    else
        return &timeMode->data[position];
}

char *Editor::getChar() {
    return &timeMode->name[position - firstChar];
}


void Editor::encoderClick() {
    if (position < back) {
        MillControl::open(MillControl::TIME_EDITOR);
        return;
    } else if (position == back) {
        if (deleteMode) {
            deleteMode = false;
        } else {
            timeModes->eepromWrite();
            close();
            return;
        }
        position = 0;
    } else if (position == gram) {
        unsigned char new_mode;
        switch (timeMode->mode) {
            case Mode::TIME_MODE:
                new_mode = Mode::WEIGHT_MODE;
                break;
#ifdef SCALE
            case Mode::WEIGHT_MODE:
                new_mode = Mode::SCALE_MODE;
                break;
#endif
            default:
                new_mode = Mode::TIME_MODE;
        }
        timeMode->setMode(new_mode);
        position = 0;
    } else if (position == add) {
        MillControl::TIME_MODE_SELECTOR.setMode(&timeModes->insertAfer(*timeMode));
        position = 0;
    } else if (position == left) {
        MillControl::TIME_MODE_SELECTOR.setMode(&timeModes->moveLeft(*timeMode));
        position = back;
    } else if (position == right) {
        MillControl::TIME_MODE_SELECTOR.setMode(&timeModes->moveRight(*timeMode));
        position = back;
    } else if (position == del) {
        if (deleteMode) {
            MillControl::TIME_MODE_SELECTOR.setMode(&timeModes->del(*timeMode));
            position = 0;
            deleteMode = false;
            close();
            return;
        } else {
            position = back;
            deleteMode = true;
        }
    } else if (position >= firstChar) {
        MillControl::open(MillControl::CHAR_EDITOR);
        return;
    }
    start();
    redraw();
}

//In Weight Mode rotator click starts the calibration mode
void Editor::millClick(unsigned char i) {
    if (timeMode->mode == Mode::WEIGHT_MODE)
        MillControl::open(MillControl::WEIGHT_CALIBRATOR);
#ifdef SCALE
    else if (timeMode->mode == Mode::SCALE_MODE)
        MillControl::open(MillControl::SCALE_CALIBRATOR);
#endif
#ifndef MILL_BUTTON
    else
        encoderClick();
#endif
}


void Editor::encoderChanged(int encoderPos) {
    position = (deleteMode ? back : 0) + encoderPos;
    redraw();
}


void Editor::draw() {
    draw(false);
}

void Editor::drawEditor() {
    draw(true);
}

void Editor::draw(bool editor) {
    State::draw();
    unsigned char weightMode = timeMode->mode == Mode::WEIGHT_MODE ? 1 : 0;
    for (char t = -weightMode; t < Mode::DATA_PER_MODE; t++) {

        const char pos = t + weightMode;

#ifdef PORTRAIT_DISPLAY
        unsigned char x = 0;
#ifdef MILL_BUTTON
        const bool small = false;
        unsigned char y = weightMode ? (UI::LINE_HEIGHT + UI::SMALL_LINE_HEIGHT + ((UI::DISPLAY_HEIGHT - UI::LINE_HEIGHT - (UI::SMALL_LINE_HEIGHT * 5)) * 2 / 10) + (((UI::DISPLAY_HEIGHT - UI::LINE_HEIGHT - (UI::SMALL_LINE_HEIGHT * 5)) * 2 / 10) + UI::SMALL_LINE_HEIGHT) * pos)
                                     : (UI::LINE_HEIGHT * 2  + ((UI::DISPLAY_HEIGHT - UI::SMALL_LINE_HEIGHT - (UI::LINE_HEIGHT * 5)) * 3 / 8) + (((UI::DISPLAY_HEIGHT - (UI::LINE_HEIGHT * 5)) * 2 / 8) + UI::LINE_HEIGHT) * pos );   //52 + line*32#endif
#else
        const bool small = false;
        unsigned char y = UI::LINE_HEIGHT *2  + ((UI::DISPLAY_HEIGHT - (UI::LINE_HEIGHT * 5)) * 3 / 8) + (((UI::DISPLAY_HEIGHT - (UI::LINE_HEIGHT * 5)) * 2 / 8) + UI::LINE_HEIGHT) * pos;   //52 + line*32#endif
#endif
#else
        unsigned char x = UI::LINE_HEIGHT * 2;
#ifdef MILL_BUTTON
        const bool small = weightMode;
        const unsigned char y = weightMode ? (((UI::DISPLAY_HEIGHT - (UI::SMALL_LINE_HEIGHT * 4)) / 8) +
                                              UI::SMALL_LINE_HEIGHT +
                                              (((UI::DISPLAY_HEIGHT - (UI::SMALL_LINE_HEIGHT * 4)) / 4) +
                                               UI::SMALL_LINE_HEIGHT) * pos) :
                                (((UI::DISPLAY_HEIGHT - (UI::LINE_HEIGHT * 3)) / 6) + UI::LINE_HEIGHT +
                                 (((UI::DISPLAY_HEIGHT - (UI::LINE_HEIGHT * 3)) / 3) + UI::LINE_HEIGHT) *
                                 pos); //small ? (13 + pos * 16) : (16 + t * 22);
#else
        const bool small = false;
        const unsigned char y = weightMode ? (((UI::DISPLAY_HEIGHT - (UI::LINE_HEIGHT * 3)) / 6) + UI::LINE_HEIGHT + (((UI::DISPLAY_HEIGHT- (UI::LINE_HEIGHT *3)) / 3) + UI::LINE_HEIGHT ) * pos) :
                                             (((UI::DISPLAY_HEIGHT - (UI::LINE_HEIGHT * 2)) / 4) + UI::LINE_HEIGHT + (((UI::DISPLAY_HEIGHT- (UI::LINE_HEIGHT *2)) / 2) + UI::LINE_HEIGHT ) * pos); // y = weightMode ? (15 + pos * 22) : (23 + pos * 32);
#endif
#endif
        const int data = (t == -1) ? timeMode->calibration_data : timeMode->data[t];

        UI::drawTimeLine(t, data, y, x, timeMode->mode != Mode::TIME_MODE, small, position == pos, editor);
    }

    UI::u8g.setFont(UI::FONT_SMALL);

    drawEditPoint(0, back, UI::BACK_SYMBOL);

    if (gram)
        drawEditPoint(1, gram, weightMode ? 'g' : (timeMode->mode == Mode::SCALE_MODE ? UI::SCALE_SYMBOL : 's'));

    if (add)
        drawEditPoint(2, add, UI::ADD_STRING);

    if (del)
        drawEditPoint(3, del, UI::DEL_STRING);

    if (left)
        drawEditPoint(4, left, UI::MOVE_LEFT_STRING);

    if (right)
        drawEditPoint(5, right, UI::MOVE_RIGHT_STRING);

    if (position >= firstChar) {
        UI::u8g.setFont(UI::FONT_REGULAR);
        char charBuf[Mode::MAX_CHARS + 1];

        const int i = position - firstChar;
        memcpy(charBuf, &timeMode->name, i + 1);
        charBuf[i + 1] = 0;

        unsigned char w = UI::u8g.getStrWidth(&charBuf[i]);

        charBuf[i] = 0;
        unsigned char x = UI::u8g.getStrWidth(charBuf);

#ifdef PORTRAIT_DISPLAY
        UI::u8g.drawHLine(x, UI::LINE_HEIGHT + UI::BORDER_WIDTH, w);
        if (editor)
            UI::u8g.drawHLine(x, UI::LINE_HEIGHT + UI::BORDER_WIDTH + 2, w);
#else
        UI::u8g.drawVLine(UI::LINE_HEIGHT + UI::BORDER_WIDTH, UI::DISPLAY_HEIGHT - x - w, w);
        if (editor)
            UI::u8g.drawVLine(UI::LINE_HEIGHT + UI::BORDER_WIDTH + 2, UI::DISPLAY_HEIGHT - x - w, w);
#endif
    }
}

void Editor::drawEditPoint(const unsigned char p, const unsigned char pos, const unsigned char symbol) const {
    UI::drawEditPoint(p, pos == position, symbol);
};
