#include <systemc.h>
#include <fstream>
#include <iostream>
#include "FIR.h"

/*
 * main.cpp
 *   - Instantiates two FIR filters: fir32 (32 taps) and fir48 (48 taps)
 *   - Connects both to a 200MHz clock (period = 5 ns)
 *   - Applies reset, then reads 64 input samples from file "firData"
 *   - Traces signals into RESULT.vcd
 */

int sc_main(int argc, char* argv[]) {
  // Create signals
  sc_clock         clk("clk", 5, SC_NS);  // 200 MHz => 5 ns period
  sc_signal<bool>  rst("rst");
  sc_signal< sc_uint<32> > x_sig("x_sig");
  sc_signal< sc_uint<32> > y32_sig("y32_sig");
  sc_signal< sc_uint<32> > y48_sig("y48_sig");

  // Instantiate FIR modules
  FIR fir32("fir32", 32);
  fir32.clk(clk);
  fir32.rst(rst);
  fir32.x(x_sig);
  fir32.y(y32_sig);

  FIR fir48("fir48", 48);
  fir48.clk(clk);
  fir48.rst(rst);
  fir48.x(x_sig);
  fir48.y(y48_sig);

  // Create VCD waveform file
  sc_trace_file *tf = sc_create_vcd_trace_file("RESULT");
  tf->set_time_unit(1, SC_NS);

  // Trace signals in specified order
  sc_trace(tf, clk,    "clk");
  sc_trace(tf, rst,    "rst");
  sc_trace(tf, x_sig,  "x");
  sc_trace(tf, y32_sig,"y32");
  sc_trace(tf, y48_sig,"y48");

  // Open input file "firData" for reading
  std::ifstream inFile("firData");
  if (!inFile.is_open()) {
    std::cerr << "ERROR: Cannot open file 'firData'!\n";
    return -1;
  }

  // Assert reset (active-low => write false) for a few cycles
  rst.write(false);
  sc_start(10, SC_NS);  // 2 clock cycles at 5 ns each
  rst.write(true);

  // Now read 64 input data words from firData, feed them to both FIRs
  unsigned value = 0;
  for (int i = 0; i < 64; i++) {
    if (!(inFile >> value)) {
      std::cerr << "WARNING: Less than 64 values in 'firData'? Exiting.\n";
      break;
    }
    x_sig.write(value);
    sc_start(5, SC_NS); // Advance 1 clock cycle
  }

  // Let simulation run a bit longer so filters settle
  sc_start(200, SC_NS);

  // Close trace file and input file
  sc_close_vcd_trace_file(tf);
  inFile.close();

  return 0;
}

