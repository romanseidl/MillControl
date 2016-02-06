#include "ScaleTarePrompt.h"
#include "MillControl.h"

#ifdef SCALE

bool ScaleTarePrompt::open() {
    UI::scale.power_up();
    UI::u8g.begin();
    //Need to wait for the scale to boot up
    delay(2500);
    UI::scale.calibrate_noise();
    UI::scale.reset_floating_weight();
    return Prompt::start();
}


//this is called when a fallback happens!
bool ScaleTarePrompt::start() {
    //Convention is: Calibrator returns 0 if cancel occured
    int weight = *MillControl::CALIBRATION_PROMPT.getData();
    if (weight)
        UI::scale.set_weight(weight);
    return close();
}

bool ScaleTarePrompt::close() {
    UI::scale.power_down();
    UI::u8g.begin();
    return State::close();
}

void ScaleTarePrompt::loop() {
    weight = UI::scale.get_floating_weight();
    if (millis() > (updateTime + 250)) {
        updateTime = millis();
        redraw();
    }
}

void ScaleTarePrompt::ok() {
    UI::scale.tare();
    *MillControl::CALIBRATION_PROMPT.getData() = 500;
    MillControl::open(MillControl::CALIBRATION_PROMPT);
}

void ScaleTarePrompt::draw() {
    Prompt::draw();
    UI::drawSubtitle(UI::TARE_TITLE);
    UI::drawRunWeightLine(weight, 0);
}

#endif