#include "systemc.h"
#include "timer.h"

int sc_main(int argc, char* argv[]) {
    // Create signals for connecting to the timer
    sc_signal<bool> start;
    sc_signal<bool> timeout;
    sc_clock clock("clock", 10, SC_NS); // 100MHz clock (10ns period)

    // Instantiate the timer
    timer timer1("timer1");
    
    // Connect the signals
    timer1.clock(clock);
    timer1.start(start);
    timer1.timeout(timeout);

    // Create VCD trace file
    sc_trace_file* tf = sc_create_vcd_trace_file("RESULT");
    if (!tf) {
        cout << "Error creating trace file" << endl;
        return 1;
    }

    // Add signals to trace file in specified order
    sc_trace(tf, clock, "clock");
    sc_trace(tf, start, "start");
    sc_trace(tf, timeout, "timeout");
    sc_trace(tf, timer1.count, "count");

    // Start simulation
    sc_start(0, SC_NS);

    // Test Scenario 1: Reset for 3 cycles
    start.write(1);  // Assert reset
    sc_start(30, SC_NS);  // 3 cycles

    // Release reset and let it count
    start.write(0);
    sc_start(50, SC_NS);  // 5 cycles

    // Test Scenario 2: Reset during counting before reaching 0
    start.write(1);
    sc_start(10, SC_NS);  // 1 cycle
    start.write(0);
    sc_start(100, SC_NS);  // 10 cycles

    // Test Scenario 3: Let it count to 0 and then reset
    start.write(0);
    sc_start(60, SC_NS);  // 6 cycles (enough to reach 0)
    start.write(1);
    sc_start(10, SC_NS);  // 1 cycle
    start.write(0);
    sc_start(40, SC_NS);  // 4 cycles

    // Close the trace file
    sc_close_vcd_trace_file(tf);
    
    return 0;
}
