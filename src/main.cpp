using namespace std;	//for now... easier

#include <iostream>	//for cout
#include <fstream>	//for file io
#include <string>	//strings
#include <sstream>	//for stringstream	XXX only used in input file parsing
#include "simulation.h"
// #include "coordinate.h"
#include "testbench.h"

int main(int argc, char *argv[])
{
	cout << "TEST" << endl;
	/*double scale[] = {.25,.5,.5};
	simulation sim;
	sim << "data/2H.vasp";
	sim.Standardize();
	sim.Scale(scale);
	sim >> "data/2H_UNIT.vasp";*/
	testbench test;
	if(argc < 2)
	{
		cout << "Please supply a command line argument!\nex: \"./ppm INPUT.tsv\"\n\n";
		return 1;
	}
	string filename(argv[1]);
	test.Run(filename);
	return 0;
}
