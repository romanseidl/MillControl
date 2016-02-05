#ifndef MILLCONTROL_RUN_H
#define MILLCONTROL_RUN_H

#include "Mode.h"
#include "BrewlessState.h"

class Run : public BrewlessState {
    unsigned long update_time = 0;

    const unsigned char run_icon;
protected:
    int run_data;
public:
    Run(const unsigned char run_icon, const int run_data);

    static void stopMill();

    static void startMill();

    virtual bool start() override;

    virtual void stop() override;

    virtual void encoderClick() override;

#ifdef DEBUG
    virtual char*  getClassName() {return (char *) "Run"; }
#endif

    virtual void loop() override;

    virtual void draw() override;

protected:
    virtual bool close() override;
};

#endif //MILLCONTROL_RUN_H