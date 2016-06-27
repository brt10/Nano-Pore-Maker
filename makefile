S = src
B = bin
O = build
TARGET = $(B)/pore
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -o $@
LFLAGS = -Wall $(DEBUG)
CXX = g++
OBJS =	$(O)/main.o\
		$(O)/coordinate.o\
		$(O)/atom.o\
		$(O)/simulation.o\
		$(O)/testbench.o
RM = rm
MKDIR = mkdir
#OBJS = $(SRCS:%.cpp=%.o)
#SRCS = $(addprefix $(S)/,$(SRCS))

a: all
c: clean
r: remake
all: build $(TARGET)

build:
	$(MKDIR) -p build
$(TARGET): $(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) -o $(TARGET)
$(O)/main.o:	$(S)/main.cpp\
				$(S)/testbench.o
	$(CXX) $(CFLAGS) $(S)/main.cpp
$(O)/coordinate.o:	$(S)/coordinate.cpp\
					$(S)/coordinate.h
	$(CXX) $(CFLAGS) $(S)/coordinate.cpp $(OUTFLAG)
$(O)/atom.o:	$(S)/atom.cpp\
				$(S)/atom.h\
				$(S)/coordinate.o\
				$(S)/K.h
	$(CXX) $(CFLAGS) $(S)/atom.cpp
$(O)/simulation.o:	$(S)/simulation.cpp\
					$(S)/simulation.h\
					$(S)/atom.o\
					$(S)/coordinate.o\
					$(S)/K.h
	$(CXX) $(CFLAGS) $(S)/simulation.cpp
$(O)/testbench.o:	$(S)/testbench.cpp\
					$(S)/testbench.h\
					$(S)/simulation.o\
					$(S)/coordinate.o
	$(CXX) $(CFLAGS) $(S)/testbench.cpp
clean:
	$(RM) -f $(O)/* $(TARGET)
remake: clean all