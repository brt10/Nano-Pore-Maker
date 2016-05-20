using namespace std;	//for now... easier

#include <iostream>	//for cout
// #include <fstream>	//file io
#include <string>	//strings
// #include <cmath>	//sqrt(), pow()
#include "simulation.h"
#include "coordinate.h"

int main()
{
	simulation sim;		//simulation object
	string filename[2];	//io filenames
	coordinate hole;	//hole coords	(may remove coordinate struct...)
	double holeRadius;	//radius of hole...
	double cell[3] = {.25, .5, .5};	//a particular unit cell (testing)
	unsigned int scale[3] = {7,4,4};		//a particualr scale for tseting 

	//temporary for testing
		holeRadius = 5.4;	//in angstroms
		// for(int a=0; a<3; a++)
		// 	hole.ord[a] = .5;
		hole = .5;	//made an operator for this. :D
		filename[0] = "IN.txt";
		filename[1] = "OUT.txt";
	
	//read data from file----------
	//sim.ReadData( filename[0] );
	sim << filename[0];

	//standardize the data
	sim.Standardize();

	//scale model(copies of particular unit cell)
	sim.Scale(cell);
	sim.Scale(scale);

	//outut fancy data
	for(int a=0; a<sim.elementNum; a++)
		cout << sim%a << "% " << sim.element[a] << "\n";
	cout << "Density: " << sim.Density() << endl;

	//find neighbors---------------
	// cout << sim.Associate();
	// cout << " Atoms bonded\n";

	//Create Hole-------------------
	// cout << sim.Hole(hole, holeRadius);
	// cout << " Atoms removed when making hole\n";

	//output file--------------------
	// sim.WriteData( filename[1] );
	sim >> filename[1];
	return 0;
}