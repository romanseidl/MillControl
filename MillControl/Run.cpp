#pragma once

#include "Run.h"
#include "MillControl.h"

Run::Run() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, OFF);
}

void Run::setMode(unsigned char mode) {
    runTime = mode;
}

void Run::loop() {
    if ((runType == TIMED_RUN && millis() > stopTime) || (runType == HOLD_RUN && UI::millButton.depressed())) {
        MillControl::setState(MillControl::TIME_MODE_SELECTOR);
    } else if (millis() > (updateTime + 250)) {
        updateTime = millis();
        redraw();
    }
}

void Run::stop() {
    stopMill();
#ifdef MILL_BUTTON
    UI::millButton.setMultiClickButton();
#endif
}

void Run::startMill() const { digitalWrite(RELAY_PIN, ON); }

void Run::stopMill() const { digitalWrite(RELAY_PIN, OFF); }

void Run::start() {
    timeMode = &MillControl::TIME_MODE_SELECTOR.getMode();
    pauseTime = 0;

    int time = timeMode->time[runTime];
    if (time > 0) {
        if (time == TimeMode::SPECIAL_TIME) {
            if (runTime == 2)
                runType = HOLD_RUN;
            else
                runType = STOP_RUN;

            stopTime = millis();
        } else {
            stopTime = millis() + (long) time * 1000l;
            runType = TIMED_RUN;
        }
#ifdef MILL_BUTTON
        UI::millButton.setSingleClickButton();
#endif
        startMill();
        redraw();
    } else
        MillControl::setState(MillControl::TIME_MODE_SELECTOR);
}

//If this is a timed tun this will only stop
void Run::millClick(unsigned char clickType) {
    if (runType == TIMED_RUN) {
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
    } else
        MillControl::setState(MillControl::TIME_MODE_SELECTOR);
}

void Run::encoderClick() {
    millClick(MillControl::CLICK);
}

void Run::draw() {
    TitledState::draw();

    char c_t[4] = "";
#ifdef PORTRAIT_DISPLAY
    int x = 14;
    int y = 62;
#else
    int x = 42;
    int y = 29;
#endif

    UI::u8g.setColorIndex(1);
    UI::u8g.drawDisc(x + 8, y - 11, 17);

    UI::u8g.setColorIndex(0);

    switch (runTime) {
        case 0:
            UI::u8g.setFont(UI::FONT_LARGE);
            UI::u8g.drawStr(x, y, "1");
            break;
        case 1:
            UI::u8g.setFont(UI::FONT_LARGE);
            UI::u8g.drawStr(x + 1, y, "2");
            break;
        case 2:
            UI::u8g.setFont(UI::FONT_BOLD);
            UI::u8g.drawStr(x + 3, y - 5, UI::SYMBOL_LONG);
            UI::u8g.setFont(UI::FONT_LARGE);
            break;
    }

    UI::u8g.setColorIndex(1);

    long remainingMillis = pauseTime ? pauseTime : stopTime - millis();

    int remaining_sec = abs(remainingMillis / 1000) + 1;

    if (remaining_sec < 0)
        remaining_sec = 0;

    if (pauseTime) {
        UI::u8g.setFont(UI::FONT_BOLD);
    }

#ifdef PORTRAIT_DISPLAY
    x = 50;
    y = UI::DISPLAY_HEIGHT - 24;
#else
    x = 115;
    y = 53;
#endif

    if (!pauseTime|| (millis() / 500) % 2) {
        sprintf(c_t, "%i", remaining_sec);
        UI::u8g.drawStr(x - UI::u8g.getStrWidth(c_t), y, c_t);
    }
    UI::u8g.setFont(UI::FONT_BOLD);
    UI::u8g.drawStr(x, y, "s");


    if (runType == TIMED_RUN && remainingMillis > 0) {
#ifdef PORTRAIT_DISPLAY
        unsigned char w =
                UI::DISPLAY_WIDTH *
                (1.0 - double(remainingMillis) / double(1000l * (long) timeMode->time[runTime]));
        UI::u8g.drawBox(0, UI::DISPLAY_HEIGHT - 3, w, 2);
#else
        unsigned char w = (UI::DISPLAY_WIDTH - 31) * ( 1.0 - double(remainingMillis)/double(1000l * (long) timeMode->time[runTime]));
        UI::u8g.drawBox(31, UI::DISPLAY_HEIGHT - 4, w, 3);
#endif
    }
}



