SYSTEMC_DIR = ${SYSTEMC_HOME}
PROJECT     = adder
BUILDFLAGS  = -g3
CXX         = g++ -std=c++17
INCFLAGS    = -I. -I${SYSTEMC_DIR}/include
LDFLAGS     = -L${SYSTEMC_DIR}/lib-linux64 -Wl,-rpath,${SYSTEMC_DIR}/lib-linux64 -lsystemc -lm

# Source files
SRC_CPPHEAD = HalfAdder
SRC_CPP     = FullAdder
HEADERS     = HalfAdder.h FullAdder.h
MAIN        = main.cpp
OBJECTS     = $(SRC_CPPHEAD:=.o) $(SRC_CPP:=.o)
EXE         = $(PROJECT)

all: $(EXE)

$(EXE): $(MAIN) $(OBJECTS) $(HEADERS)
	@echo "$@ building..."
	$(CXX) $(INCFLAGS) $(MAIN) $(OBJECTS) $(LDFLAGS) -o $@
	@echo ""
	@echo "$@ build done successfully..."
	@echo ""

%.o:%.cpp %.h
	@echo "Compiling $< ..."
	$(CXX) -c $< $(INCFLAGS)

clean:
	rm -f $(EXE)
	rm -f *.o
	rm -f *.vcd
