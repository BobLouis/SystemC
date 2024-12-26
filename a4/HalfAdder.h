// HalfAdder.h
#ifndef HALFADDER_H
#define HALFADDER_H

#include <systemc.h>

SC_MODULE(HalfAdder) {
    sc_in<bool> A, B;
    sc_out<bool> Sum, Carry;

    void process();

    SC_CTOR(HalfAdder) {
        SC_METHOD(process);
        sensitive << A << B;
    }
};

#endif
