#ifndef ATOM_H
#define ATOM_H

#include <string>		//for strings
#include "coordinate.h"	//for coordinates
#include "K.h"			//for constants
//#include "simulation.h"
class atom_cls
{
	private:	
		double RealDistance(coordinate);//the real distance to a coord
		const simulation &parent;		//gives atom access to parent class (for lattice)
	public:
		//constructors-------------------
		// atom_cls();
		atom_cls(const simulation& p);	//must be constructed with reference
		//atom_cls(string,coordinate,atom_cls*[]);
		//prototypes----------------------
		double ModDistance(atom_cls*);	//shortest distance to another atom

		//variables---------------------
		coordinate co;					//coordinate
		atom_cls *bond[K::MAX_BONDS];	//pointer array of atoms bound to 
		unsigned int bonds;				//number of bonds
		bool exists;					//if the atom should be concidered
		bool freedom[3];				//if free to move along axis
		unsigned int element;			//index of atoms element in the array... later use 

};

		
#endif