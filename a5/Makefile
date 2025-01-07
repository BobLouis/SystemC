###############################################################################
# Makefile for FIR project using SystemC
###############################################################################

SYSTEMC_DIR = $(SYSTEMC_HOME)
PROJECT     = fir

# Compiler and flags
CXX         = g++ -std=c++17
# We add -DSC_INCLUDE_FX so older SystemC versions pull in sc_fixed
BUILDFLAGS  = -g3 -DSC_INCLUDE_FX
INCFLAGS    = -I. -I$(SYSTEMC_DIR)/include
LDFLAGS     = -L$(SYSTEMC_DIR)/lib-linux64 \
              -Wl,-rpath,$(SYSTEMC_DIR)/lib-linux64 \
              -lsystemc -lm

# Source files
SRC_CPPHEAD = FIR       # Means FIR.cpp + FIR.h
SRC_CPP     =
HEADERS     = FIR.h
MAIN        = main.cpp
OBJECTS     = $(SRC_CPPHEAD:=.o) $(SRC_CPP:=.o)
EXE         = $(PROJECT)

###############################################################################
# Build rules
###############################################################################

all: $(EXE)

$(EXE): $(MAIN) $(OBJECTS) $(HEADERS)
	@echo "$@ building..."
	$(CXX) $(BUILDFLAGS) $(INCFLAGS) $(MAIN) $(OBJECTS) $(LDFLAGS) -o $@
	@echo ""
	@echo "$@ build done successfully..."
	@echo ""

%.o: %.cpp %.h
	@echo "Compiling $< ..."
	$(CXX) $(BUILDFLAGS) -c $< $(INCFLAGS)

clean:
	rm -f $(EXE)
	rm -f *.o
	rm -f *.vcd

