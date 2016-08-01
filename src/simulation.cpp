#include "simulation.h"
// double simulation::latice[3] = {0};
simulation::simulation()
{
	ClearData();
}
void simulation::ClearData(void)
{
	//clear atom vector
	for_each(atom.begin(), atom.end(), [](atom_cls* atomP)
	{
		delete atomP;
		return;
	});
	atom.clear();
	multiplier=1;
	return;
}
bool simulation::ReadData(const string filename)
{
	//variables
	stringstream ss;
	string line;
	char fChar;
	ifstream infile;
	infile.open(filename.c_str());
	unsigned int atomicNumber;		//AN of element

	//elementNum = 0;	//from simulation
	//reset class
	ClearData();

	if(infile.fail()){
		cerr << "Failed to open: \"" << filename << "\"\n";
		return 0;
	}
	//read from file...
	getline(infile, title);
	infile >> multiplier;
	for(int a=0; a<3; a++)
		for(int b=0; b<3; ++b)
		{
			if(a==b) infile >> lattice[a];
			else infile >> lattice[2];	//essentially junk... works, just ugly. :P
		}
	//read element names------------
	// do{
	// 	infile >> elementChar;

	// 	if(elementChar>=A && elementChar<=Z || elementChar>=a && elementChar<=z)
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

	//read in names of elements
	while(ss >> element[elementNum])
		++elementNum;

	//read in atom count for each element
	for(unsigned int a=0; a<elementNum; ++a)
		infile >> elementCount[a];
	infile >> tag;

	//read atom data
	for(unsigned int e=0; e<elementNum; ++e)			//element
	{
		//find AN of element
		atomicNumber = K::ElementIndex(element[e]);
		if(atomicNumber+1 == 0)
		{
			cerr << "Unable to find the \"" << element[e] << "\" you are looking for..." << endl;
			return 0;
		}
		//for each atom of that element
		for(unsigned int i=0; i<elementCount[e]; ++i)	//index
		{
			coordinate coord;
			vector<bool> freedom;

			atom[e][i].element = eIndex;	//element set as index of sym - lets the atom know its symbol, covalent radius, and mass
			for(int c=0; c<3; ++c)			//read coord
				infile >> coord[c];
			for(int c=0; c<3; ++c)			//read freedom
			{
				infile >> fChar;
				freedom.pushback( (Uppercase(fChar)=='T') );
			}

			atom.pushback(new atom_cls(atomicNumber, coord, freedom));
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
	for_each(atom.begin(), atom.end(), [](atom_cls* atomP)
	{
		atomP->coord.Mod(1);	//XXX should probaly check for overlap with other atoms...
		return;
	});
	return;
}
//returns how many bonds were made
void simulation::Disassociate(void)
{
	for_each(atom.begin(), atom.end(), [](atom_cls* atomP)
	{
		atomP->bond.clear();
		return;
	});
	return;
}
int simulation::Associate(void)	//XXX make sure it deals with previous bonds!
{
	
	unsigned int bondCount=0;			//return value; # of bond created
	double distance;					//distance between atoms
	double bondLength;					//optimal bond length for atoms
	vector<atom_cls*>::iterator it[2];	//iterator array

	for(it[0]=atom.begin(); it[0]!=atom.end(); ++it[0])
	{
		if(!*it[0]->exists) return;		//make sure atom exists
		for(it[1]=it[0]+1; it[1]!=atom.end(); ++it[1])	//check only the atoms not yet analyzed.
		{
			if(!*it[1]->exists) return;		//make sure atom exists
			distance = ModDistance(*it[0], *it[1]);
			bondLength = K::COV_RAD[*it[0]->atomicN] + K::COV_RAD[*it[1]->atomicN];
			if( distance <= bondLength*(1+K::BOND_TOLERANCE))	//if not too far away
			{
				if(distance >= bondLength*(1-K::BOND_TOLERANCE))	//if not too close
				{
					if(Bond(*it[0], *it[1]))	++bondCount;
				}
				else cerr<<"Atoms "<< K::SYM[*it[0]->atomicN] <<" and "<< K::SYM[*it[1]->atomicN] <<"were unnaturally close at "<<distance<<"Angstroms\n";
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
		int b=(a+1)%2;									//other atom
		if(atomP[a]->IsBound(atomP[b])) continue;		//if already bound, skip this direction.
		atomP[a]->bond.push_back(atomP[b]);				//bond
	}
	return 1;
}
void simulation::Passivate(atom_cls* removed, atom_cls* passivated, string pe)
{
	//atom_cls* subject = new atom_cls();			//Hydrogen atom pointer
	coordinate r,p,s;		//coordinates of atoms (removed, passivated, subject)
	double ratio;			//ratio of bondlength
	unsigned int peIndex	//index of pe in simulation
	unsigned int PEAN;		//atomic number of passivating element in K.h (atomic number-1)

	//initializers
	r=removed->coord;
	p=passivated->coord;
	for(peIndex=0; peIndex<K::NUM_ELEMENTS; ++peIndex)	//search for pe in current elements
		if(Uppercase(element[peIndex]) == Uppercase(pe)) break;
	if(peIndex >= K::MAX_ELEMENTS)		//if passivating element is not alreay in simulation
	{
		PEAN = K::ElementIndex(pe);	//search for passivating element in K.h
		if(PEAN >= K::NUM_ELEMENTS)	//not found
		{
			cerr << "Passivating element \"" << pe << "\" not recognized (simulation::Passivate)" << endl;
			return;	//XXX need better error-catching
		}
		elementCount[elementNum] = 0;
		element[elementNum] = pe;
		peIndex = elementNum;
		++elementNum;
	}
	else PEAN = elementIndex[peIndex];		//already in sim
	ratio = (K::COV_RAD[PEAN] + K::COV_RAD[passivated->atomicN]) / ModDistance(r, p);

	s = ((r-p)+.5);	//center around .5 to use Mod()
	s.Mod();		//Modulus 1
	s -= .5;		//center around 0
	s *= ratio;		//fit to new length
	s += p;			//center around p
	s.Mod();		//Modulus 1

	atom.push_back(new atom_cls(PEAN, s));	//create subject.
	bond(atom.back(), passivated);			//bond atoms
	elementCount[peIndex]++;				//add to count
	return;
}
bool simulation::WriteData(const string filename)
{
	//variables
	unsigned int e;	//indexing
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
		for(int b=0; b<3; ++b)
			outfile << ((a==b) ? lattice[a] : 0) << " ";
		outfile << "\n";
	}
	for(e=0; e<elementNum; ++e)	//element names
		outfile << element[e] << " ";
	outfile << "\n";
	for(e=0; e<elementNum; ++e)	//# extant atoms
		outfile << Extant(e) << ' ';
	outfile << "\n" << tag << "\n";

	//output atom coords and freedom
	for_each(atom.begin(), atome.end(), [outfile](atom_cls* atomP)
	{
		if(atomP->exists)//only write out atoms if they exist ;)
		{
			outfile << ' ';	//preceed with space
			for(int a=0; a<3; ++a)
				outfile << atomP->coord[a] << " ";
			for(int a=0; a<3; ++a)
				outfile << (atomP->freedom[a] ? "T " : "F ");
			outfile << "\n";	//write a newline (do not use endl (forces flush))
		}
		return;
	});
	outfile.close();
	return 1;
}
bool simulation::operator>>(const string filename)
{
	return WriteData(filename);
}
bool simulation::CopyCell(unsigned int length, unsigned int axis)		//makes a mosaic of the current cell of given length and axis
{
	unsigned int index;	//for index of new atom
	unsigned int e,b;	//indexing
	vector<atom_cls*>::iterator end = atom.end();
	//copy cell
	for_each(atom.begin(), end, [](atom_cls* P)	//use end so that this doesnt add FOREVER... :P
	{
		for(b=1; b<length; b++)			//for each block (except first -already have it. :P)
		{
			atom.pushback(new atom_cls(*atomP));	//copy
			atom.back()->coord[axis] += b;			//shift coordinates along axis
		}
		return;
	});
	//scale all coordinates to fit in bounds	//could easily put this in loop above... but would be hard to read.
	for_each(atom.begin(), atom.end(), [](atom_cls* atomP)
	{
		atomP->coord /= length;
		return;
	});

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
bool simulation::Scale(unsigned int s)
{
	unsigned int scale[3] = {s,s,s};
	return Scale(scale);
}
bool simulation::Scale(double scale[3])	//ignores bonding. //FOR SCALING DOWN ONLY! the old alorithm wasnt good.
{
	//scale all coordinates to fit in bounds
	for_each(atom.begin(), atom.end(), [](atom_cls* atomP)
	{
		atomP->coord /= scale;
		return;
	});
	//scale lattice-----------
	for(int a=0; a<3; a++)
		lattice[a] *= scale[a];	//XXX a good excuse to make a vector class or use coordinate... (inherit from vector?)
	//trim outliers---------------
	cerr << Trim() << " Atoms were trimmed off after Scaling\n";
	return 1;
}
int simulation::Trim(void)
{
	int count =0;
	vector<atom_cls*>::iterator it;	//indexing

	for(it=atom.begin(); it!=atom.end();)
	{
		if(!((*it->coord >= 0) && (*it->coord < 1)) || !*it->exists)	//if not within bounds (important logic here. :P)
		{
			RemoveAtom(it);
			++count;
		} else ++it;			//only increment if nothing was removed.
	}
	return count;
}
void simulation::RemoveAtom(vector<atom_cls*>::iterator& it)
{	
	//unbind
	for_each(*it->bond.begin, *it->bond.end(), [](atom_cls* atomP)
	{
		atomP->BreakBond(*it);	//break bond to atom
		return;
	});
	*it->bond.clear();	//remove bonds from atom

	//delete
	delete *it;

	//erase
	atom.erase(it);

	return;
}
int simulation::PassivatedPore(double radius, coordinate* center, string pe)	//makes a passivated hole by recursion.
{
	coordinate c =.5;
	if(!center)
		center = &c;
	atom_cls* centerAtom = Closest(*center);			//find atom closest to the coordinates
	if(ModDistance(centerAtom->co, *center) > radius)	//if the atom is too far away, stop
		return 0;
	return PassivatedPore(radius, centerAtom, pe, center);	//else make passivated hole about this coordinate starting with centerAtom
}
int simulation::PassivatedPore(double radius, vector<atom_cls*>::iterator& subject, string pe, coordinate* center)	//makes a passivated hole by recursion.
{
	if(!center)	//null pointer
		center = &(*subject->co);
	int count = 1;

	vector<atom_cls*>::iterator bonded;

	//remove atom
	RemoveAtom()

	for(bonded=subject->bond.begin(); bonded!=subject->bond.end(); ++bonded)
	{
		if(*bonded->exists)	//if not already marked.
		{
			if(ModDistance(*bonded->co, *center) < radius)	//if bonded atom is within radius
				count+=PassivatedPore(radius, *bonded, pe, center);
			else if(!pe.empty()) 	//passivate here
				Passivate(subject, *bonded, pe);
		}
	}
	return count;
}
// int simulation::Remove(void)
// {
// 	return count;
// }
atom_cls* simulation::Closest(coordinate c, string e, int extant)	//extant:-1=everything, 0=non-extant, 1=extant
{
	unsigned int a;										//indexing
	for(a=0; a < K::MAX_ELEMENTS; ++a)						//find elemetn in sim
		if(Uppercase(e)==Uppercase(element[a])) break;	//if in array
	if(a < K::MAX_ELEMENTS)								//Exists in sim
		return Closest(c,a);
	cerr << "The element: \"" << e << "\" does not seem to exist in the simulation. (simulation::Closest)" << endl;
	return 0;
}
atom_cls* simulation::Closest(coordinate c, unsigned int E, int extant)	//default E=-1 //XXX should make this more efficient... has to look through all atoms
{
	double testDist;
	double minDist=0;		//will be set on the first iteration
	atom_cls* minP=0;		//pointer to closest atom
	atom_cls* testP=0;		//pointer to test atom
	unsigned int e,i,t(0);	//indexing

	if(E == (unsigned int)-1){		//no element selected
		for(e=0; e<elementNum; ++e)	//for all elements
		{
			testP = Closest(c,e);
			if(!testP) continue;	//if a null pointer was returned
			testDist = ModDistance(testP->co, c);	// XXX will contributs slightly to the time... but how many elements does one have?
			if(testDist < minDist || t==0)
			{
				minDist = testDist;
				minP = testP;
			}
			++t;
		}
	}
	else if(elementCount[E]==0)	//no elements to search 
		cerr << "There are no \"" << element[E] << "\" atoms in the simulation (simulation::Closest)" << endl;
	else if(E>=0 && E<elementNum){	//element has been selected
		for(i=0; i<elementCount[E]; ++i)
		{
			testP = &atom[E][i];
			if(extant!=-1)							//if we are extant-specific
				if(testP->exists != (bool)extant)	//if not the type of extant we are looking for...
					continue;
			testDist = ModDistance(testP->co, c);
			if( testDist < minDist || t==0)
			{
				minDist = testDist;
				minP = &atom[E][i];
			}
			++t;
		}
	}else cerr << "element select of: " << E << "Is out of bounds (simulation::Closest)" << endl;	//out of bounds //returns null pointer
	return minP;	//XXX no error catching!
}
atom_cls* simulation::Center(int E)	//default = -1
{
	return Closest(coordinate(.5),E);
}
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
	for(unsigned int e=0; e<elementNum; e++)
		sum+=Extant(e)*K::MASS[elementIndex[e]];
	return sum;
}
unsigned int simulation::Extant(string e)
{
	unsigned int i = (unsigned int)ElementIndex(e);
	if(i == (unsigned int)-1)	//not found
	{
		cerr << "Found no \"" << e << "\" in the simulation! (simulation::Extant)" << endl;
		return 0;
	}
	return Extant(i);
}
unsigned int simulation::Extant(unsigned int e)
{
	if(e>=elementNum)
	{
		cerr << "The index of \"" << "\" is too large! (simulation::Extant)" << endl;
	}
	unsigned int sum=0;
	for(unsigned int i=0; i<elementCount[e]; i++)
		sum+=atom[e][i].exists;
	return sum;
}
unsigned int simulation::Atoms(void)		//total # extant of atoms
{
	unsigned int sum=0;
	for(unsigned int e=0; e<elementNum; e++)
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
//distance
double simulation::RealDistance(coordinate a, coordinate b)
{
	return (a*lattice).Distance(b*lattice);
}
double simulation::ModDistance(coordinate a, coordinate b)
{
	coordinate mod = a - (b+.5);//shift the coords so that coord b is in the center.
	mod.Mod();					//shift within bounds
	return RealDistance(mod, coordinate(.5));
}
double simulation::ModDistance(atom_cls* a, atom_cls* b)
{
	return ModDistance(a->co, b->co);
}
//element control
int simulation::ElementIndex(string name)
{
	name = Uppercase(name);
	for(unsigned int e=0; e<elementNum; e++)
		if(name == Uppercase(element[e]))	//if Found match
			return e;
	return -1;	//if failed to find match. return -1
}