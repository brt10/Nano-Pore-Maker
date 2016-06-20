#include "atom.h"

atom_cls::atom_cls()
{
	ClearData();
}
atom_cls::atom_cls(const atom_cls& atom)	//should not copy bonds
{
	ClearData();
	co = atom.co;
	exists = atom.exists;
	element = atom.element;			
	for(int a=0; a<3; a++)
		freedom[a] = atom.freedom[a];
}
void atom_cls::ClearData(void)
{

	for(unsigned int a=0; a<K::MAX_BONDS; a++)
		bond[a] = 0;	//nulify all pointers
	bondNum = 0;
	exists = 1;
	for(int a=0; a<3; a++)
		freedom[a] = 1;
	return;
}
atom_cls& atom_cls::operator=(const atom_cls& atom)
{
	ClearData();
	//do not transfer freedoms
	co = atom.co;			//coords
	exists = atom.exists;	//extant
	element = atom.element;	//element
	return (*this);
}

void atom_cls::BreakBond(atom_cls* atomP)
{
	int index = IsBound(atomP);
	if(index == -1)
		return;
	//move the other pointers down
	for(unsigned int a=index; a <bondNum-1; a++)	//move all refs
		bond[a] = bond[a+1];
	bondNum--;	//decrease the number of bonds
	return;
}
int atom_cls::IsBound(const atom_cls* atomP)
{
	unsigned int index;
	for(index=0; index<bondNum; index++)
		if(bond[index] == atomP)
			return index;
	return -1;	//bond not found
}

