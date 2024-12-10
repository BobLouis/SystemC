#ifndef TIMER_H
#define TIMER_H

#include <systemc.h>

SC_MODULE(timer) {
    sc_in<bool> clock;      // 時鐘信號
    sc_in<bool> start;      // 啟動信號
    sc_out<bool> timeout;   // 超時信號
    sc_signal<int> count;   // 計數器

    SC_CTOR(timer) {
        SC_METHOD(process);
        sensitive << clock.pos();
        timeout.initialize(false);
    }

    void process() {
        if (start.read() == 0) {
            count.write(10);  // 預設計數器值
            timeout.write(false);
        } else {
            int current_count = count.read();
            if (current_count > 0) {
                count.write(current_count - 1);
                timeout.write(false);
            } else {
                timeout.write(true);
            }
        }
    }
};

#endif

