out = n
name = nate

all: $(name)

$(name): main.o coordinate.o simulation.o atom.o 
	g++ main.o coordinate.o simulation.o atom.o -o $(name)
main.o: main.cpp
	g++ -c main.cpp
coordinate.o: coordinate.cpp
	g++ -c coordinate.cpp
atom.o: atom.cpp
	g++ -c atom.cpp
simulation.o: simulation.cpp
	g++ -c simulation.cpp
clean:
	rm $(out)
purge: #clean
	rm *.o $(out)