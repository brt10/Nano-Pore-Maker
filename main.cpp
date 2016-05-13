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
		holeRadius = .25;
		for(int a=0; a<3; a++)
			hole.ord[a] = .5;
		filename[0] = "IN.txt";	//for now dont prompt
		filename[1] = "OUT.txt";
	
	//read data from file
	sim.ReadData( filename[0] );
	
	//find neighbors
	sim.Associate();
	
	//Create Hole
	sim.Hole(hole);

	//output file
	sim.WriteData( filename[1] );
	return 0;
}