#ifndef MILLCONTROL_PROMPT_H
#define MILLCONTROL_PROMPT_H

#include "BrewlessState.h"

class Prompt : public BrewlessState {
    const int start_position;
    const int positions;
public:
    Prompt();

    Prompt(const int start_position, const int positions);

    virtual void encoderClick() override;

    virtual void encoderChanged(int encoderPos) override;

    virtual bool start() override;

    virtual void ok();

    virtual void cancel();

protected:
    int position = 0;
    unsigned char prompt_position = 0;

    virtual void draw() override;
};

#endif //MILLCONTROL_PROMPT_H