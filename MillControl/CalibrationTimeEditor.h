#pragma once

#include "TimeEditor.h"

//==================================================================================================
// EditTime State

class CalibrationTimeEditor : public TimeEditor {
public:
    virtual bool start() override;
    virtual void draw() override;
    virtual void encoderClick() override;

#ifdef DEBUG
    virtual char*  getClassName() {return "CalibrationTimeEditor"; }
#endif
};


