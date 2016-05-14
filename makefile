out = n
name = nate
dbg = -g

all: $(name)

$(name): main.o coordinate.o atom.o simulation.o
	g++ $(dbg) main.o coordinate.o atom.o simulation.o -o $(out)
main.o: main.cpp
	g++ -c $(dbg) main.cpp
coordinate.o: coordinate.cpp
	g++ -c $(dbg) coordinate.cpp
atom.o: atom.cpp
	g++ -c $(dbg) atom.cpp
simulation.o: simulation.cpp
	g++ -c $(dbg) simulation.cpp
clean:
	rm $(out)
purge: #clean
	rm *.o $(out)
#debug: main.dbg coordinate.dbg atom.dbg simulation.dbg
#	g++ main.dbg coordinate.dbg atom.dbg simulation.dbg -o 