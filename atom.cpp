#include "atom.h"
atom_cls::atom_cls()
{
	bonds = 0;
	exists = 1;
}
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
double atom_cls::RealDistance(coordinate a)
{
	coordinate b = this->co;
	for(int i=0; i<3; i++)
	{
		a.ord[i]*=this->lattice[i];
		b.ord[i]*=this->lattice[i];
	}
	return Distance(a,b);	//defined with coordinates
}