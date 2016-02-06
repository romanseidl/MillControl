#include "BrewlessState.h"
#include "MillControl.h"

#ifdef BREW_BUTTON
//No brew timer here
void BrewlessState::brewClick() {
    MillControl::openInBackground(MillControl::BREW_TIMER);
}
#endif