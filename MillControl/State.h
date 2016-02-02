#pragma once

#include "UI.h"

class State {
protected:
    void setEncoderMode(int, int);
    bool close();

public:
    State* previousState = NULL;
    void redraw();

    virtual bool open();

    virtual bool start();
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

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "State"; }
#endif
};

