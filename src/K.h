#ifndef BONDING_H
#define BONDING_H

#include <string>	//for strings
#include "strops.h"	//for Uppercase()
namespace K
{
	// const unsigned int MAX_ELEMENTS = 5/*118*/;		//max number of elements in simulation
	// const unsigned int MAX_ATOMS = 2000/*3000*/;	//max number of atoms in simulation
	// const unsigned int MAX_BONDS = 26;		//max bonds per atom... should probaly set to MAX_ELEMENTS*MAX_ATOMS to advoid complications?
	const unsigned int PRECISION = 6;		//precision in outputs
	const double MOLE = 6.022e23;			//avagadro's number
	const double BOND_TOLERANCE = .1;		//deviation of bond length
	const unsigned int NUM_ELEMENTS = 118;	//number of elements in nature... don't HAVE to use elements... :P
	// const std::string SYM[NUM_ELEMENTS] = {"H","C","Si"};
	// const double COV_RAD[NUM_ELEMENTS] = {0.32,0.76,0.9};
	// const double MASS[NUM_ELEMENTS] = {1.0079/MOLE,6/MOLE, 20/MOLE};
	const std::string SYM[NUM_ELEMENTS] = {"H","He","Li","Be","B","C","N","O","F","Ne","Na","Mg","Al","Si","P","S","Cl","Ar","K","Ca","Sc","Ti","V","Cr","Mn","Fe","Co","Ni","Cu","Zn","Ga","Ge","As","Se","Br","Kr","Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe","Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn","Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","","","","","","","","",""};
	const double COV_RAD[NUM_ELEMENTS] = {0.32,0.93,1.23,0.9,0.82,0.77,0.75,0.73,0.72,0.71,1.54,1.36,1.18,1.11,1.06,1.02,0.99,0.98,2.03,1.74,1.44,1.32,1.22,1.18,1.17,1.17,1.16,1.15,1.17,1.25,1.26,1.22,1.2,1.16,1.14,1.12,2.16,1.91,1.62,1.45,1.34,1.3,1.27,1.25,1.25,1.28,1.34,1.48,1.44,1.41,1.41,1.36,1.33,1.31,2.35,1.98,1.69,1.65,1.65,1.64,1.63,1.62,1.85,1.61,1.59,1.59,1.58,1.57,1.56,1.74,1.56,1.44,1.34,1.3,1.28,1.26,1.27,1.3,1.34,1.49,1.48,1.47,1.46,1.46,1.45,0,0,0,0,1.65,0,1.42,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	const double MASS[NUM_ELEMENTS] = {1.0079/MOLE,4.0026/MOLE,6.941/MOLE,9.0122/MOLE,10.811/MOLE,12.0107/MOLE,14.0067/MOLE,15.9994/MOLE,18.9984/MOLE,20.1797/MOLE,22.9897/MOLE,24.305/MOLE,26.9815/MOLE,28.0855/MOLE,30.9738/MOLE,32.065/MOLE,35.453/MOLE,35.453/MOLE,39.948/MOLE,40.078/MOLE,44.9559/MOLE,47.867/MOLE,50.9415/MOLE,51.9961/MOLE,54.938/MOLE,55.845/MOLE,58.9332/MOLE,58.9332/MOLE,63.546/MOLE,65.39/MOLE,69.723/MOLE,72.64/MOLE,74.9216/MOLE,78.96/MOLE,79.904/MOLE,83.8/MOLE,85.4678/MOLE,87.62/MOLE,88.9059/MOLE,91.224/MOLE,92.9064/MOLE,95.94/MOLE,98/MOLE,101.07/MOLE,102.906/MOLE,106.42/MOLE,107.868/MOLE,112.411/MOLE,114.818/MOLE,118.71/MOLE,121.76/MOLE,121.76/MOLE,127.6/MOLE,131.293/MOLE,132.905/MOLE,137.327/MOLE,138.905/MOLE,140.116/MOLE,140.908/MOLE,144.24/MOLE,145/MOLE,150.36/MOLE,151.964/MOLE,157.25/MOLE,158.925/MOLE,162.5/MOLE,164.93/MOLE,167.259/MOLE,168.934/MOLE,173.04/MOLE,174.967/MOLE,178.49/MOLE,180.948/MOLE,183.84/MOLE,186.207/MOLE,190.23/MOLE,192.217/MOLE,195.078/MOLE,196.966/MOLE,200.59/MOLE,204.383/MOLE,207.2/MOLE,208.98/MOLE,209/MOLE,210/MOLE,222/MOLE,223/MOLE,226/MOLE,227/MOLE,227/MOLE,232.038/MOLE,232.038/MOLE,238.029/MOLE,238.029/MOLE,244/MOLE,247/MOLE,247/MOLE,251/MOLE,252/MOLE,257/MOLE,258/MOLE,259/MOLE,262/MOLE,262/MOLE,262/MOLE,266/MOLE,266/MOLE,266/MOLE,268/MOLE,0/MOLE,0/MOLE,0/MOLE,0/MOLE,0/MOLE,0/MOLE,0/MOLE,0/MOLE,0/MOLE};
	//functions
	unsigned int AtomicNumber(std::string);
}
#endif
