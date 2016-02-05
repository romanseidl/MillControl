#include "Prompt.h"

void Prompt::encoderClick() {
    switch (position) {
        case 0:
            ok();
            break;
        case 1:
            cancel();
            break;
    }
}

void Prompt::encoderChanged(int encoderPos) {
    position = encoderPos;
    redraw();
}

void Prompt::draw() {
    State::draw();

    UI::u8g.setFont(UI::FONT_SMALL);
    UI::drawEditPoint(prompt_position, position == 0, UI::OK_SYMBOL);
    UI::drawEditPoint(prompt_position + 1, position == 1, 'x');
}

void Prompt::cancel() {
    close();
}

void Prompt::ok() {
    close();
}

Prompt::Prompt(const int start_position, const int positions) : start_position(start_position), positions(positions) {

}

Prompt::Prompt() : start_position(0), positions(2) {
}

bool Prompt::start() {
    setEncoderMode(positions, start_position);
    return true;
}

#ifdef BREW_BUTTON
#endif