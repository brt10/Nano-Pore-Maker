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

	// //prototypes
	// double ModDist(double[3], double[3]);	//returns the shortest distance between atoms (as a copied unit cell)
	// //constants
	// unsigned int MAX_ELEMENTS = 10;
	// unsigned int MAX_ATOMS = 1000;

	// //variables
	// ifstream infile;
	// ofstream outfile;
	// string filename[2];

	// double multiplier;
	// double lattice[3];
	// string element[MAX_ELEMENTS];		//names of elements
	// unsigned int amount[MAX_ELEMENTS];	//number of atoms per element
	


	//---------------------------
	//read in data from file
	//---------------------------
	//prompt for filenames
	/*do{
		cout << "Enter the filename to cut a hole in: "
		cin >> filename[0]
	}
	while(infile.fail());
	*/
	//for testing use:
	//filename[0] = "IN.txt";
	//read header

	//---------------------------