using namespace std;	//for now... easier

#include <iostream>	//for cout
// #include <fstream>	//file io
#include <string>	//strings
// #include <cmath>	//sqrt(), pow()
#include "simulation.h"
#include "coordinate.h"

int main()
{
	simulation sim;
	string filename[2];
	coordinate hole;
	double holeRadius;

	//temporary for testing
		holeRadius = .5;	//in angstroms
		for(int a=0; a<3; a++)
			hole.ord[a] = .5;
		filename[0] = "IN.txt";
		filename[1] = "OUT.txt";
	
	//read data from file
	sim.ReadData( filename[0] );
	//find neighbors
	cout << sim.Associate();
	cout << " Atoms bonded\n";
	//Create Hole
	cout << sim.Hole(hole, holeRadius);
	cout << " Atoms removed when making hole\n";

	cout << atom_cls::lattice[2] << endl;
	cout << sim.element[0] << endl;

	for(int e=0; e< sim.elementNum; e++)
		cout << e << " : " << sim.element[e] << endl;

	//output file
	sim.WriteData( filename[1] );
	return 0;
}