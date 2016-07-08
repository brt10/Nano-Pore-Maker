TARGET = bhm
DEBUG = -g
LFLAGS = -Wall $(DEBUG)
CXX = g++
RM = rm

a: all
c: clean
r: remake
all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(LFLAGS) $(TARGET).cpp -o $(TARGET)
clean:
	$(RM) -f $(TARGET)
remake: clean all