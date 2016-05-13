#include "simulation.h"
simulation::simulation()
{
}
bool simulation::ReadData(string filename)
{
	//variables
	stringstream ss;
	string line;
	char freedomChar;
	ifstream infile;
	infile.open(filename.c_str());

	elementNum = 0;	//from simulation

	if(infile.fail()){
		cerr << "Failed to open: \"" << filename << "\"\n";
		return 0;
	}
	//read from file...
	infile >> multiplier;
	for(int a=0; a<3; a++)
	{
		// infile >> this->lattice[a];
		// this->atom[0][0].lattice[a] = this->lattice[a];	//set static 
		infile >> atom_cls::lattice[a];
	}
	//read element names
	getline(infile,line);	
	ss<<line;
	while(ss >> element[elementNum])
		elementNum++;
	for(int a=0; a<elementNum; a++)
		infile >> elementCount[a];
	//read atom data
	for(int e=0; e<elementNum; e++)			//element
		for(int i=0; i<elementCount[e]; i++)	//index
		{
			atom[e][i].element = e;
			for(int c=0; c<3; c++)			//read coord
				infile >> atom[e][i].co.ord[c];
			for(int c=0; c<3; c++)			//read freedom
			{
				infile >> freedomChar;
				atom[e][i].freedom[c] = (freedomChar=='T' || freedomChar=='t');
			}
		}
	infile.close();

	//send lattice data
	return 1;
}
int simulation::Associate(void)
{
	return 0;
}
int simulation::Hole(coordinate h)
{
	return 0;
}
bool simulation::WriteData(string filename)
{
	ofstream outfile;
	outfile.open(filename.c_str());

	if(outfile.fail()){
		cerr << "Failed to open: \"" << filename << "\"\n";
		return 0;
	}
	//write out to file...

	outfile.close();
	return 1;
}
