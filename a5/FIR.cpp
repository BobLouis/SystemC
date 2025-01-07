#include "FIR.h"

/*
 * FIR::fir_thread()
 *   - On reset (rst==false), clear delay line and compute b[i].
 *   - Otherwise, every clock, shift the delay line, multiply-accumulate,
 *     and write the output.
 */

void FIR::fir_thread() {

  // We stay in this infinite loop.  SC_CTHREAD requires wait() exactly once.
  while (true) {
    // Synchronous reset behavior
    if (rst.read() == false) {
      // Clear delay line
      for (unsigned i = 0; i < tap; i++) {
        delayLine[i] = 0;
      }

      // Compute each coefficient b[i] = 1/(tap+1), in 32-bit fixed point
      // sc_fixed<wl=32, iwl=16>, then copy bit-by-bit to sc_uint<32>.
      for (unsigned i = 0; i < tap; i++) {
        sc_fixed<32, 16> fb;
        fb = (float)1.0f / (float)(tap + 1);

        sc_uint<32> coeff;
        for (int bit = 0; bit < 32; bit++) {
          coeff[bit] = fb[bit];  // copy each bit
        }
        b[i] = coeff;
      }

      // Output 0 while in reset
      y.write(0);

      // Wait for next clock
      wait();

    } else {
      // Normal operation:
      // 1) Shift the delay line
      for (int i = tap - 1; i > 0; i--) {
        delayLine[i] = delayLine[i - 1];
      }
      delayLine[0] = x.read();

      // 2) Multiply-accumulate
      //    We'll use a bigger accumulator type to avoid overflow.
      sc_fixed<64, 32> acc = 0;  // 64 bits total, 32 integer bits

      for (unsigned i = 0; i < tap; i++) {
        // Turn delayLine[i] into sc_fixed<32,16>
        sc_fixed<32,16> sample_fixed;
        for (int bit = 0; bit < 32; bit++) {
          sample_fixed[bit] = delayLine[i][bit];
        }

        // Turn b[i] into sc_fixed<32,16>
        sc_fixed<32,16> coeff_fixed;
        for (int bit = 0; bit < 32; bit++) {
          coeff_fixed[bit] = b[i][bit];
        }

        acc += (sample_fixed * coeff_fixed);
      }

      // 3) Convert accumulator back to sc_uint<32>
      sc_uint<32> outVal;
      for (int bit = 0; bit < 32; bit++) {
        outVal[bit] = acc[bit];  // copy bit-by-bit
      }

      // 4) Write output
      y.write(outVal);

      // Wait for next clock
      wait();
    }
  } // while(true)
}

