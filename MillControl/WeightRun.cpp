#include "WeightRun.h"
#include "MillControl.h"

#ifdef SCALE

bool WeightRun::start() {
    calibration_data = &mode->calibration_data;

    UI::scale.power_up();
    UI::scale.tare(50);

    start_time = millis();
    return Run::start();
}

void WeightRun::loop() {
    if (millis() - start_time > STARTUP_DELAY)
        weight = max(0, UI::scale.get_floating_weight());

    if (weight > (run_data + *calibration_data)) {
        //Stop Mill etc.
        Run::close();
        //Learn how much the scale is missing out
        long weight = UI::scale.get_stable_weight();
        long offset = run_data - weight;
/*        Serial.begin(9600);
        Serial.print("\n");
        Serial.print(run_data);
        Serial.print(",");
        Serial.print(weight);
        Serial.print(",");
        Serial.print(offset);
*/
        //only believe offsets up to 0.5 grams
        if (offset < 10 && offset > -10) {
/*            Serial.print(",");
            Serial.print(*calibration_data);
*/
            *calibration_data = *calibration_data + (offset / 2);
/*            Serial.print(",");
            Serial.print(*calibration_data);
*/
            MillControl::TIME_MODE_SELECTOR.timeModes.eepromWrite();
        }
//        Serial.flush();
        UI::scale.power_down();
    }
    else
        Run::loop();
}

bool WeightRun::close() {
    Run::close();
    UI::scale.power_down();
}


void WeightRun::draw() {
    Run::draw();

#ifdef PORTRAIT_DISPLAY
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT;
    const char y = UI::DISPLAY_HEIGHT - 12 * UI::BORDER_WIDTH;
#else
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT; //115;
    const char y = UI::DISPLAY_HEIGHT - 6 * UI::BORDER_WIDTH; //53;
#endif

    UI::drawRunWeight(x, y, weight);
}

void WeightRun::millClick(unsigned char i) {
    close();
}

#endif
