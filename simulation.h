#ifndef SIMULATION_H
#define SIMULATION_H

using namespace std;	//for now

#include <string>	//strings
#include <fstream>	//file io
#include <iostream>	//for cerr
#include <sstream>	//for stringstream
#include <iomanip>	//for formatting output
#include "K.h"		//namespace for constants
#include "coordinate.h"	//for coordinates
#include "atom.h"	//for atom class



class simulation
{
	private:
		//bool ForEachAtom(void (*function)()		//this would be sooooooo nice

	public:
		//constructors------------
		simulation();
		//prototypes--------------
		bool ReadData(string);			//reads datafile	XXX may want to return number of atoms read in?
		int Associate(void);			//bonds neighbors
		int Hole(coordinate,double);	//creates hole in atoms of radius
		// int Passivate();				//passivates hole
		bool WriteData(string);			//outputs current state to datafile
		//variables-----------------
		// static double lattice[3];
		double multiplier;							//multiplier for system... unused so far...
		unsigned int elementCount[K::MAX_ELEMENTS];	//count of atoms for each element
		string element[K::MAX_ELEMENTS];			//element name
		unsigned int elementNum;					//number of elements
		atom_cls atom[K::MAX_ELEMENTS][K::MAX_ATOMS]/*(*this)*/;	//atoms in system
		// double lattice[3];	***depreciated use atom_cls::lattice[3] now	//lattice axis scale... unit->angstroms?

};
#endif