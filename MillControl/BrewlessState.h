#ifndef MILLCONTROL_BREWLESSSTATE_H
#define MILLCONTROL_BREWLESSSTATE_H

#include "State.h"

class BrewlessState : public State {
#ifdef BREW_BUTTON
public:
    virtual void brewClick() override;
#endif
};

#endif //MILLCONTROL_BREWLESSSTATE_H
