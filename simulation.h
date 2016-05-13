#ifndef SIMULATION_H
#define SIMULATION_H

#include "K.h"		//namespace for constants
#include "coordinate.h"	//for coordinates
#include "atom.h"	//for atom class
#include <string>	//strings
#include <fstream>	//file io
#include <iostream>	//for cerr
#include <sstream>	//for stringstream


class simulation
{
	private:
		double multiplier;							//multiplier for system... unused so far...
		
		string element[K::MAX_ELEMENTS];			//element name
		unsigned int elementNum;					//number of elements
		unsigned int elementCount[K::MAX_ELEMENTS];	//count of atoms for each element

	public:
		//constructors------------
		simulation();
		//prototypes--------------
		bool ReadData(string);
		int Associate(void);
		int Hole(coordinate);	//creates hole in atoms
		bool WriteData(string);
		//variables-----------------
		atom_cls atom[K::MAX_ELEMENTS][K::MAX_ATOMS](*this);	//atoms in system
		double lattice[3];							//lattice axis scale... unit->angstroms?

};
#endif