//
// Created by roman on 05.02.16.
//

#ifndef MILLCONTROL_DIRECTRUN_H
#define MILLCONTROL_DIRECTRUN_H


#include "Run.h"

class DirectRun : public Run {
    unsigned long startTime;
public:
    DirectRun(const unsigned char run_icon, const int run_data) : Run(run_icon, run_data) { }

    virtual bool start() override;

    virtual void millClick(unsigned char i) override;

    virtual void draw() override;
};


#endif //MILLCONTROL_DIRECTRUN_H
