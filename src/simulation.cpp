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
	unsigned int e, E;	//indexing
	ofstream outfile;
	outfile.open(filename.c_str());
	vector<int> AN;
	vector<atom_cls*>::iterator it;

	//set precision
	outfile << fixed;
	outfile << setprecision(K::PRECISION);

	if(outfile.fail()){
		cerr << "Failed to open: \"" << filename << "\"\n";
		return 0;
	}
	//title
	outfile << title << "\n";
	//multiplier
	outfile << multiplier << "\n";
	//lattice
	for(int a=0; a<3; a++)
	{
		for(int b=0; b<3; ++b)
			outfile << ((a==b) ? lattice[a] : 0) << " ";
		outfile << "\n";
	}

	//count elements...
	for(it=atom.begin(); it!=atom.end(); ++it)
		if(AN.end() == find(AN.begin(), AN.end(), *it->AtomicN))
			AN.push_back(*it->atomicN);

	//element names
	for(e=0; e<AN.size(); ++e)	
		outfile << K::SYM[AN[e]] << " ";
	outfile << "\n";

	//# extant atoms
	for(e=0; e<AN.size(); ++e)	
		outfile << Atoms(e) << ' ';
	outfile << "\n" << tag << "\n";

	//output atom coords and freedom
	for(unsigned int e=0; e<AN.size(); ++e)
	{
		for_each(atom.begin(), atom.end(), [outfile](atom_cls* atomP)
		{
			if(atomP->exists && atomP->atomicN == AN[e])//only write out atoms if they exist ;)
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
	}
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
	if(!center) center = &c;
	vector<atom_cls*>::iterator centerAtom = Closest(*center);		//find atom closest to the coordinates
	if(centerAtom == atom.end())
	{
		cerr << "Found no atoms near coordinate: ";
		for(unsigned int a=0; a<3; ++a)
			cerr << center->ord[a] << " ";
		cerr << "(simulations::PassivatedPore)" << endl;
		return 0;
	}
	if(ModDistance(*centerAtom->co, *center) > radius)				//if the atom is too far away, stop
		return 0;
	return PassivatedPore(radius, centerAtom, pe, center);	//else make passivated hole about this coordinate starting with centerAtom
}
int simulation::PassivatedPore(double radius, vector<atom_cls*>::iterator& subject, string pe, coordinate* center)	//makes a passivated hole by recursion.
{
	if(!center)	//null pointer
		center = &(*subject->co);
	int count = 1;
	vector<atom_cls*>::iterator bonded;	//atoms binded to **subject

	//passivate
	for(bonded=subject->bond.begin(); bonded!=subject->bond.end(); ++bonded)
	{
		if(*bonded->exists)	//if not already marked.
		{
			if(ModDistance(*bonded->co, *center) < radius)	//if bonded atom is within radius
				count+=PassivatedPore(radius, *bonded, pe, center);
			else if(!pe.empty()) 	//passivate here
				Passivate(*subject, *bonded, pe);
		}
	}

	//remove atom
	RemoveAtom(subject)

	return count;
}
vector<atom_cls*>::iterator simulation::Closest(coordinate c, string e, int extant)	//extant:-1=everything, 0=non-extant, 1=extant
{
	unsigned int AN = K::AtomicNumber(e);
	if(AN+1==0)
	{
		cerr << "The element: \"" << e << "\" does not seem to exist. (simulation::Closest)" << endl;
		return atom.end();
	}
	else return Closest(c, AN, extant);
}
vector<atom_cls*>::iterator simulation::Closest(coordinate c, unsigned int AN, int extant)	//default E=-1 //XXX should make this more efficient... has to look through all atoms
{
	double testDist;
	double minDist=0;								//will be set on the first test
	vector<atom_cls*>::iterator min =atom.end();	//iterator to closest atom
	unsigned int t=0;								//number of tests

	if(AtomNum(AN)<=0)	//no elements to search 
		cerr << "There are no \"" << element[E] << "\" atoms in the simulation (simulation::Closest)" << endl;
	else
		for(it=atom.begin(); it!=atom.end(); ++it)
		{
			if(AN != (unsigned int)-1 && *it->atomicN != AN)	continue;	//not the element we are looking for
			if(extant!=-1 && *it->exists != (bool)extant)	continue;		//not the extant we are looking for

			testDist = ModDistance(*it->coord, c);
			if( testDist < minDist || t==0)
			{
				minDist = testDist;
				min = it;
			}
			++t;
		}
	return min;	//XXX no error catching!
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
double simulation::Mass(void)			//mass of extant atom in g
{
	double sum = 0;
	vector<atom_cls*>::iterator it;

	for(it=atom.begin(); it!=atom.end(); ++it)
	{
		if(!*it->extant)	continue;
		sum+=K::MASS[*it->atomicN];
	}
	return sum;
}
unsigned int simulation::Atoms(string e)
{
	unsigned int AN = K::AtomicNumber(e);
	if(AN+1==0)	//not found
	{
		cerr << "Found no \"" << e << "\" in the simulation! (simulation::Extant)" << endl;
		return 0;
	}
	return Extant(AN);
}
unsigned int simulation::Atoms(unsigned int AN)
{
	unsigned int sum=0;
	vector<atom_cls*>::iterator it;
	for(it=atom.begin(); it!=atom.end(); ++it)
		sum+=(*it->atomicN==AN && *it->extant);
	return sum;
}
unsigned int simulation::Atoms(void)		//total # extant of atoms
{
	unsigned int sum=0;
	vector<atom_cls*>::iterator it;
	for(it=atom.begin(); it!=atom.end(); ++it)
		sum+=*it->extant;
	return sum;
}
double simulation::Density(void)			//density of system in g/cm^3
{
	return Mass()/Volume();
}
double simulation::operator%(const string e)	//percent of extant elements
{
	return 100 * (double)Extant(e) / (double)Atoms();
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
	return ModDistance(a->coord, b->coord);
}
unsigned int ElementNum(void)	//number of elements
{
	vector<int> AN;
	vector<atom_cls*>::iterator it;
	for(it=atom.begin(); it!=atom.end(); ++it)
		if(AN.end() == find(AN.begin(), AN.end(), *it->AtomicN))
			AN.push_back(*it->atomicN);
	return AN.size();
}