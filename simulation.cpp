#include "simulation.h"
// double simulation::latice[3] = {0};
simulation::simulation()
{
	ClearData();
}
void simulation::ClearData(void)
{
	elementNum=0;
	for(int a=0; a< K::MAX_ELEMENTS; a++)
		elementCount[a] = 0;
	multiplier=1;
	return;
}
bool simulation::ReadData(const string filename)
{
	//variables
	stringstream ss;
	string line;
	char freedomChar;
	ifstream infile;
	infile.open(filename.c_str());

	//elementNum = 0;	//from simulation
	//reset class
	ClearData();
	//reset atoms
	for(int e=0; e<K::MAX_ELEMENTS; e++)
		for(int i=0; i<K::MAX_ATOMS; i++)
			atom[e][i].ClearData();

	if(infile.fail()){
		cerr << "Failed to open: \"" << filename << "\"\n";
		return 0;
	}
	//read from file...
	//infile >> title;
	getline(infile, title);
	infile >> multiplier;
	for(int a=0; a<3; a++)
		for(int b=0; b<3; b++)
		{
			if(a==b) infile >> lattice[a];
			else infile >> lattice[2];	//essentially junk... works, just ugly. :P
		}
	//read element names------------
	// do{
	// 	infile >> elementChar;

	// 	if(elementChar>=A && elementChar<=Z && elementChar>=a && elementChar<=z)
	// 	{
	// 		elementNum
	// 	}
	// 	while(elementChar!=)
	// 	{
	// 		element[elementNum]+=elementChar;
	// 		infile >> elementChar;
	// 	}
	// } while((int)elementChar != 13)
	do{
		getline(infile,line);
	}while(line.length()<=1/* && (int)line[0] <65*/);	// XXX NEED FIX SOON!damn this is ugly
	// // getline(infile,line);
	// // getline(infile,line);
	ss<<line;
	// std::cout << "line length: "<< line.length() << "\n";
	// for(int a=0; a<line.length(); a++)
	// {
	// 	cout << "char " << a << " : \"" << (int)line[a] << "\"\n";
	// }
	// std::cout << "line contents: \""<< line << "\"\n";
	while(ss >> element[elementNum])
		elementNum++;
	// cout << "ELEMENT NUM: " << elementNum << endl;
	for(int a=0; a<elementNum; a++)
		infile >> elementCount[a];
	infile >> tag;

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
bool simulation::operator<<(const string filename)
{
	return ReadData(filename);
}
void simulation::Standardize(void)
{
	for(int e=0; e<elementNum; e++)
		for(int i=0; i<elementCount[e]; i++)
			atom[e][i].co.Mod(1);	//XXX should probaly check for overlap with others...
	return;
}
//returns how many bonds were made
void simulation::Disassociate(void)
{
	for(int e=0; e < elementNum; e++)			//for every atom
		for(int i=0; i < elementCount[e]; i++)	//
			for(int b=0; b<K::MAX_BONDS; b++)
			{
				atom[e][i].bondNum = 0;
				atom[e][i].bond[b] = 0;
			}
	return;
}
int simulation::Associate(void)	//XXX make sure it deals with previous bonds!
{
	unsigned int e[2];
	unsigned int i[2];
	atom_cls *atomP[2];
	unsigned int bondCount=0;
	for(e[0]=0; e[0] < elementNum; e[0]++)				//for every atom
		for(i[0]=0; i[0] < elementCount[e[0]]; i[0]++)	//
		{
			atomP[0] = &atom[e[0]][i[0]];		//make temporary pointer to atom
			if(!atomP[0]->exists) continue;		//make sure atom exists
			for(e[1]=e[0]; e[1]<elementNum; e[1]++)					//check only the atoms not yet analyzed.
				for(i[1]= (e[0]==e[1] ? i[0]+1 : 0); i[1]<elementCount[e[1]]; i[1]++)	//i[1]=i[0]+1 i OK b/c of the condition ;)
				{
					// cout << e[0] << ',' << e[1] << '\t' << i[0] << ',' << i[1] << '\n';
					atomP[1] = &atom[e[1]][i[1]];	//create temporary pointer to atom
					if(!atomP[1]->exists) continue;	//make sure atom[] exists						
					if( ModDistance(atomP[0], atomP[1]) <= K::BOND_LENGTH[atomP[0]->element][atomP[1]->element]*K::BOND_DEVIATION )	//if close enough
					{
						if(Bond(atomP))
							bondCount++;
					}

				}
		}

	return bondCount;
}
bool simulation::Bond(atom_cls* aP0, atom_cls* aP1)
{
	atom_cls* atomP[2] = {aP0, aP1};
	return Bond(atomP);
}
bool simulation::Bond(atom_cls* atomP[2])
{
	for(int a=0; a<2; a++)	//bond elements together	//XXX shoudl check if already bound
	{
		int b=(a+1)%2;	//other atom
		if(atomP[a]->IsBound(atomP[b]) != -1) continue;	//if already bound
		if(atomP[a]->bondNum < K::MAX_BONDS)
		{
			atomP[a]->bond[atomP[a]->bondNum] = atomP[b];
			atomP[a]->bondNum++;
		} else {
			cerr << "The bonds on atom (" /*<< e[a] << "," << i[a]*/;
			cerr << ") have exceeded the limit of: ";
			cerr << K::MAX_BONDS << "\n";
			cerr << "THIS IS LIKELY A CRITICAL ERROR!\n";
			cerr << "Most likely the lattice is too small, or uses different units than the bond lengths\n";
			return 0;
		}
	}
	return 1;
}
//returns the number of atoms removed
int simulation::Hole(coordinate h, double r)
{
	unsigned int atomCount =0;
	for(int e=0; e < elementNum; e++)			//for each atom
		for(int i=0; i < elementCount[e];)	//
			if( ModDistance(h, atom[e][i].co) < r && atom[e][i].exists)	//if in the hole and extant
			{
				RemoveAtom(e,i);
				atomCount++;
			} else i++;
	return atomCount;
}
void simulation::Passivate(atom_cls* removed, atom_cls* passivated)	//assume to passivate with Hydrogen
{
	atom_cls* H;	//Hydrogen atom pointer
	coordinate r,p;	//coordinates of atoms
	double ratio;	//ratio of length

	//initializers
	r=removed->co;
	p=passivated->co;
	ratio = K::BOND_LENGTH[2][passivated->element] / ModDistance(r, p);
	//XXX assume third element is Hydrogen. O.o MUST rewrite later, but looking for better way than searching for elements each time... use enum?
	if(elementNum<3)
	{
		elementNum++;
		elementCount[2] = 0;
		element[2] = "H";
	}
	H = &atom[2][elementCount[2]];
	H->ClearData();
	H->exists=1;
	// H->co = (((r-p)*ratio)+p);	//XXX this is wrong because the distance is not r-p!
	H->co = ((r-p)+.5);	//center around .5 to use Dec()
	H->co.Dec();		//Modulus 1
	H->co -= .5;		//center around 0
	H->co *= ratio;		//fit to new length
	H->co += p;			//center around p
	H->co.Dec();		//Modulus 1
	H->bond[0] = passivated;
	//XXX should check to make sure doesnt exceed max bonds: alternatively replace old bond...
	//bond passivated to Hydrogen
	passivated->bond[passivated->bondNum] = H;
	passivated->bondNum++;
	elementCount[2]++;
	return;
}
bool simulation::WriteData(const string filename)
{
	//variables
	ofstream outfile;
	outfile.open(filename.c_str());

	//set precision
	outfile << fixed;
	outfile << setprecision(K::PRECISION);

	if(outfile.fail()){
		cerr << "Failed to open: \"" << filename << "\"\n";
		return 0;
	}
	//write out to file...
	outfile << title << "\n";
	outfile << multiplier << "\n";
	//outfile << "\n";	//write a newline (do not use endl (forces flush))
	for(int a=0; a<3; a++)
	{
		for(int b=0; b<3; b++)
			outfile << ((a==b) ? lattice[a] : 0) << "\t";
		outfile << "\n";
	}
	for(int e=0; e<elementNum; e++)	//element names
		outfile << element[e] << "\t";
	outfile << "\n";
	for(int e=0; e<elementNum; e++)	//# extant atoms
		outfile << Extant(e) << '\t';
	outfile << "\n" << tag << "\n";

	//only write out atoms if they exist ;)
	for(int e=0; e<elementNum; e++)
		for(int i=0; i<elementCount[e]; i++)
			if(atom[e][i].exists)
			{
				for(int a=0; a<3; a++)
					outfile << atom[e][i].co.ord[a] << "\t";
				for(int a=0; a<3; a++)
					outfile << (atom[e][i].freedom[a] ? "T " : "F ");
				outfile << "\n";	//write a newline (do not use endl (forces flush))
			}

	outfile.close();
	return 1;
}
bool simulation::operator>>(const string filename)
{
	return WriteData(filename);
}
// int simulation::UnitCell(double scale[3])		//makes the scale given, and makes the atoms within it into a unit cell (returns #atoms in cell)
	// {
	// 	unsigned int count = 0; //# atoms in cell
	// 	//bool inside=1; 	//if atom is inside.

	// 	for(int e=0; e < elementNum; e++)			//for every atom
	// 		for(int i=0; i < elementCount[e]; i++)	//
	// 			if(atom[e][i].exists)
	// 			{
	// 				//inside = 1;
	// 				for(int a=0; a<3; a++)
	// 					if(atom[e][i].co.ord[a] > scale[a])
	// 					{
	// 						atom[e][i].exists = 0;
	// 						break;
	// 					}
	// 				//atom[e][i].exists = inside;
	// 				count += atom[e][i].exists;
	// 				elementCount[e] -= atom[e][i].exists;

	// 					// if(atom[e][i].co.ord[a] <= scale[a]) {
	// 					// 	count++;	//do not set exists to true here... I bet bad things would happen... ;)
	// 					// } else {
	// 					// 	atom[e][i].exists = 0;
	// 					// 	elementCount[e]--;
	// 					// }
	// 			}
	// 	return count;	//lil lazy to impliment count just yet...
	// }
bool simulation::CopyCell(unsigned int length, unsigned int axis)		//makes a mosaic of the current cell of given length and axis
{
	unsigned int index;	//for index of new atom
	//errorcatching
	for(int e=0; e<elementNum; e++)
		if(elementCount[e]*length > K::MAX_ATOMS)
		{
			cerr << "cannot copy, the scale is too big!";
			cerr << "This operation would need " << elementCount[e]*length;
			cerr << " " << element[e] << " atoms.\n";
			cerr << "The Maximum of any element is: " << K::MAX_ATOMS << "\n";
			cerr << "This is regulated by K.h" << endl;
			return 0;
		}
	//copy cell
	for(int e=0; e<elementNum; e++)				//for each element,
	{
		for(int i=0; i<elementCount[e]; i++)	//for each atom
			for(int b=1; b<length; b++)			//for each block (except first -already have it. :P)
			{
				index = i+b*elementCount[e];		//index of new atom
				atom[e][index] = atom[e][i];		//copy the atom into the block
				atom[e][index].co.ord[axis] += b;	//shift coordinates along axis
			}
		elementCount[e]*=length;				//set the # of elements
	}

	//scale all coordinates to fit in bounds	//could easily put this in loop above... but would be hard to read.
	for(int e=0; e<elementNum; e++)
		for(int i=0; i<elementCount[e]; i++)
			atom[e][i].co.ord[axis] /= length;

	//scale lattice-----------
	lattice[axis] *= length;
	return 1;
}
bool simulation::Scale(unsigned int scale[3])
{	//error catching

	//copy unit cell
	for(int a=0; a<3; a++)	//for each axis
		if(!CopyCell(scale[a],a)) return 0;

	//trim outliers---------------
	//cerr << Trim() << " Atoms were trimmed off after Scaling\n";
	return 1;
}
bool simulation::Scale(double scale[3])	//ignores bonding. //FOR SCALING DOWN ONLY! the old algorythm wasnt good.
{
	//scale all coordinates to fit in bounds	//could easily put this in loop above... but would be hard to read.
	for(int e=0; e<elementNum; e++)
		for(int i=0; i<elementCount[e]; i++)
			atom[e][i].co /= scale;
	//scale lattice-----------
	for(int a=0; a<3; a++)
		lattice[a] *= scale[a];	//XXX a good excuse to make a vector class or use coordinate... (inherit from vecor?)
	//trim outliers---------------
	cerr << Trim() << " Atoms were trimmed off after Scaling\n";
	return 1;
}
bool simulation::Scale(string inFilename, string scaleFilename)	//XXX create a filename obj so pathing and extensions are taken care of
{
	//variables
	ifstream scaleFile;
	unsigned int count;
	unsigned int scale[3];
	string outFilename;
	unsigned int s;
	
	//open scalefile
	scaleFile.open(scaleFilename.c_str());
	if(scaleFile.fail())
	{
		cerr << "Failed to open \"" << scaleFilename << "\"" << endl;
		return 0;
	}

	scaleFile >> count;

	for(int a=0; a<count; a++)
	{
		//read in scale
		outFilename = "";	//reset outfilename
		for(int b=0; b<3; b++)
		{
			scaleFile >> scale[b];				//read scale
			// outFilename += to_string(scale[b]);	//create outfilename
			s = scale[b];
			do{
				outFilename += ('0'+s%10);
			} while((s/=10) >0);
		}
		if(!ReadData(inFilename))	//read data XXX hope to get rid of this later.
		{
			scaleFile.close();
			return 0;
		} 			
		if(!Scale(scale))			//scale data
		{
			cerr << "failed to scale: " << outFilename << endl;
			scaleFile.close();
			return 0;
		}				
		if(!WriteData(outFilename))	//write data
		{
			scaleFile.close();
			return 0;	
		}
	}
	//close scalefile
	scaleFile.close();
	return 1;
}
int simulation::Trim(void)
{
	int count =0;
	for(int e=0; e < elementNum; e++)			//for every atom
		for(int i=0; i < elementCount[e];)		//making room for exclusions
			if(!((atom[e][i].co >= 0) && (atom[e][i].co < 1)) || !atom[e][i].exists)	//if not within bounds (important logic here. :P)
			{
				RemoveAtom(e,i);
				count++;
			} else i++;
	return count;
}
void simulation::RemoveAtom(unsigned int e, unsigned int i)
{	//unbind atoms to this
	for(int a=0; a<atom[e][i].bondNum; a++)
	{
		atom[e][i].bond[a]->BreakBond(&atom[e][i]);	//break the bond from the other side.
		atom[e][i].bond[a] = 0;						//nulify pointer
	}
	atom[e][i].bondNum = 0;

	//remove atom from list
	for(int index=i; index+1<elementCount[e]; index++)
		atom[e][index] = atom[e][index+1];
	elementCount[e]--;
	return;
}
int simulation::PassivatedHole(coordinate hole, unsigned int radius)	//makes a passivated hole by recursion.
{
	//this is gonna be kinda hacky...
	//append an atom to list at coordinates, and run association.
	//remove from list, and run Passivate hole on the list of bonds.
	//return number of atoms removed

	// atom[0][elementCount].co = hole
	// elementCount[0]++;

	// return 0;
}
int simulation::PassivatedHole(coordinate center, unsigned int radius, atom_cls* subject)	//makes a passivated hole by recursion.
{
	int count = 1;
	subject->exists=0;	//mark for removal.	//XXX later simply remove atoms (rewuires using a pointer array of atoms)
	for(int a=0; a<subject->bondNum; a++)
		if(subject->bond[a]->exists)	//if not already marked.
		{
			if(ModDistance(subject->bond[a]->co, center) < radius)	//if bonded atom is within radius
				count+=PassivatedHole(center,radius,subject->bond[a]);
			else 	//passivate here
				Passivate(subject, subject->bond[a]);
		}
	return count;
}
// int simulation::Remove(void)
// {
// 	return count;
// }
double simulation::Volume(void)			//volume of lattice in cm^3
{
	double product=1;
	for(int a=0; a<3; a++)
		product*=lattice[a];
	return product*(1e-24);		
}
double simulation::Mass(void)				//mass of extant atom in g	//XXX NEEDS A LOT OF WORK
{
	double sum = 0;
	for(int e=0; e<elementNum; e++)
		sum+=Extant(e)*K::MASS[e];
	return sum;
}
unsigned int simulation::Extant(unsigned int e)
{
	unsigned int sum=0;
	for(int i=0; i<elementCount[e]; i++)
		sum+=atom[e][i].exists;
	return sum;
}
unsigned int simulation::Atoms(void)		//total # extant of atoms
{
	unsigned int sum=0;
	for(int e=0; e<elementNum; e++)
		sum+=Extant(e);
	return sum;
}
double simulation::Density(void)			//density of system in g/cm^3
{
	return Mass()/Volume();
}
double simulation::operator%(const unsigned int e)	//percent of extant elements
{
	return 100 * (double)Extant(e) / (double)Atoms();
}

double simulation::RealDistance(coordinate a, coordinate b)
{
	return (a*lattice).Distance(b*lattice);
}
double simulation::ModDistance(coordinate a, coordinate b)
{
	coordinate mod = a - (b+.5);//shift the coords so that coord b is in the center.
	mod.Dec();					//shift within bounds
	return RealDistance(mod, coordinate(.5));
}
double simulation::ModDistance(atom_cls* a, atom_cls* b)
{
	return ModDistance(a->co, b->co);
}