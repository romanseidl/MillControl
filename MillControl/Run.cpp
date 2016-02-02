#include "Run.h"
#include "MillControl.h"

#define portOfPin(P)\
  (((P)>=0&&(P)<8)?&PORTD:(((P)>7&&(P)<14)?&PORTB:&PORTC))
#define ddrOfPin(P)\
  (((P)>=0&&(P)<8)?&DDRD:(((P)>7&&(P)<14)?&DDRB:&DDRC))
#define pinIndex(P)((uint8_t)(P>13?P-14:P&7))
#define pinMask(P)((uint8_t)(1<<pinIndex(P)))

#define pinAsOutput(P) *(ddrOfPin(P))|=pinMask(P)
#define digitalLow(P) *(portOfPin(P))&=~pinMask(P)
#define digitalHigh(P) *(portOfPin(P))|=pinMask(P)

Run::Run() {
    pinAsOutput(UI::RELAY_PIN);
    digitalHigh(UI::RELAY_PIN);
}

//Initialize run data - > data gets pushed by the selector
void Run::setMode(unsigned char _run_icon, int _run_data, bool hold) {
    run_icon = _run_icon;
    run_data = _run_data;

    if (run_data == Mode::SPECIAL_DATA) {
        if (hold)
            runType = HOLD_RUN;
        else
            runType = STOP_RUN;
    } else {
        runType = TIMED_RUN;
    }
}

//watches if the run should be stopped and updates display
void Run::loop() {
    if ((runType == TIMED_RUN && millis() > stopTime)
        //Hold Run only exists if there is a mill button
        #ifdef MILL_BUTTON
        || (runType == HOLD_RUN && !UI::millButton.isPressed())
#endif
            ) {
        close();
    } else if (millis() > (updateTime + 250)) {
        updateTime = millis();
        redraw();
    }
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

void Run::startMill() const { digitalLow(UI::RELAY_PIN); }

void Run::stopMill() const { digitalHigh(UI::RELAY_PIN); }

bool Run::start() {
    pauseTime = 0;

    stopTime = millis();
    if (runType == TIMED_RUN) {
        lastEncoderPos = run_data / 10;
        setEncoderMode(10000, lastEncoderPos);
        stopTime += (long) run_data * 100l;
    }
#ifdef MILL_BUTTON
    UI::millButton.setMultiClick(false);
#else
    UI::encoderButton.setMultiClick(false);
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
    unsigned char x = 0;
    unsigned char y = UI::LINE_HEIGHT + (UI::DISPLAY_HEIGHT / 3) + (UI::LARGE_LINE_HEIGHT / 2);
#else
    unsigned char x = UI::LINE_HEIGHT * 2 + (UI::LARGE_LINE_HEIGHT / 2) + UI::BORDER_WIDTH * 2; //42
    unsigned char y = UI::LARGE_LINE_HEIGHT + UI::BORDER_WIDTH * 2 + UI::BORDER_WIDTH * 5; //29
#endif

    UI::u8g.setFont(UI::FONT_LARGE_NUMERIC);
    UI::drawLargeSymbol(x, y, run_icon);

    long remainingMillis = pauseTime ? pauseTime : stopTime - millis();

    int remainingSeconds = abs(remainingMillis / 1000) + 1;

    if (remainingSeconds < 0)
        remainingSeconds = 0;

    if (pauseTime)
        UI::u8g.setFont(UI::FONT_REGULAR);

#ifdef PORTRAIT_DISPLAY
    x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT;
    y = UI::DISPLAY_HEIGHT - 12 * UI::BORDER_WIDTH;
#else
    x = UI::DISPLAY_WIDTH - UI::LINE_HEIGHT; //115;
    y = UI::DISPLAY_HEIGHT - 6 * UI::BORDER_WIDTH; //53;
#endif

    if (!pauseTime || (millis() / 500) % 2) {
        sprintf(c_t, "%i", remainingSeconds);
        UI::u8g.drawStr(x - UI::u8g.getStrWidth(c_t), y, c_t);
    }
    UI::u8g.setFont(UI::FONT_REGULAR);
    UI::u8g.drawStr(x, y, "s");


    if (runType == TIMED_RUN && remainingMillis > 0) {
#ifdef PORTRAIT_DISPLAY
        unsigned char w =
                UI::DISPLAY_WIDTH *
                (1.0 - double(remainingMillis) / double(100l * (long) run_data));
        UI::u8g.drawBox(0, UI::DISPLAY_HEIGHT - UI::BORDER_WIDTH * 2 - 1, w, UI::BORDER_WIDTH * 2);
#else
        unsigned char w = (UI::DISPLAY_WIDTH - UI::LINE_HEIGHT * 2) * ( 1.0 - double(remainingMillis)/double(100l * (long) run_data));
        UI::u8g.drawBox(UI::LINE_HEIGHT * 2, UI::DISPLAY_HEIGHT - UI::BORDER_WIDTH * 2 - 1, w, UI::BORDER_WIDTH * 2);
#endif
    }
}


void Run::encoderChanged(int encoderPos) {
    if (!pauseTime && runType == TIMED_RUN) {
        const int encoderDiff = encoderPos - lastEncoderPos;
        //Adapt time and data
        stopTime += encoderDiff * 1000l;
        run_data += encoderDiff * 10;
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
