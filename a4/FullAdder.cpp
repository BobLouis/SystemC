// FullAdder.cpp
#include "FullAdder.h"

void FullAdder::process() {
    Cout.write(c1.read() | c2.read());
}
