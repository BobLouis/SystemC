// timer.h
#include "systemc.h"

SC_MODULE(timer) {
    sc_in<bool> clock;    // Clock input
    sc_in<bool> start;    // Synchronous reset (active high)
    sc_out<bool> timeout; // Timeout signal
    
    int count;            // Internal counter

    void runtimer();      // Main process

    SC_CTOR(timer) {
        SC_CTHREAD(runtimer, clock.pos());
        reset_signal_is(start, true); // Specify reset signal (active high)
        count = 0;
    }
};
