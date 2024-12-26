// FullAdder.h
#ifndef FULLADDER_H
#define FULLADDER_H

#include <systemc.h>
#include "HalfAdder.h"

SC_MODULE(FullAdder) {
    sc_in<bool> A, B, Cin;
    sc_out<bool> S, Cout;

    HalfAdder* ha1;
    HalfAdder* ha2;
    
    sc_signal<bool> s1, c1, c2;

    void process();

    SC_CTOR(FullAdder) {
        ha1 = new HalfAdder("ha1");
        ha2 = new HalfAdder("ha2");

        ha1->A(A);
        ha1->B(B);
        ha1->Sum(s1);
        ha1->Carry(c1);

        ha2->A(s1);
        ha2->B(Cin);
        ha2->Sum(S);
        ha2->Carry(c2);

        SC_METHOD(process);
        sensitive << c1 << c2;
    }

    ~FullAdder() {
        delete ha1;
        delete ha2;
    }
};

#endif
