SRCDIR = src
BINDIR = bin
OBJDIR = build
TARGET = $(BINDIR)ppm
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
CXX = g++
SRCS =	main.cpp\
		coordinate.cpp\
		atom.cpp\
		simulation.cpp\
		testbench.cpp
OBJS = $(SRCS:.cpp=.o)

CSRCS = $(addprefix $(SRCDIR),$(SRCS))
COBJS = $(addprefix $(OBJDIR),$(OBJS))
CTARGET = $(addprefix $(BINDIR),$(TARGET))

a: all
all: $(CTARGET)

$(TARGET): $(COBJS)
	$(CXX) $(LFLAGS) $(COBJS) -o $(CTARGET)
main.o: main.cpp testbench.h
	$(CXX) $(CFLAGS) main.cpp
coordinate.o: coordinate.cpp
	$(CXX) $(CFLAGS) coordinate.cpp
atom.o: atom.cpp atom.h coordinate.h
	$(CXX) $(CFLAGS) atom.cpp
simulation.o: simulation.cpp simulation.h atom.h
	$(CXX) $(CFLAGS) simulation.cpp
testbench.o: testbench.cpp testbench.h simulation.h coordinate.h
	$(CXX) $(CFLAGS) testbench.cpp
c: clean
clean:
	@echo cleaning...
	#rm -f *.o $(TARGET)	
	rm -rf $(OBJDIR) $(CTARGET)
r: remake
remake:
	make clean
	make all