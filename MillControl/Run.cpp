#pragma once

#include <Arduino.h>
#include "Run.h"
#include "MillControl.h"

Run::Run() {
    pinMode(UI::RELAY_PIN, OUTPUT);
    digitalWrite(UI::RELAY_PIN, OFF);
}

void Run::setMode(unsigned char mode) {
    runTime = mode;
}

void Run::loop() {
    if ((runType == TIMED_RUN && millis() > stopTime)
//Hold Run only exists if there is a mill button
#ifdef MILL_BUTTON
            || (runType == HOLD_RUN && UI::millButton.depressed())
#endif
        )
    {
        close();
    } else if (millis() > (updateTime + 250)) {
        updateTime = millis();
        redraw();
    }
}

void Run::stop() {
    stopMill();
#ifdef MILL_BUTTON
    UI::millButton.setMultiClickButton();
#else
    UI::encoderButton.setMultiClickButton();
#endif
    UI::u8g.begin(); //resetting display - might help if there is interferences with the mill switch
}

void Run::startMill() const { digitalWrite(UI::RELAY_PIN, ON); }

void Run::stopMill() const { digitalWrite(UI::RELAY_PIN, OFF); }

bool Run::start() {
    DEBUG_PRINTLN("open: Run");
    timeMode = &MillControl::TIME_MODE_SELECTOR.getMode();
    pauseTime = 0;

    runDeciSeconds = timeMode->getDeciSeconds(runTime);
    if (runDeciSeconds == TimeMode::SPECIAL_DATA) {
        if (runTime == 2)
            runType = HOLD_RUN;
        else
            runType = STOP_RUN;

        stopTime = millis();
    } else {
        lastEncoderPos = runDeciSeconds / 10;
        setEncoderMode(30000, lastEncoderPos);
        stopTime = millis() + (long) runDeciSeconds * 100l;
        runType = TIMED_RUN;
    }
#ifdef MILL_BUTTON
    UI::millButton.setSingleClickButton();
#else
    UI::encoderButton.setSingleClickButton();
#endif
    startMill();
    return true;
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
        close();
}

void Run::encoderClick() {
    millClick(MillControl::CLICK);
}



void Run::draw() {
    State::draw();

    char c_t[5] = "";
#ifdef PORTRAIT_DISPLAY
    int x = 0;
    int y = UI::LINE_HEIGHT + (UI::DISPLAY_HEIGHT / 3) +  (UI::LARGE_LINE_HEIGHT / 2);
#else
    int x = UI::LINE_HEIGHT * 2 + (UI::LARGE_LINE_HEIGHT / 2) + UI::BORDER_WIDTH * 2; //42
    int y = UI::LARGE_LINE_HEIGHT + UI::BORDER_WIDTH * 2 + UI::BORDER_WIDTH * 5; //29
#endif

    UI::u8g.setFont(UI::FONT_LARGE_NUMERIC);
    UI::drawSymbol(x, y, runTime, UI::LARGE_LINE_HEIGHT, 4, 2);

    long remainingMillis = pauseTime ? pauseTime : stopTime - millis();

    int remainingSeconds = abs(remainingMillis / 1000) + 1;

    if (remainingSeconds < 0)
        remainingSeconds = 0;

    if (pauseTime) {
        UI::u8g.setFont(UI::FONT_REGULAR);
    }

#ifdef PORTRAIT_DISPLAY
    x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT;
    y = UI::DISPLAY_HEIGHT - 12 * UI::BORDER_WIDTH;
#else
    x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT; //115;
    y = UI::DISPLAY_HEIGHT - 6 * UI::BORDER_WIDTH; //53;
#endif

    if (!pauseTime|| (millis() / 500) % 2) {
        sprintf(c_t, "%i", remainingSeconds);
        UI::u8g.drawStr(x - UI::u8g.getStrWidth(c_t), y, c_t);
    }
    UI::u8g.setFont(UI::FONT_REGULAR);
    UI::u8g.drawStr(x, y, "s");


    if (runType == TIMED_RUN && remainingMillis > 0) {
#ifdef PORTRAIT_DISPLAY
        unsigned char w =
                UI::DISPLAY_WIDTH *
                (1.0 - double(remainingMillis) / double(100l * (long) runDeciSeconds));
        UI::u8g.drawBox(0, UI::DISPLAY_HEIGHT - UI::BORDER_WIDTH * 2 - 1, w, UI::BORDER_WIDTH * 2);
#else
        unsigned char w = (UI::DISPLAY_WIDTH - UI::LINE_HEIGHT * 2) * ( 1.0 - double(remainingMillis)/double(100l * (long) runDeciSeconds));
        UI::u8g.drawBox(UI::LINE_HEIGHT * 2, UI::DISPLAY_HEIGHT - UI::BORDER_WIDTH * 2 - 1, w, UI::BORDER_WIDTH * 2);
#endif
    }
}


void Run::encoderChanged(int encoderPos) {
    if(!pauseTime && runType == TIMED_RUN){
        const int encoderDiff = encoderPos - lastEncoderPos;
        stopTime += encoderDiff * 1000l;
        runDeciSeconds += encoderDiff * 10;
        lastEncoderPos = encoderPos;
    }
    lastEncoderPos = encoderPos;
}

#ifdef BREW_BUTTON
//This state will do nothing on a brew click
void Run::brewClick() {
    MillControl::openInBackground(MillControl::BREW_TIMER);
}
#endif