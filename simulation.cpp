#include "simulation.h"
simulation::simulation()
{
	elementNum=0;
	for(int a=0; a< K::MAX_ELEMENTS; a++)
		elementCount[a] = 0;
	multiplier=1;
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
		//std::cout << atom_cls::lattice[a];
	}
	//read element names
	getline(infile,line);	
	ss<<line;
	std::cout << line<< endl;
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
//returns how many bonds were made
int simulation::Associate(void)
{
	unsigned int e[2];
	unsigned int i[2];
	atom_cls *atomP[2];
	unsigned int bondCount=0;
	for(e[0]=0; e[0] < elementNum; e[0]++)		//for every atom
		for(i[0]=0; i[0] < elementCount[e[0]]; i[0]++)	//
		{
			atomP[0] = &atom[e[0]][i[0]];		//make temporary pointer to atom
			if(!atomP[0]->exists) continue;	//make sure atom exists
			for(e[1]=e[0]; e[1]<elementNum; e[1]++)		//check only the atoms not yet analyzed.
				for(i[1]=i[0]+1; i[1]<elementCount[e[1]]; i[1]++)	//i[1]=i[0]+1 i OK b/c of the condition ;)
				{
					atomP[1] = &atom[e[1]][i[1]];	//create temporary pointer to atom
					if(!atomP[1]->exists) continue;	//make sure atom[] exists
					// if( (e[0] == e[1]) && (i[0] == i[1]) ) //if they are the same atom
					// 	continue;
					// for(int a=0; a<2; a++)	//create temporary variables (otherwise the names are super long.)
					// 	atomP[a] = &atom[e[a]][i[a]];
					if( atomP[0]->ModDistance(atomP[1]) <= K::BOND_LENGTH )	//if close enough
					{
						for(int a=0; a<2; a++)	//bond elements together
						{
							atomP[a]->bond[atomP[a]->bondNum] = atomP[(a+1)%2];
							atomP[a]->bondNum++;
						}
						bondCount++;
					}

				}
		}

	return bondCount;
}
//I decided to work backwards for this one... 
//find how close elements are to the hole from teh element's perspective instead of hole's
//returns the number of atoms removed
int simulation::Hole(coordinate h, double r)
{
	atom_cls hole;
	hole.co = h;
	unsigned int atomCount =0;
	for(int e=0; e < elementNum; e++)
		for(int i=0; i < elementCount[e]; i++)
			if( atom[e][i].ModDistance(&hole) < r && atom[e][i].exists)	//if in the hole and extant
			{
				atom[e][i].exists =  0;		//remove from existance... that was easy
				atomCount++;
				//make bonded atoms forget it.
				//alternatively, we could rely on the associate function... it ignores non-extant atoms
				//...but this is more efficient... slightly.
				for(int a=0; a<atom[e][i].bondNum; a++)
				{
					atom[e][i].bond[a]->BreakBond(&atom[e][i]);	//break the bond from the other side.
					atom[e][i].bond[a] = 0;	//nulify pointer
				}
				atom[e][i].bondNum = 0;	//reset bond number
				// while(atom[e][i].bondNum>0)
				// {
				// 	atom[e][i].bond = 0;	//nullify pointer
				// 	atom[e][i].bondNum--;	//decrease counter
				// }
			}
	return atomCount;
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
	//only write out atoms if they exist ;)

	outfile.close();
	return 1;
}
