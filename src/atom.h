#ifndef ATOM_H
#define ATOM_H

#include <vector>		//for vectors
#include <algorithm>	//for find_if
#include "coordinate.h"	//for coordinates
#include "K.h"			//for constants

using namespace std;	//for now...

class atom_cls
{
	public:
		//constructors-------------------
		atom_cls(const atom_cls&);					//copy an existing atom without bonds
		atom_cls(int, coordinate, vector<bool> &);	//initializing atoms quickly
		atom_cls(int=0, coordinate=coordinate(0));	//initializing atoms quickly (without freedom)
		~atom_cls();
		//prototypes----------------------
		void Default(void);						//sets default values
		atom_cls& operator=(const atom_cls&);	//copy
		void BreakBond(atom_cls*);				//may add error catching to this later...
		bool IsBound(atom_cls*);				//if bound to atom
		//variables---------------------
		coordinate coord;				//coordinate 
		bool exists;					//if the atom should be concidered
		vector<bool> freedom;		//if free to move along axis
		unsigned int atomicN;			//atomic number of element represented.
		vector<atom_cls*> bond;			//bonds to other atoms
};

		
#endif