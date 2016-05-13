#ifndef K_H
#define K_H
namespace K
{
	const unsigned int MAX_ELEMENTS = 10;
	const unsigned int MAX_ATOMS = 1000;
	const unsigned int MAX_BONDS = 26;
	//in future, use array of bond lengths... limiting factor is reading these in from a file... [shrug] easy nuff.
	//const double BOND_LENGTH[ ((MAX_ELEMENTS+1) * MAX_ELEMENTS)/2 ];
	const double BOND_LENGTH = 2.4;		//in Angstroms... 
	//C-C	1.20	1.54
	//C-Si	1.86
	//C-H	1.06	1.12
	//Si-Si	2.33
	//Si-H	1.46
	//H-H	0.74
}
#endif