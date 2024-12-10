#include <systemc.h>
#include "timer.h"

int sc_main(int argc, char* argv[]) {
    // 信號宣告
    sc_signal<bool> clock;
    sc_signal<bool> start;
    sc_signal<bool> timeout;

    // Timer 模組例項化
    timer t("Timer");
    t.clock(clock);
    t.start(start);
    t.timeout(timeout);

    // 建立 VCD Trace File
    sc_trace_file* tf = sc_create_vcd_trace_file("RESULT");
    sc_trace(tf, clock, "clock");
    sc_trace(tf, start, "start");
    sc_trace(tf, timeout, "timeout");
    sc_trace(tf, t.count, "count");

    // 時鐘產生過程 (100MHz = 10ns)
    sc_time clk_period(10, SC_NS);
    sc_start(0, SC_NS);

    // 產生時鐘波形
    sc_spawn([&]() {
        while (true) {
            clock.write(false);
            wait(clk_period / 2);
            clock.write(true);
            wait(clk_period / 2);
        }
    });

    // 測試過程
    // 1. Reset timer for 3 cycles before counting
    start.write(0);
    sc_start(30, SC_NS); // 等待 3 個時鐘週期 (3 x 10ns = 30ns)

    // 2. Release and start counting
    start.write(1);
    sc_start(60, SC_NS); // 等待 6 個時鐘週期 (6 x 10ns = 60ns)

    // 3. Reset timer during counting before count reaches 0
    start.write(0);
    sc_start(30, SC_NS); // 等待 3 個時鐘週期 (3 x 10ns = 30ns)

    // 4. Release timer again
    start.write(1);
    sc_start(100, SC_NS); // 等待 10 個時鐘週期 (10 x 10ns = 100ns)

    // 5. Reset timer after count reaches 0
    sc_start(30, SC_NS); // 等待多一些時間後再 reset
    start.write(0);
    sc_start(30, SC_NS); // 等待 3 個時鐘週期 (3 x 10ns = 30ns)

    // 關閉 Trace File
    sc_close_vcd_trace_file(tf);

    return 0;
}

