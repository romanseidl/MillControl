#ifndef MILLCONTROL_BREWLESSSTATE_H
#define MILLCONTROL_BREWLESSSTATE_H

#include "State.h"

class BrewlessState : public State {

public:
    virtual void brewClick() override;
};

#endif //MILLCONTROL_BREWLESSSTATE_H
