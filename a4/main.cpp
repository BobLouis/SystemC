// main.cpp
#include <systemc.h>
#include "HalfAdder.h"
#include "FullAdder.h"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    // Signals for Half Adder
    sc_signal<bool> ha_a, ha_b, ha_sum, ha_carry;
    
    // Signals for Full Adder
    sc_signal<bool> fa_a, fa_b, fa_cin, fa_s, fa_cout;

    // Create modules
    HalfAdder ha("half_adder");
    FullAdder fa("full_adder");

    // Connect Half Adder
    ha.A(ha_a);
    ha.B(ha_b);
    ha.Sum(ha_sum);
    ha.Carry(ha_carry);

    // Connect Full Adder
    fa.A(fa_a);
    fa.B(fa_b);
    fa.Cin(fa_cin);
    fa.S(fa_s);
    fa.Cout(fa_cout);

    // Create VCD trace file
    sc_trace_file* tf = sc_create_vcd_trace_file("RESULT");

    // Add signals to trace file in specified order
    sc_trace(tf, ha_a, "half_adder_A");
    sc_trace(tf, ha_b, "half_adder_B");
    sc_trace(tf, ha_sum, "half_adder_Sum");
    sc_trace(tf, ha_carry, "half_adder_Carry");
    sc_trace(tf, fa_a, "full_adder_A");
    sc_trace(tf, fa_b, "full_adder_B");
    sc_trace(tf, fa_cin, "full_adder_Cin");
    sc_trace(tf, fa_s, "full_adder_S");
    sc_trace(tf, fa_cout, "full_adder_Cout");

    // Start simulation
    sc_start(1, SC_NS);

    // Test Half Adder (4 combinations)
    cout << "\nHalf Adder Test Cases:" << endl;
    cout << "A B | Sum Carry" << endl;
    cout << "--------------" << endl;

    bool test_inputs[4][2] = {{0,0}, {0,1}, {1,0}, {1,1}};
    
    for(int i = 0; i < 4; i++) {
        ha_a.write(test_inputs[i][0]);
        ha_b.write(test_inputs[i][1]);
        sc_start(1, SC_NS);
        
        cout << test_inputs[i][0] << " " << test_inputs[i][1] << " | "
             << ha_sum.read() << "   " << ha_carry.read() << endl;
    }

    // Test Full Adder (8 combinations)
    cout << "\nFull Adder Test Cases:" << endl;
    cout << "A B Cin | Sum Cout" << endl;
    cout << "------------------" << endl;

    bool test_inputs_fa[8][3] = {
        {0,0,0}, {0,0,1}, {0,1,0}, {0,1,1},
        {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1}
    };

    for(int i = 0; i < 8; i++) {
        fa_a.write(test_inputs_fa[i][0]);
        fa_b.write(test_inputs_fa[i][1]);
        fa_cin.write(test_inputs_fa[i][2]);
        sc_start(1, SC_NS);
        
        cout << test_inputs_fa[i][0] << " " << test_inputs_fa[i][1] << " "
             << test_inputs_fa[i][2] << "   | "
             << fa_s.read() << "   " << fa_cout.read() << endl;
    }

    sc_close_vcd_trace_file(tf);
    return 0;
}
