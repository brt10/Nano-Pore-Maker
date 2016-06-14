using namespace std;	//for now... easier

#include <iostream>	//for cout
#include <fstream>	//for file io
#include <string>	//strings
#include <sstream>	//for stringstream	XXX only used in input file parsing
// #include "simulation.h"
// #include "coordinate.h"
#include "testbench.h"

int main()
{
	testbench test;
	test.Run("RUN.tsv");
	return 0;
}
