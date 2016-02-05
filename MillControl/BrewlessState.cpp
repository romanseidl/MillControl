#include "BrewlessState.h"
#include "MillControl.h"

//No brew timer here
void BrewlessState::brewClick() {
    MillControl::openInBackground(MillControl::BREW_TIMER);
}