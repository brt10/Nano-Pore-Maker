out = ppm
name = nate
dbg = -g

a: all
all: $(name)

$(name): main.o coordinate.o atom.o simulation.o testbench.o
	g++ $(dbg) main.o coordinate.o atom.o simulation.o testbench.o -o $(out)
main.o: main.cpp
	g++ -c $(dbg) main.cpp
coordinate.o: coordinate.cpp
	g++ -c $(dbg) coordinate.cpp
atom.o: atom.cpp
	g++ -c $(dbg) atom.cpp
simulation.o: simulation.cpp
	g++ -c $(dbg) simulation.cpp
testbench.0: testbench.cpp
	g++ -c $(dbg) testbench.cpp
c: clean
clean:
	rm $(out)
p: purge
purge: #clean
	rm *.o $(out)
r: remake
remake:
	make purge
	make all
#debug: main.dbg coordinate.dbg atom.dbg simulation.dbg
#	g++ main.dbg coordinate.dbg atom.dbg simulation.dbg -o 