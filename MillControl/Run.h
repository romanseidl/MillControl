//==================================================================================================
// RunMode
#pragma once

#include "State.h"
#include "TimeMode.h"



class Run : public State {

    static const unsigned char RELAY_PIN = 10;
    static const unsigned char ON = LOW;
    static const unsigned char OFF = HIGH;

    static const unsigned char TIMED_RUN = 0;
    static const unsigned char STOP_RUN = 1;
    static const unsigned char HOLD_RUN = 2;


    unsigned char runType;

    unsigned long stopTime;
    unsigned long updateTime;
    unsigned long pauseTime;
    int runDeciSeconds;
    int lastEncoderPos;

    TimeMode *timeMode;

    unsigned char runTime = 0;

public:
    void stopMill() const;
    void startMill() const;

    Run();

    void setMode(unsigned char);

    virtual void start() override;

    virtual void stop() override;

    virtual void millClick(unsigned char i) override;


    virtual void encoderChanged(int encoderPos) override;

    virtual void encoderClick() override;

protected:
    virtual void loop() override;

    virtual void draw() override;
};
