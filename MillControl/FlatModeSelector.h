#ifndef MILLCONTROL_FLATMODESELECTOR_H
#define MILLCONTROL_FLATMODESELECTOR_H

#include "State.h"
#include "ModeSelector.h"

class FlatModeSelector : public ModeSelector {
    unsigned char selectedTime = 0;

public:
    FlatModeSelector();

private:
    void eepromWriteTime();

    int findTimePos(int mode, int time);

public:
    virtual bool start() override;

    virtual void draw() override;

    virtual void encoderClick() override;

    virtual void millClick(unsigned char clickType) override;

    virtual void encoderChanged(int encoderPos) override;

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "FlatModeSelector"; }
#endif
};

#endif //MILLCONTROL_FLATMODESELECTOR_H