// timer.cpp
#include "timer.h"

void timer::runtimer() {
    // Reset behavior
    count = 0;
    timeout.write(0);
    
    while(true) {
        if (count == 0) {
            timeout.write(1);
        } else {
            count--;
            timeout.write(0);
        }

        // Synchronous reset check
        if (start.read()) {
            cout << "Timer: timer start detected" << endl;
            count = 5;
            timeout.write(0);
        }
        
        wait(); // Wait for next clock edge
    }
}
