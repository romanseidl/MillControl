#include "TimedRun.h"


bool TimedRun::start() {
    pauseTime = 0;

    stopTime = millis();
    lastEncoderPos = run_data / 10;
    setEncoderMode(10000, lastEncoderPos);
    stopTime += (long) run_data * 100l;

    return Run::start();
}

//watches if the run should be stopped and updates display
void TimedRun::loop() {
/*        //Hold Run only exists if there is a mill button
        #ifdef MILL_BUTTON
        || (run_type == HOLD_RUN && !UI::millButton.isPressed())
#endif
            ) */
    if (millis() > stopTime)
        close();
    else
        Run::loop();
}

//If this is a timed tun this will only stop
void TimedRun::millClick(unsigned char clickType) {
    if (!pauseTime) {
        pauseTime = stopTime - millis();
        //Will time out
        stopTime = millis() + UI::PAUSE_TIME * 1000;
        stopMill();
    } else {
        stopTime = millis() + pauseTime;
        startMill();
        pauseTime = 0;
    }
}

void TimedRun::draw() {
    Run::draw();

    long remainingMillis = max(0, pauseTime ? pauseTime : stopTime - millis());
    int remainingSeconds = remainingMillis / 1000 + 1;

#ifdef PORTRAIT_DISPLAY
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT;
    const char y = UI::DISPLAY_HEIGHT - 12 * UI::BORDER_WIDTH;
#else
    const char x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT; //115;
    const char y = UI::DISPLAY_HEIGHT - 6 * UI::BORDER_WIDTH; //53;
#endif

    if (!pauseTime || (millis() / 500) % 2) {
        UI::drawRunTime(x, y, remainingSeconds, pauseTime);
    }

    UI::drawProgressBar(1.0 - double(remainingMillis) / double(100l * (long) run_data));
}

void TimedRun::encoderChanged(int encoderPos) {
    const int encoderDiff = encoderPos - lastEncoderPos;
    //Adapt time and data
    stopTime += encoderDiff * 1000l;
    run_data += encoderDiff * 10;
    lastEncoderPos = encoderPos;
    lastEncoderPos = encoderPos;
}