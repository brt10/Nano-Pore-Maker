#ifndef ATOM_H
#define ATOM_H

// #include <string>		//for strings
#include "coordinate.h"	//for coordinates
#include "K.h"			//for constants
//#include "simulation.h"
class atom_cls
{
	private:	
		double RealDistance(coordinate);	//the real distance to a coord
		// int IsBound(atom_cls*);		//may be useful to write (impliment in breakbond)
		// const simulation &parent;		//gives atom access to parent class (for lattice)
	public:
		//constructors-------------------
		atom_cls();
		atom_cls(double[3]);
		// atom_cls(const simulation& p);	//must be constructed with reference
		//prototypes----------------------
		void BreakBond(atom_cls*);		//may add error catching to this later...
		double ModDistance(atom_cls*);	//shortest distance to another atom
		//variables---------------------
		coordinate co;					//coordinate
		atom_cls *bond[K::MAX_BONDS];		//pointer array of atoms bound to 
		unsigned int bondNum;			//number of bonds
		bool exists;					//if the atom should be concidered
		bool freedom[3];				//if free to move along axis
		unsigned int element;			//index of atoms element in the array... later use for bond lengths
		static double lattice[3];		//shared lattice for all atoms

};

		
#endif