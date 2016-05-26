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
	string dataFilename;
	string outFilename;
	coordinate hole;
	double holeRadius;
	double cell[3] = {.25,.5,.5};
	unsigned int scale[3] = {8,4,4};		//the scales to output... use scalefile later.

	//assignments
	//scaleFilename = "data/scalefile.txt";
	hole = .5;
	hole.ord[0] = 0;
	holeRadius = 5;
	dataFilename = "data/64.vasp";
	outFilename = "data/OUT.vasp";

	sim << dataFilename;
	sim.Standardize();
	sim.Scale(cell);
	sim.Scale(scale);
	// sim.Disassociate();
	sim.Associate();
	
	// int e=0;
	// int i=27;
	// coordinate x = sim.atom[e][i].co;
	// coordinate y;
	// cout << "1\t" << sim.atom[e][i].bondNum << "\n\t";
	// for(int a=0; a<3; a++)
	// 	cout << x[a] << '\t';
	// 	// cout << .5 << '\t';
	// cout << "TTT\n";  
	// for(int a=0; a<sim.atom[e][i].bondNum; a++)
	// {
	// 	y = sim.atom[e][i].bond[a]->co;
	// 	// y = (x-y)+.5;
	// 	// y.Dec();
	// 	cout << "\t";
	// 	for(int b=0; b<3; b++)
	// 		cout << y[b] << '\t';
	// 	// cout << sim.ModDistance(&sim.atom[e][i], sim.atom[e][i].bond[a]);
	// 	cout << "TTT\n";
	// }
	//-----------------
	// coordinate x,y,z;
	// x=0;
	// y=.5;
	// z=y*sim.lattice;
	// cout << sim.ModDistance(x,y) << " A\n";
	// for(int a=0; a<3; a++)
	// 	cout << sim.lattice[a] << "\t";
	// cout << '\n';
	//------------------
	// int count=0;
	// for(int e=0; e<sim.elementNum; e++)
	// 	for(int i=0; i<sim.elementCount[e]; i++)
	// 		if(sim.atom[e][i].bondNum ==4)
	// 			count++;
	// 		else
	// 			cout << e << i << ' ' << sim.atom[e][i].bondNum << '\n';
	// cout << count << " of " << sim.Atoms() << " atoms are OK\n";
	// cout << sim.Hole(hole,holeRadius);
	cout << sim.PassivatedHole(sim.atom[0][203].co, 6, &sim.atom[0][203]);
	cout << " atoms removed.\n";
	cout << sim.Trim() << " atoms trimmed";
	sim >> outFilename;
	// for(int e=0; e<sim.elementNum; e++)
	// 	cout << sim%e << "% " << sim.element[e] << "\n";
	// cout << "Density: " << sim.Density() << " g/cm^3\n";
	// cout << K::MASS[0] << endl;
	//sim.Scale(outFilename, scaleFilename);
	return 0;
}