#pragma once
//
// Created by roman on 03.12.15.
//

#include "State.h"

class CalibrationPrompt : public State {
    int deciGrams = 70;
    int position = 0;

protected:
    virtual void draw() override;

    void draw(bool editor);

public:
    virtual bool start() override;

    virtual bool open() override;

    virtual void encoderClick() override;

    virtual void encoderChanged(int encoderPos) override;
    void drawEditor();

    int *getTime();

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "CalibrationPrompt"; }
#endif
};

