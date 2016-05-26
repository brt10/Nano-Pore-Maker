using namespace std;	//for now... easier

#include <iostream>	//for cout
#include <string>	//strings
#include "simulation.h"
#include "coordinate.h"

int main()
{
	//variables
	simulation sim;		//simulation object
	//string scaleFilename;
	string dataFilename = "data/64.vasp";
	string outFilename = "data/OUT.vasp";
	double holeRadius;
	// double cell[3] = {.25,.5,.5};
	// unsigned int scale[3] = {8,4,4};		//the scales to output... use scalefile later.

	for(int a=0; a<6; a++)
	{
		holeRadius = a;
		sim << dataFilename;
		sim.Standardize();
		sim.Associate();
		//make hole at rough center on a Si atom
		cout << sim.PassivatedHole(holeRadius, sim.Center(0));
		cout << " atoms removed.\n";
		sim >> outFilename;
		
		for(int e=0; e<sim.elementNum; e++)
			cout << sim%e << "% " << sim.element[e] << "\n";
		cout << "Density: " << sim.Density() << " g/cm^3\n";
	}

	return 0;
}