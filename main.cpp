using namespace std;	//for now... easier

#include <iostream>	//for cout
#include <fstream>	//for file io
#include <string>	//strings
#include <sstream>	//for stringstream	XXX only used in input file parsing
// #include "simulation.h"
// #include "coordinate.h"
// #include "testbench.h"

int main()
{
	string text = "2	4	8";
	stringstream ss;
	int scale;

	ss << text;
	for(int a=0; a<3; a++)
	{
		ss >> scale;
		cout << scale << endl;
	}
	// ss.str("");
	ss.clear();
	ss << text;
	for(int a=0; a<4; a++)
	{
		ss >> scale;
		cout << scale << endl;
	}

	
}
