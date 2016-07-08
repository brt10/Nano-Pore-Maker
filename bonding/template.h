#ifndef BONDING_H
#define BONDING_H

#include <string>
namespace K
{
	const unsigned int MAX_ELEMENTS = 10;
	const unsigned int MAX_ATOMS = 3000;
	const unsigned int MAX_BONDS = 26;		//max bonds per atom... should probaly set to MAX_ELEMENTS*MAX_ATOMS to advoid complications?
	//in future, use array of bond lengths... limiting factor is reading these in from a file... [shrug] easy nuff.
	//const double BOND_LENGTH[ ((MAX_ELEMENTS+1) * MAX_ELEMENTS)/2 ];
	// const double BOND_LENGTH = 1.9;		//in Angstroms... 
	const unsigned int PRECISION = 4;	//precision in output
	const double MOLE = 6.022e23;//		Si 				C 				H
	const double MASS[MAX_ELEMENTS] = {28.085/MOLE ,12.011/MOLE, 1.00794/MOLE};
//RADII GO HERE
}
const unsigned int MAX_ELEMENTS=118;	//# of elements (may not be used for elements on future )
const double covalentRad[MAX_ELEMENTS] = 
const std::string elementSym = 

#endif