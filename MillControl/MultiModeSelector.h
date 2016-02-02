#pragma once

#include "State.h"
#include "ModeSelector.h"


class MultiModeSelector : public ModeSelector {

public:
    virtual bool start() override;

    virtual void draw() override;

    virtual void encoderClick() override;

    virtual void millClick(unsigned char clickType) override;

    virtual void encoderChanged(int encoderPos) override;

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "MultiModeSelector"; }
#endif
};

