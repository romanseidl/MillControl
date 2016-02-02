#pragma once

#include "State.h"
#include "Mode.h"

class ModeSelector : public State {
    unsigned long updateTime;
public:
    ModeList timeModes;

    void setMode(Mode *mode);

    Mode &getMode();

    ModeSelector();

protected:

    void eepromWrite();

    int selectedMode;

    virtual bool start() override;


public:

#ifdef BREW_BUTTON

    virtual void loop() override;

    //This is a workaround as there seems to be a strange compiler bug with defines is includes
    virtual void brewClick();

#endif

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "ModeSelector"; }
#endif
};


