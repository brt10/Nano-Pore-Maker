#include "atom.h"

atom_cls::atom_cls()
{
	for(int a=0; a<K::MAX_BONDS; a++)
		bond[a] = 0;	//nulify all pointers
	bondNum = 0;
	exists = 1;
}
atom_cls::atom_cls(const atom_cls& atom)
{
	for(int a=0; a<K::MAX_BONDS; a++)
		bond[a] = atom.bond[a];
	co = atom.co;
	bondNum = atom.bondNum;
	exists = atom.exists;
	element = atom.element;
	for(int a=0; a<3; a++)
		freedom[a] = atom.freedom[a];
}
// atom_cls atom_cls::operator=(const atom_cls& atom)
// {
// 	this->(atom);
// 	return (*this)
// }

void atom_cls::BreakBond(atom_cls* atomP)
{
	int index = IsBound(atomP);
	if(index == -1)
		return;
	//move the other pointers down
	for(int a=index; a <bondNum-1; a++)	//move all refs
		bond[a] = bond[a+1];
	bondNum--;	//decrease the number of bonds
	return;
}
int atom_cls::IsBound(const atom_cls* atomP)
{
	int index;
	for(index=0; index<bondNum; index++)
		if(bond[index] == atomP)
			return index;
	return -1;	//bond not found
}

