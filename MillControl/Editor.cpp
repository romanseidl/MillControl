#include "MillControl.h"

const char Editor::DEL_STRING[2] = "x";
const char Editor::ADD_STRING[2] = "+";
const char Editor::MOVE_LEFT_STRING[2] = "<";
const char Editor::MOVE_RIGHT_STRING[2] = ">";

void Editor::start() {
    timeMode = &MillControl::TIME_MODE_SELECTOR.getMode();
    timeModes = &MillControl::TIME_MODE_SELECTOR.getTimeModes();

    firstChar = TimeMode::DATA_PER_MODE + (timeMode->weightMode ? 1 : 0);

    const int size = MillControl::TIME_MODE_SELECTOR.size();

    back = firstChar++;

    if (!deleteMode)
        gram = firstChar++;
    else
        gram = 0;

    if (size < TimeModeList::MAX_MODES && !deleteMode)
        add = firstChar++;
    else
        add = 0;

    if (size > 1)
        del = firstChar++;
    else
        del = 0;


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
        setEncoderMode(firstChar + TimeMode::MAX_CHARS, position);

    //if there is not mill button i need a multi click button in the gram editor
#ifndef MILL_BUTTON
    if(timeMode->weightMode)
        UI::millButton.setMultiClickButton();
    else
        UI::millButton.setSingleClickButton();
#endif
    redraw();
}

void Editor::stop() {
}

int *Editor::getTime() {
    if (timeMode->weightMode) if (position == 0)
        return &timeMode->centiSecondsPerGram;
    else
        return &timeMode->data[position - 1];
    else
        return &timeMode->data[position];
}

char *Editor::getChar() {
    return &timeMode->name[position - firstChar];
}


void Editor::encoderClick() {
    if (position < back) {
        MillControl::setState(MillControl::TIME_EDITOR);
    } else if (position == back) {
        if (deleteMode) {
            deleteMode = false;
            start();
        } else {
            timeModes->eepromWrite();
            MillControl::setState(MillControl::TIME_MODE_SELECTOR);
        }
        position = 0;
    } else if (position == gram) {
        timeMode->setWeightMode(!timeMode->weightMode);
        position = 0;
        start();
    } else if (position == add) {
        MillControl::TIME_MODE_SELECTOR.setMode(&timeModes->insertAfer(*timeMode));
        position = 0;
        start();
    } else if (position == left) {
        MillControl::TIME_MODE_SELECTOR.setMode(&timeModes->moveLeft(*timeMode));
        position = back;
        start();
    } else if (position == right) {
        MillControl::TIME_MODE_SELECTOR.setMode(&timeModes->moveRight(*timeMode));
        position = back;
        start();
    } else if (position == del) {
        if (deleteMode) {
            MillControl::TIME_MODE_SELECTOR.setMode(&timeModes->del(*timeMode));
            position = 0;
            deleteMode = false;
            start();
        } else {
            deleteMode = true;
            start();
        }
    } else if (position >= firstChar) {
        MillControl::setState(MillControl::CHAR_EDITOR);
    }
}

//In Weight Mode encoder click starts the calibration mode
void Editor::millClick(unsigned char i) {
    if(timeMode->weightMode)
        MillControl::setState(MillControl::WEIGHT_CALIBRATOR);
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
    unsigned char weightMode = timeMode->weightMode ? 1 : 0;
    for (char t = -weightMode; t < TimeMode::DATA_PER_MODE; t++) {
        const char pos = t + weightMode;
#ifdef PORTRAIT_DISPLAY
        unsigned char x = 0;
#ifdef MILL_BUTTON
        const bool small = false;
        unsigned char y = weightMode ? (39 + pos * 22) : (45 + pos * 28);
#else
        const bool small = false;
        unsigned char y = 45 + pos * 28;
#endif
#else
        unsigned char x = 25;
#ifdef MILL_BUTTON
        const bool small = weightMode;
        unsigned char y = small ? (13 + pos * 16) : (16 + t * 22);
#else
        const bool small = false;
        unsigned char y = weightMode ? (15 + pos * 22) : (23 + pos * 32);
#endif
#endif
        const int data = t == -1 ? timeMode->centiSecondsPerGram : timeMode->data[t];

        drawTimeLine(t, data, y, x, weightMode, small, position == pos, editor);

        if (position == pos) {

        }
    }

    UI::u8g.setFont(UI::FONT_SMALL);

    drawEditPoint(0, SYMBOL_BACK, back);

    if (gram)
        drawEditPoint(1, weightMode ? "s" : "g", gram);

    if (add)
        drawEditPoint(2, ADD_STRING, add);

    if (del)
        drawEditPoint(3, DEL_STRING, del);

    if (left)
        drawEditPoint(4, MOVE_LEFT_STRING, left);

    if (right)
        drawEditPoint(5, MOVE_RIGHT_STRING, right);

    if (position >= firstChar) {
        UI::u8g.setFont(UI::FONT_REGULAR);
        char charBuf[TimeMode::MAX_CHARS + 1];

        const int i = position - firstChar;
        memcpy(charBuf, &timeMode->name, i + 1);
        charBuf[i + 1] = 0;

        unsigned char w = UI::u8g.getStrWidth(&charBuf[i]);

        charBuf[i] = 0;
        unsigned char x = UI::u8g.getStrWidth(charBuf);

#ifdef PORTRAIT_DISPLAY
        UI::u8g.drawHLine(x, 18, w);
        if (editor)
            UI::u8g.drawHLine(x, 16, w);
#else
        UI::u8g.drawVLine(18, UI::DISPLAY_HEIGHT - x - w, w);
        if (editor)
            UI::u8g.drawVLine(16, UI::DISPLAY_HEIGHT - x - w, w);
#endif
    }
}

void Editor::drawEditPoint(unsigned char p, const char *symbol, const unsigned char pos) {
    unsigned char w = UI::u8g.getStrWidth(symbol);
//    UI::u8g.setFont(UI::FONT_SMALL);
#ifdef PORTRAIT_DISPLAY
    const int x = p > 0 ? p * 11 + 0 : 0;
    const int y = UI::DISPLAY_HEIGHT - 4;
#else
    const int x = UI::DISPLAY_WIDTH - (p == 0 ? 29 : 14);
    const int y = p > 1 ? p * 13 - 3: 10;
#endif

    UI::u8g.drawStr(x, y, symbol);
    if (pos == position) {
        UI::u8g.drawHLine(x, y + 2, w);
    }
};


