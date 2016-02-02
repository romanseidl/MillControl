//
// Created by roman on 15.01.16.
//

#include <TimerOne.h>
#include "MultiTimer1.h"
#include "UI.h"

unsigned char MultiTimer1::n_timers = 0;
Timed *MultiTimer1::timers[MAX_TIMERS];

//Trigger all events
void MultiTimer1::update() {
    //DEBUG_PRINTLN(n_timers)
    for (unsigned char b = 0; b < n_timers; b++)
        MultiTimer1::timers[b]->run();
}

void MultiTimer1::add(Timed *timed) {
    DEBUG_PRINTLN("MultiTimer1::add()")
    DEBUG_PRINTLN(n_timers);
    // Register Button
    MultiTimer1::timers[n_timers] = timed;
    n_timers++;
}


void MultiTimer1::start() {
    //Microseconds
    Timer1.initialize(INTERVAL);
    Timer1.attachInterrupt(MultiTimer1::update);
}
