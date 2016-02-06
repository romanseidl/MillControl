#include "Run.h"
#include "MillControl.h"

Run::Run(unsigned char run_icon, int run_data) : run_data(run_data), run_icon(run_icon) {
    //Init Relay
    pinMode(UI::RELAY_PIN, OUTPUT);
    digitalWrite(UI::RELAY_PIN, HIGH);
}

bool Run::start() {
#ifdef MILL_BUTTON
    UI::millButton.setMultiClick(false);
#else
    UI::encoderButton.setMultiClick(false);
#endif
    startMill();
    UI::u8g.begin(); //resetting display - might help if there is interferences with the mill switch
    return true;
}

void Run::stop() {
    stopMill();
#ifdef MILL_BUTTON
    UI::millButton.setMultiClick(true);
#else
    UI::encoderButton.setMultiClick(true);
#endif
    UI::u8g.begin(); //resetting display - might help if there is interferences with the mill switch
}


void Run::loop() {
    if (millis() > (update_time + 250)) {
        update_time = millis();
        redraw();
    }
}

void Run::startMill() { digitalWrite(UI::RELAY_PIN, LOW); }

void Run::stopMill() { digitalWrite(UI::RELAY_PIN, HIGH); }


void Run::encoderClick() {
    millClick(MillControl::CLICK);
}


void Run::draw() {
    State::draw();

#ifdef PORTRAIT_DISPLAY
    unsigned char x = 0;
    unsigned char y = UI::LINE_HEIGHT + (UI::DISPLAY_HEIGHT / 3) + (UI::LARGE_LINE_HEIGHT / 2);
#else
    unsigned char x = UI::LINE_HEIGHT * 2 + UI::BORDER_WIDTH; //42
    unsigned char y = UI::LARGE_LINE_HEIGHT + UI::BORDER_WIDTH * 2 + UI::BORDER_WIDTH; //29
#endif

    UI::u8g.setFont(UI::FONT_LARGE_NUMERIC);
    UI::drawLargeSymbol(x, y, run_icon);

}

bool Run::close() {
    if (BrewlessState::close())
        delete this;
}

