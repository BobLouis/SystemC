SYSTEMC_HOME = /opt/systemc-3.0.1
CXX = g++
CXXFLAGS = -std=c++17 -I$(SYSTEMC_HOME)/include -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc -Wl,-rpath,$(SYSTEMC_HOME)/lib-linux64


TARGET = hello
SOURCES = timer.cpp hello.cpp

all:
	$(CXX) $(SOURCES) -o $(TARGET) $(CXXFLAGS)

clean:
	rm -f $(TARGET) RESULT.vcd

