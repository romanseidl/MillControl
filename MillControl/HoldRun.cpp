#include "HoldRun.h"

void HoldRun::loop() {
    if (!UI::millButton.isPressed())
        close();
    else
        DirectRun::loop();
}