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
		void (*ForEachAtom)(atom_cls*)		//this would be sooooooo nice

	public:
		//constructors------------
		simulation();
		//prototypes--------------
		bool ReadData(string);			//reads datafile	XXX may want to return number of atoms read in?
		int Associate(void);			//bonds neighbors
		int Hole(coordinate,double);	//creates hole in atoms of radius
		// int Passivate();				//passivates hole
		bool WriteData(string);			//outputs current state to datafile
		int UnitCell(double[3]);		//makes the scale given, and makes the atoms within it into a unit cell (returns #atoms in cell)
		void CopyCell(double[3]);		//makes a mosaic of the current cell to the given scale. (may overload as int... not too important)
		void Scale(double[3]);			//scales the model to a certain size, retaining structure, and bond lengths

		double RealDistance(coordinate, coordinate);	//the real distance to a coord
		//void BreakBond(atom_cls*);		//may add error catching to this later...
		double ModDistance(atom_cls*, atom_cls*);	//shortest distance to another atom

		// void (*ForEachAtom)(unsigned int, unsigned int);	//will be usefull... buld in error catching later :)
		//variables-----------------
		string title;								//title of the file :P
		double multiplier;							//multiplier for system... unused so far...
		unsigned int elementCount[K::MAX_ELEMENTS];	//count of atoms for each element
		string element[K::MAX_ELEMENTS];			//element name
		unsigned int elementNum;					//number of elements
		string tag;									//unknown tag... (direct)
		atom_cls atom[K::MAX_ELEMENTS][K::MAX_ATOMS];	//atoms in system
		double lattice[3];			//XXX change to vector or coord later!				//lattice axis scale... unit->angstroms?
		//coordinate lattice;

};
#endif