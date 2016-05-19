#ifndef ATOM_H
#define ATOM_H

// #include <string>		//for strings
#include "coordinate.h"	//for coordinates
#include "K.h"			//for constants
//#include "simulation.h"
class atom_cls
{
	public:
		//constructors-------------------
		atom_cls();
		atom_cls(const atom_cls&);		//copy an existing atom
		//prototypes----------------------
		// atom_cls operator=(const atom_cls&);	//for initializing by reference...
		int IsBound(const atom_cls*);			//may be useful to write (impliment in breakbond) (returns the index of the bond)
		void BreakBond(atom_cls*);		//may add error catching to this later...
		//variables---------------------
		coordinate co;					//coordinate
		atom_cls *bond[K::MAX_BONDS];	//pointer array of atoms bound to 
		unsigned int bondNum;			//number of bonds
		bool exists;					//if the atom should be concidered
		bool freedom[3];				//if free to move along axis
		unsigned int element;			//index of atoms element in the array... later use for bond lengths
};

		
#endif