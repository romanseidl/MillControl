#include "WeightCalibrationRun.h"
#include "MillControl.h"

bool WeightCalibrationRun::open() {
    updateTime = 0;
    startTime = millis();
    Run::startMill();
    return true;
}

void WeightCalibrationRun::loop() {
#ifdef MILL_BUTTON
    if (!UI::millButton.isPressed())
        encoderClick();
#endif

    if (millis() > (updateTime + 250)) {
        updateTime = millis();
        redraw();
    }
}

//this is called when a fallback happens!
bool WeightCalibrationRun::start() {
    //Convention is: Calibrator returns 0 if cancel occured
    int deciGrams = *MillControl::CALIBRATION_PROMPT.getData();
    if (deciGrams) {
        long centiSecondsPerGram = getRunMillis() / MillControl::CALIBRATION_PROMPT.weight;
        if (centiSecondsPerGram > Mode::SPECIAL_DATA)
            centiSecondsPerGram = Mode::SPECIAL_DATA;
        MillControl::TIME_MODE_SELECTOR.getMode().calibration_data = centiSecondsPerGram;
    }
    return close();
}

void WeightCalibrationRun::stop() {
    stopTime = millis();
    Run::stopMill();
}

void WeightCalibrationRun::millClick(unsigned char i) {
    encoderClick();
}


void WeightCalibrationRun::encoderClick() {
    *MillControl::CALIBRATION_PROMPT.getData() = 70;
    MillControl::open(MillControl::CALIBRATION_PROMPT);
}

void WeightCalibrationRun::draw() {
    State::draw();
    UI::drawSubtitle(UI::CALIBRATION_TITLE);

#ifdef PORTRAIT_DISPLAY
    const char x= UI::DISPLAY_WIDTH - UI::SMALL_LINE_HEIGHT - UI::LINE_HEIGHT - UI::BORDER_WIDTH;
    const char y= UI::DISPLAY_HEIGHT - UI::BORDER_WIDTH - 1;
#else
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT - 1; //89
    const char y = UI::DISPLAY_HEIGHT - 3 * UI::BORDER_WIDTH - 1;//57;
#endif

    UI::drawRunTime(x, y, (millis() - startTime) / 1000);
}

unsigned long WeightCalibrationRun::getRunMillis() {
    return stopTime - startTime;
}

