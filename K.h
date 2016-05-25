#ifndef K_H
#define K_H
namespace K
{
	const unsigned int MAX_ELEMENTS = 10;
	const unsigned int MAX_ATOMS = 1000;
	const unsigned int MAX_BONDS = 26;		//max bonds per atom... should probaly set to MAX_ELEMENTS*MAX_ATOMS to advoid complications?
	//in future, use array of bond lengths... limiting factor is reading these in from a file... [shrug] easy nuff.
	//const double BOND_LENGTH[ ((MAX_ELEMENTS+1) * MAX_ELEMENTS)/2 ];
	// const double BOND_LENGTH = 1.9;		//in Angstroms... 
	const double DEV = 1.1;	//deviation of bond length
	//https://en.wikipedia.org/wiki/Bond_length
	//									Si 		C 		H
	const double BOND_LENGTH[3][3] ={	{2.33*DEV,	1.86*DEV,	1.46*DEV},	//Si
										{1.86*DEV,	1.54*DEV,	1.10*DEV},//C
										{1.46*DEV,	1.10*DEV,	0.74*DEV}};//H
	const double BOND_LENGTH_H = 1;
	const unsigned int PRECISION = 4;
	const double MOLE = 6.022e23;//		Si 				C 				H
	const double MASS[MAX_ELEMENTS] = {28.085/MOLE ,12.011/MOLE, 1.00794/MOLE};
	// unsigned int Si = 0;
	// unsigned int C = 1;
	//C-C	1.20	1.54(sp3)
	//C-Si	1.86
	//C-H	1.06	1.12
	//Si-Si	2.33
	//Si-H	1.46
	//H-H	0.74
}
#endif