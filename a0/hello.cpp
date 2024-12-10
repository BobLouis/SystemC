#include <systemc.h>
#include "timer.h"

// 時鐘生成函數
void clock_generator(sc_signal<bool>& clock, sc_time clk_period) {
    while (true) {
        clock.write(false);
        wait(clk_period / 2);
        clock.write(true);
        wait(clk_period / 2);
    }
}

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

    // 時鐘週期 (100MHz = 10ns)
    sc_time clk_period(10, SC_NS);

    // 時鐘生成過程
    sc_spawn([&]() { clock_generator(clock, clk_period); });

    // 測試過程
    start.write(0);       // 1. Reset timer
    sc_start(30, SC_NS);  // 等待 3 個週期

    start.write(1);       // 2. Release and start counting
    sc_start(60, SC_NS);  // 等待 6 個週期

    start.write(0);       // 3. Reset during counting
    sc_start(30, SC_NS);  // 等待 3 個週期

    start.write(1);       // 4. Release again
    sc_start(100, SC_NS); // 等待 10 個週期

    start.write(0);       // 5. Reset after count reaches 0
    sc_start(30, SC_NS);  // 等待 3 個週期

    // 關閉 Trace File
    sc_close_vcd_trace_file(tf);

    return 0;
}

