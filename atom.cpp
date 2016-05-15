#include "atom.h"

double atom_cls::lattice[3] = {0};

atom_cls::atom_cls()
{
	for(int a=0; a<3; a++)
		lattice[a] = 1;
	for(int a=0; a<K::MAX_BONDS; a++)
		bond[a] = 0;
	bondNum = 0;
	exists = 1;
}
// atom_cls::atom_cls(double l[3])
// {
// 	for(int a=0; a<3; a++)
// 		lattice[a] = l[a];
// }
// atom_cls::atom_cls(const simulation& p) : parent(p)
// {
// 	// parent = p;
// 	bonds = 0;
// 	exists = 1;
// }
double atom_cls::ModDistance(atom_cls *atom)
{
	// double minDistance
	coordinate mod = atom->co;	//this will be shifted later...
	for(int a=0; a<3; a++)		//for each axis
	{
		//shift the coords... so the atom of interest is in the center.
		mod.ord[a]-=(this->co.ord[a]+.5);
		//shift within bounds
		while(mod.ord[a]<0)
			mod.ord[a]++;
		while(mod.ord[a]>=1)
			mod.ord[a]--;
	}
	return this->RealDistance(mod);	//function of atom_cls
}
void atom_cls::BreakBond(atom_cls* atom)
{
	unsigned int index=0;
	//find the bond to remove
	while(bond[index]!=atom && index <bondNum)	//find atom
		index++;
	if(bond[index]!=atom) return;	// if the atom was not found.
	//move the other pointers down
	for(int a=index; a <bondNum-1; a++)	//move all refs
		bond[a] = bond[a+1];
	bondNum--;	//decrease the number of bonds
	return;
}
double atom_cls::RealDistance(coordinate a)
{
	coordinate b = this->co;
	for(int i=0; i<3; i++)
	{
		a.ord[i]*=/*atom_cls::*/lattice[i];
		b.ord[i]*=/*atom_cls::*/lattice[i];
	}
	return Distance(a,b);	//defined with coordinates
}