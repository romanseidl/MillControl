#ifndef MILLCONTROL_MODESELECTOR_H
#define MILLCONTROL_MODESELECTOR_H

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
#endif

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "ModeSelector"; }
#endif
};

#endif //MILLCONTROL_MODESELECTOR_H