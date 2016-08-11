#include "simulation.h"
// double simulation::latice[3] = {0};
simulation::simulation()
{
	ClearData();
}
simulation::~simulation()
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
	string symbol;
	unsigned int AN;
	unsigned int count;
	map<unsigned int, unsigned int> element;
	map<unsigned int, unsigned int>::iterator eIt;
	unsigned int total = 0;
	

	// string temp_str;
	// unsigned int temp_uint;

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

	//read in names of elements, map atomic numbers
	while(ss >> symbol)
	{
		AN = K::AtomicNumber(symbol);
		if(AN+1==0)
		{
			cerr << "The elemental symbol: \"" << symbol << "\" was not recognized, aborting. (simulation::ReadData)" << endl;
			return 0;
		}
		element[AN] = 0;
	}

	//read in atom count for each element, and ad to the total
	for(eIt=element.begin(); eIt!=element.end(); ++eIt)
	{
		infile >> count;
		eIt->second = count;
		total+=count;
	}

	//reserve space for atoms
	atom.reserve(total);

	//read in tag //XXX rename this!
	infile >> tag;

	//read atom data
	for(eIt=element.begin(); eIt!=element.end(); ++eIt)			//element
	{
		//for each atom of that element
		for(unsigned int i=0; i<eIt->second; ++i)	//index
		{
			coordinate coord;
			vector<bool> freedom;

			for(int c=0; c<3; ++c)			//read coord
				infile >> coord.ord[c];
			for(int c=0; c<3; ++c)			//read freedom
			{
				infile >> fChar;
				freedom.push_back( (Uppercase(fChar)=='T') );
			}

			atom.push_back(new atom_cls(eIt->first, coord, freedom));
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

	cout << "Bonding ";
	unsigned int percentage = 0;	//every 10 percent (perdekage?)

	for(it[0]=atom.begin(); it[0]!=atom.end(); ++it[0])
	{
		if(percentage < (it[0]-atom.begin())*10/atom.size())
			cout << ++percentage*10 << "%\b\b\b" << flush;
		if(!(*(it[0]))->exists) continue;		//make sure atom exists
		for(it[1]=it[0]+1; it[1]!=atom.end(); ++it[1])	//check only the atoms not yet analyzed.
		{
			if(!(*(it[1]))->exists) continue;		//make sure atom exists
			distance = ModDistance(*it[0], *it[1]);
			bondLength = K::COV_RAD[(*it[0])->atomicN] + K::COV_RAD[(*it[1])->atomicN];
			if( distance <= bondLength*(1+K::BOND_TOLERANCE))	//if not too far away
			{
				if(distance >= bondLength*(1-K::BOND_TOLERANCE))	//if not too close
				{
					if(Bond(*it[0], *it[1]))	++bondCount;
				}
				else cerr<<"Atoms "<< K::SYM[(*it[0])->atomicN] <<" and "<< K::SYM[(*it[1])->atomicN] <<"were unnaturally close at "<<distance<<"Angstroms\n";
			}
		}
	}
	cout << endl;
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
	//atom_cls* subject = new atom_cls();			//subject atom pointer
	coordinate r,p,s;		//coordinates of atoms (removed, passivated, subject)
	double ratio;			//ratio of bondlength
	unsigned int PEAN;		//atomic number of passivating element in K.h (atomic number-1)

	//initializers
	r=removed->coord;
	p=passivated->coord;

	PEAN = K::AtomicNumber(pe);	//search for passivating element in K.h
	if(PEAN+1==0)	//not found
	{
		cerr << "Passivating element \"" << pe << "\" not recognized (simulation::Passivate)" << endl;
		return;
	}

	ratio = (K::COV_RAD[PEAN] + K::COV_RAD[passivated->atomicN]) / ModDistance(r, p);

	s = ((r-p)+.5);	//center around .5 to use Mod()
	s.Mod();		//Modulus 1
	s -= .5;		//center around 0
	s *= ratio;		//fit to new length
	s += p;			//center around p
	s.Mod();		//Modulus 1

	atom.push_back(new atom_cls(PEAN, s));	//create subject.
	Bond(atom.back(), passivated);			//bond atoms
	return;
}
bool simulation::WriteData(const string filename)
{
	//variables	
	ofstream outfile;						//file obj		
	vector<unsigned int> AN;				//vector of atomic numbers
	vector<unsigned int>::iterator AN_it;	//indexing
	vector<atom_cls*>::iterator atom_it;	//indexing

	//set precision
	outfile << fixed;
	outfile << setprecision(K::PRECISION);

	outfile.open(filename.c_str());		//attempt to open file
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
	for(atom_it=atom.begin(); atom_it!=atom.end(); ++atom_it)			//for each atom
		if(AN.end() == find(AN.begin(), AN.end(), (*atom_it)->atomicN))	//if not in list
			AN.push_back((*atom_it)->atomicN);							//add to list

	//element names
	for(AN_it = AN.begin(); AN_it!=AN.end(); ++AN_it)	
		outfile << K::SYM[*AN_it] << ' ';
	outfile << "\n";

	//# extant atoms
	for(AN_it = AN.begin(); AN_it!=AN.end(); ++AN_it)
		outfile << Atoms(*AN_it) << ' ';
	outfile << "\n" << tag << "\n";

	//output atom coords and freedom
	for(AN_it=AN.begin(); AN_it!=AN.end(); ++AN_it)
	{
		for(atom_it=atom.begin(); atom_it!=atom.end(); ++atom_it)
		{
			if((*atom_it)->exists && (*atom_it)->atomicN == *AN_it)//only write out atoms if they exist, and right AN ;)
			{
				outfile << ' ';	//preceed with space
				for(int a=0; a<3; ++a)
					outfile << (*atom_it)->coord[a] << ' ';
				for(int a=0; a<3; ++a)
					outfile << ((*atom_it)->freedom[a] ? "T " : "F ");
				outfile << "\n";	//write a newline (do not use endl (forces flush))
			}
		}
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
	//reserve space
	atom.reserve(atom.size()*length);	//need to reserve space before marking end

	const vector<atom_cls*>::iterator end = atom.end();	//constant stopping point (could iterate backwards...)
	vector<atom_cls*>::iterator atom_it;				//indexing

	//copy cell
	for(atom_it=atom.begin(); atom_it!=end; ++atom_it)	//use end so that this doesnt add FOREVER... :P
	{
		for(unsigned int b=1; b<length; b++)			//for each block (except first -already have it. :P)
		{
			atom.push_back(new atom_cls(**atom_it));	//copy
			atom.back()->coord[axis] += b;				//shift coordinates along axis
		}
	}
	//scale all coordinates to fit in bounds	//could easily put this in loop above... but would be hard to read.
	for_each(atom.begin(), atom.end(), [length, axis](atom_cls* atomP)
	{
		atomP->coord[axis] /= length;
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
	{
		cout << "Scaling" << a << endl;
		if(!CopyCell(scale[a],a)) return 0;
	}
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
	for_each(atom.begin(), atom.end(), [scale](atom_cls* atomP)
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
		if(!(((*it)->coord >= 0) && ((*it)->coord < 1)) || !(*it)->exists)	//if not within bounds (important logic here. :P)
		{
			RemoveAtom(*it);
			++count;
		} else ++it;			//only increment if nothing was removed.
	}
	return count;
}
void simulation::RemoveAtom(atom_cls* atomP)
{	
	vector<atom_cls*>::iterator atom_it;	//index

	atom_it = find(atom.begin(), atom.end(), atomP);				//find the atom
	if(atom_it==atom.end())	//not found
	{
		cerr << "Attempted to remove non-extant atom... could be a memory issue!" << endl;
		return;
	}

	//unbind
	for_each(atomP->bond.begin(), atomP->bond.end(), [atomP](atom_cls* bondedP)
	{
		bondedP->BreakBond(atomP);	//break bond to atom
		return;
	});
	atomP->bond.clear();	//remove bonds from atom

	//delete
	delete atomP;

	//erase
	atom.erase(atom_it);

	return;
}
int simulation::PassivatedPore(double radius, coordinate center, string pe)	//makes a passivated hole by recursion.
{
	vector<atom_cls*>::iterator centerAtom = Closest(center);		//find atom closest to the coordinates
	if(centerAtom == atom.end())
	{
		cerr << "Found no atoms near coordinate: ";
		for(unsigned int a=0; a<3; ++a)
			cerr << center[a] << " ";
		cerr << "(simulations::PassivatedPore)" << endl;
		return 0;
	}
	if(ModDistance((*centerAtom)->coord, center) > radius)				//if the atom is too far away, stop
		return 0;
	return PassivatedPore(radius, *centerAtom, pe, center);	//else make passivated hole about this coordinate starting with centerAtom
}
int simulation::PassivatedPore(double radius, atom_cls* subject, string pe, coordinate center)	//makes a passivated hole by recursion.
{
	int count = 1;
	// vector<atom_cls*>::iterator bonded;	//atoms binded to **subject

	subject->exists = 0;	//required to smehow remove atom from making inf loops...

	//passivate
	for(unsigned int b=0; b<subject->bond.size(); ++b)
	{
		if(subject->bond[b]->exists)	//if not already marked.
		{
			if(ModDistance(subject->bond[b]->coord, center) < radius)	//if bonded atom is within radius
			{
				count+=PassivatedPore(radius, subject->bond[b], pe, center);	//recurse
				--b;	//bond will be removed from list, so redo. //XXX source of extra calculation...
			}
			else if(!pe.empty()) 	//passivate here
				Passivate(subject, subject->bond[b], pe);
		}
	}

	//remove atom
	RemoveAtom(subject);

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
	vector<atom_cls*>::iterator atom_it;
	unsigned int t=0;								//number of tests

	if(Atoms(AN)+1==0)	//no elements to search //need error-catcing
		cerr << "There are no atoms with Atomic Number: "<< AN << " in the simulation (simulation::Closest)" << endl;
	else
		for(atom_it=atom.begin(); atom_it!=atom.end(); ++atom_it)
		{
			if(AN != (unsigned int)-1 && (*atom_it)->atomicN != AN)	continue;	//not the element we are looking for
			if(extant!=-1 && (*atom_it)->exists != (bool)extant)	continue;		//not the extant we are looking for

			testDist = ModDistance((*atom_it)->coord, c);
			if( testDist < minDist || t==0)
			{
				minDist = testDist;
				min = atom_it;
			}
			++t;
		}
	return min;	//XXX no error catching!
}
vector<atom_cls*>::iterator simulation::Center(unsigned int AN)	//default = -1
{
	return Closest(coordinate(.5),AN);
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
		if(!(*it)->exists)	continue;
		sum+=K::MASS[(*it)->atomicN];
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
	return Atoms(AN);
}
unsigned int simulation::Atoms(unsigned int AN)
{
	unsigned int sum=0;
	vector<atom_cls*>::iterator it;
	for(it=atom.begin(); it!=atom.end(); ++it)
		sum+=((*it)->atomicN==AN && (*it)->exists);
	return sum;
}
unsigned int simulation::Atoms(void)		//total # extant of atoms
{
	unsigned int sum=0;
	vector<atom_cls*>::iterator it;
	for(it=atom.begin(); it!=atom.end(); ++it)
		sum+=(*it)->exists;
	return sum;
}
double simulation::Density(void)			//density of system in g/cm^3
{
	return Mass()/Volume();
}
double simulation::operator%(const string e)	//percent of extant elements
{
	return 100 * (double)Atoms(e) / (double)Atoms();
}
double simulation::operator%(const unsigned int e)	//percent of extant elements
{
	return 100 * (double)Atoms(e) / (double)Atoms();
}
vector<vector<atom_cls*>> simulation::Cluster(void)
{
	vector<vector<atom_cls*>> cluster;
	vector<atom_cls*>::iterator atom_it;
	vector<atom_cls*> temp;	//temporary storage for groups

	for(atom_it=atom.begin(); atom_it!=atom.end(); ++atom_it)	//every atom
	{	//if atom is not already in cluster...
		if(cluster.size()==0||cluster.end() == find_if(cluster.begin(), cluster.end(), [atom_it](vector<atom_cls*> atomV)
		{
			return (atomV.end()!=find_if(atomV.begin(), atomV.end(), [atom_it](atom_cls* atomP)
			{
				return (*atom_it == atomP);
			}));
			
		}))
		{
			temp.clear();
			cluster.push_back(Group( temp, *atom_it));	//add that atom's group
		}
	}
	return cluster;
}
vector<atom_cls*> simulation::Group(vector<atom_cls*>& tree, atom_cls* seed)
{
	for_each(seed->bond.begin(), seed->bond.end(), [&tree, this](atom_cls* branch)
	{
		if(tree.end() == find(tree.begin(), tree.end(), branch))	//if unique
		{
			// cout << "adding a branch!" << endl;
			tree.push_back(branch);	//add the branch
			Group(tree, branch);	//recurse along branch	//XXX could remove branch, and just continue from last on tree
		}
		return;
	});
	
	return tree;
}
unsigned int simulation::BondNum(unsigned int AN0, unsigned int AN1)
{
	unsigned int count = 0;
	for_each(atom.begin(), atom.end(), [&count, AN0, AN1](atom_cls* a0)
	{
		if(a0->exists && a0->atomicN==AN0)
			for_each(a0->bond.begin(), a0->bond.end(), [&count, AN1](atom_cls* a1)
			{
				if(a1->exists && a1->atomicN==AN1)
					++count;
				return;
			});
		return;
	});
	return count/2;
}
unsigned int simulation::BondNum(string SYM0, string SYM1)
{
	string sym[2] = {SYM0, SYM1};
	unsigned int AN[2];
	for(int a=0; a<2; ++a)
	{
		AN[a] = K::AtomicNumber(sym[a]);
		if(AN[a]+1==0)
		{
			cerr << sym[a] << " was not recognized as an elemental symbol" << endl;
			return 0;
		}
	}
	return BondNum(AN[0],AN[1]);
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
unsigned int simulation::ElementNum(void)	//number of elements
{
	vector<int> AN;
	vector<atom_cls*>::iterator it;
	for(it=atom.begin(); it!=atom.end(); ++it)
		if(AN.end() == find(AN.begin(), AN.end(), (*it)->atomicN))
			AN.push_back((*it)->atomicN);
	return AN.size();
}