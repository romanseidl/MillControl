#pragma once

#include "Button.h"
#include "UI.h"

class State {
private:
    //the active mode - may be this or any sub-mode
protected:
    void setEncoderMode(int, int);

public:
    void redraw();

    virtual void start();
    virtual void stop();

    //Draws the UI - title only for the master state
    virtual void draw();

    //UI functions
    virtual void millClick(unsigned char);

    virtual void encoderClick();

    virtual void encoderChanged(int encoderPos);

#ifdef BREW_BUTTON
    virtual void brewClick();
#endif

    //Delegating loop function - false will exit a sub-mode (if there is one)
    virtual void loop();

};

