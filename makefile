S = src
B = bin
O = build
TARGET = $(B)/pore
DEBUG = -g
CFLAGS = -std=c++11 -Wall -c $(DEBUG) -o $@
LFLAGS = -std=c++11 -Wall $(DEBUG)
CXX = g++
OBJS =	$(O)/main.o\
		$(O)/coordinate.o\
		$(O)/atom.o\
		$(O)/simulation.o\
		$(O)/testbench.o\
		$(O)/strops.o\
		$(O)/K.o
RM = rm
MKDIR = mkdir
#OBJS = $(SRCS:%.cpp=%.o)
#SRCS = $(addprefix $(S)/,$(SRCS))

a: all
c: clean
r: remake
all: $(O) $(B) $(TARGET)

$(O):
	$(MKDIR) -p $(O)
$(B):
	$(MKDIR) -p $(B)
$(TARGET): $(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) -o $(TARGET)
$(O)/main.o:	$(S)/main.cpp\
				$(O)/testbench.o
	$(CXX) $(CFLAGS) $(S)/main.cpp
$(O)/coordinate.o:	$(S)/coordinate.cpp\
					$(S)/coordinate.h
	$(CXX) $(CFLAGS) $(S)/coordinate.cpp $(OUTFLAG)
$(O)/atom.o:	$(S)/atom.cpp\
				$(S)/atom.h\
				$(O)/coordinate.o\
				$(S)/K.h
	$(CXX) $(CFLAGS) $(S)/atom.cpp
$(O)/simulation.o:	$(S)/simulation.cpp\
					$(S)/simulation.h\
					$(O)/atom.o\
					$(O)/coordinate.o\
					$(S)/K.h\
					$(O)/strops.o
	$(CXX) $(CFLAGS) $(S)/simulation.cpp
$(O)/testbench.o:	$(S)/testbench.cpp\
					$(S)/testbench.h\
					$(O)/simulation.o\
					$(O)/coordinate.o\
					$(O)/strops.o
	$(CXX) $(CFLAGS) $(S)/testbench.cpp
$(O)/strops.o:	$(S)/strops.cpp\
				$(S)/strops.h
	$(CXX) $(CFLAGS) $(S)/strops.cpp
$(O)/K.o:	$(S)/K.cpp\
			$(S)/K.h\
			$(O)/strops.o
	$(CXX) $(CFLAGS) $(S)/K.cpp

clean:
	$(RM) -f $(O)/* $(TARGET)
remake: clean all