#include "MillControl.h"

const char Editor::DEL_STRING[2] = "x";
const char Editor::ADD_STRING[2] = "+";
const char Editor::MOVE_LEFT_STRING[2] = "<";
const char Editor::MOVE_RIGHT_STRING[2] = ">";

void Editor::start() {
    timeMode = &MillControl::TIME_MODE_SELECTOR.getMode();
    timeModes = &MillControl::TIME_MODE_SELECTOR.getTimeModes();

    firstChar = TimeMode::TIMES_PER_MODE + 1;

    const int size = MillControl::TIME_MODE_SELECTOR.size();

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

    if(deleteMode)
        setEncoderMode(2, 0);
    else
        setEncoderMode(firstChar + TimeMode::MAX_CHARS, position);

    redraw();
}

void Editor::stop() {
}

int *Editor::getTime() {
    return &timeMode->time[position];
}

char *Editor::getChar() {
    return &timeMode->name[position - firstChar];
}


void Editor::encoderClick() {
    if (position < TimeMode::TIMES_PER_MODE) {
        MillControl::setState(MillControl::TIME_EDITOR);
    } else if (position == TimeMode::TIMES_PER_MODE) {
        if(deleteMode) {
            deleteMode = false;
            start();
        } else {
            timeModes->eepromWrite();
            MillControl::setState(MillControl::TIME_MODE_SELECTOR);
        }
        position = 0;
    } else if (position == add) {
        MillControl::TIME_MODE_SELECTOR.setMode(&timeModes->insertAfer(*timeMode));
        position = 0;
        start();
    } else if (position == left) {
        MillControl::TIME_MODE_SELECTOR.setMode(&timeModes->moveLeft(*timeMode));
        position = TimeMode::TIMES_PER_MODE;
        start();
    } else if (position == right) {
        MillControl::TIME_MODE_SELECTOR.setMode(&timeModes->moveRight(*timeMode));
        position = TimeMode::TIMES_PER_MODE;
        start();
    } else if (position == del) {
        if(deleteMode){
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

void Editor::encoderChanged(int encoderPos) {
    position = (deleteMode ? TimeMode::TIMES_PER_MODE : 0) + encoderPos;
    redraw();
}


void Editor::draw() {
    draw(false);
}

void Editor::drawEditor() {
    draw(true);
}

void Editor::draw(bool editor) {
    TitledState::draw();

    for (unsigned char t = 0; t < TimeMode::TIMES_PER_MODE; t++) {
        const int time = timeMode->time[t];

#ifdef PORTRAIT_DISPLAY
        unsigned char x = 0;
    #ifdef MILL_BUTTON
        unsigned char y = 52 + t * 25;
    #else
        unsigned char y = 57 + t * 30;
    #endif
#else
        unsigned char x = 35;
    #ifdef MILL_BUTTON
        unsigned char y = 16 + t * 22;
    #else
        unsigned char y = 23 + t * 32;
    #endif
#endif

        drawTimeLine(t, time, y, x);

        if (position == t) {
#if defined(MILL_BUTTON) && !defined(PORTRAIT_DISPLAY)
            unsigned char d = 2;
#else
            unsigned char d = 4;
#endif
            if (editor)
                UI::u8g.drawHLine(x + 24, y + d, time == TimeMode::SPECIAL_TIME ? (t == 3 ? 31 : 40) : 28);
            else
                UI::u8g.drawHLine(x, y + d, 64);
        }
    }

    UI::u8g.setFont(UI::FONT_SMALL);

    drawEditPoint(0, SYMBOL_BACK, TimeMode::TIMES_PER_MODE);

    if (add)
        drawEditPoint(1, ADD_STRING, add);

    if (del)
        drawEditPoint(2, DEL_STRING, del);

    if (left)
        drawEditPoint(3, MOVE_LEFT_STRING, left);

    if (right)
        drawEditPoint(4, MOVE_RIGHT_STRING, right);

    if (position >= firstChar) {

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

#ifdef PORTRAIT_DISPLAY
    const int x = p > 0 ? p * 12 + 5 : 0;
    const int y = UI::DISPLAY_HEIGHT - 1;
#else
    const int x = UI::DISPLAY_WIDTH - 14;
    const int y = p * 13 + 10;
#endif

    UI::u8g.drawStr(x, y, symbol);
    if (pos == position) {
        UI::u8g.drawHLine(x, y, w);
    }
};


