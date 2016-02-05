#ifndef MILLCONTROL_HOLDRUN_H
#define MILLCONTROL_HOLDRUN_H


#include "DirectRun.h"

class HoldRun : public DirectRun {

public:
    HoldRun(const unsigned char run_icon, const int run_data) : DirectRun(run_icon, run_data) { }

    virtual void loop() override;
};


#endif //MILLCONTROL_HOLDRUN_H
