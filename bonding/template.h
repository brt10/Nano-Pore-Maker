#ifndef BONDING_H
#define BONDING_H

#include <string>
namespace K
{
	const unsigned int MAX_ELEMENTS = 5;	//max number of elements in simulation
	const unsigned int MAX_ATOMS = 3000;	//max number of atoms in simulation
	const unsigned int MAX_BONDS = 26;		//max bonds per atom... should probaly set to MAX_ELEMENTS*MAX_ATOMS to advoid complications?
	const unsigned int PRECISION = 4;		//precision in outputs
	const double MOLE = 6.022e23;			//avagadro's number
	const double BOND_TOLERANCE = .1;		//deviation of bond length
	const unsigned int NUM_ELEMENTS = 118;	//number of elements in nature so far... (don't HAVE to use elements... ;)
//RADII GO HERE
}
#endif