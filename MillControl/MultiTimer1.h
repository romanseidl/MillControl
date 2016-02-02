#pragma once

#include "Timed.h"

class MultiTimer1 {
    static const unsigned char MAX_TIMERS = 10;
    static const long INTERVAL = 10l * 1000l;
public:
    static Timed *timers[];

    static unsigned char n_timers;
    static unsigned char interval;

    static void update();

    static void add(Timed *timed);

    static void start();
};


