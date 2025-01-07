#ifndef FIR_H
#define FIR_H
#include <systemc.h>

/*
 * The FIR module implements an any-order FIR filter in a single SC_CTHREAD.
 *
 * Ports:
 *   - clk : clock (positive edge)
 *   - rst : synchronous, active-low reset
 *   - x   : sc_uint<32> input
 *   - y   : sc_uint<32> output
 *
 * The constructor argument 'tap' sets how many taps the filter has.
 * The default constructor is 16 taps.
 */

SC_MODULE(FIR) {
  // Ports
  sc_in<bool> clk;
  sc_in<bool> rst;
  sc_in< sc_uint<32> > x;
  sc_out< sc_uint<32> > y;

  // Number of taps
  unsigned int tap;

  // Coefficients and delay line storage
  sc_uint<32>* b;         // b[i] holds the 32-bit fixed-point coeff for tap i
  sc_uint<32>* delayLine; // holds the last 'tap' input samples in 32-bit

  // The main FIR process
  void fir_thread();

  // Default constructor => 16 taps
  SC_CTOR(FIR) : tap(16) {
    b          = new sc_uint<32>[tap];
    delayLine  = new sc_uint<32>[tap];

    SC_CTHREAD(fir_thread, clk.pos());
    // Synchronous, active-low reset
    reset_signal_is(rst, /*active level*/ false);
  }

  // Overloaded constructor => user-specified taps
  FIR(sc_module_name name, unsigned int t) : sc_module(name), tap(t) {
    b          = new sc_uint<32>[tap];
    delayLine  = new sc_uint<32>[tap];

    SC_CTHREAD(fir_thread, clk.pos());
    // Synchronous, active-low reset
    reset_signal_is(rst, /*active level*/ false);
  }

  // Destructor
  ~FIR() {
    delete [] b;
    delete [] delayLine;
  }
};

#endif

