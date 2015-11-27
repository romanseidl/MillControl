#include "State.h"
#include "MillControl.h"

//--------------------------------------------------------------------------------
// State

void State::redraw() {
    UI::u8g.firstPage();
    do {
        draw();
    } while (UI::u8g.nextPage());
}

void State::setEncoderMode(int positions, int position) {
    UI::encoder.setMode(positions, position);
}

void State::millClick(unsigned char clickType) {
}

void State::stop() {
}

void State::start() {
}

void State::draw() {
}

void State::loop() {
}

void State::encoderChanged(int encoderPos) {
}

void State::encoderClick() {
}

//--------------------------------------------------------------------------------
// Titled State

void TitledState::draw() {
    State::draw();
    UI::u8g.setFont(UI::FONT_SMALL);
#ifdef PORTRAIT_DISPLAY
    UI::u8g.drawStr(0, 14, MillControl::TIME_MODE_SELECTOR.getMode().name);
#else
 //   UI::u8g.drawStr90(14, 50, MillControl::TIME_MODE_SELECTOR.getMode().name);
    UI::u8g.drawStr270(14, UI::DISPLAY_HEIGHT - 1, MillControl::TIME_MODE_SELECTOR.getMode().name);
#endif

    //Memory display
/*    UI::u8g.setFont(u8g_font_u8glib_4);
    char mem_str[5];
    const int mem = freeMemory();
    sprintf(mem_str, "%i", mem);
    UI::u8g.drawStr(0, 24, mem_str);*/
}

