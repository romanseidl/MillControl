#include "HoldRun.h"

void HoldRun::loop() {
#ifdef MILL_BUTTON
    if (!UI::millButton.isPressed())
        close();
    else
        DirectRun::loop();
#endif
}