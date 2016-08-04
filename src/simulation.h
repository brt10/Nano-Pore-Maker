#ifndef SIMULATION_H
#define SIMULATION_H

using namespace std;	//for now

#include <string>	//strings
#include <fstream>	//file io
#include <iostream>	//for cerr
#include <sstream>	//for stringstream	(reading in data)	XXX should be able to replace with reading characters
#include <iomanip>	//for formatting output to file
#include <vector>	//for vectors
#include <map>		//for maps
#include <algorithm>//for for_each, etc.
// #include <numeric>	//for accumulate
//src
#include "strops.h"	//string operations
#include "K.h"		//namespace for constants
#include "coordinate.h"	//for coordinates
#include "atom.h"	//for atom class

class simulation
{
	private:
		//void (*ForEachAtom)(atom_cls*)		//this would be sooooooo nice

	public:
		//constructors------------
		simulation();
		//prototypes--------------
		void ClearData(void);			//clears all data in class
		bool ReadData(const string);	//reads datafile	XXX may want to return number of atoms read in?
		bool operator<<(const string);
		void Standardize(void);			//moves all atoms to [0,1) (usefull after reading in data) XXX would be nice to know how many were changed

		void Disassociate(void);		//unbind all
		int Associate(void);			//bonds neighbors
		bool Bond(atom_cls*, atom_cls*);//bond, Covalent Bond
		bool Bond(atom_cls*[2]);
		void Passivate(atom_cls*, atom_cls*, string="H");	//passivates hole
		bool WriteData(const string);	//outputs current state to datafile
		bool operator>>(const string);
		bool CopyCell(unsigned int, unsigned int);		//makes a mosaic of the current cell to the given scale. (may overload as int... not too important)
		bool Scale(unsigned int[3]);
		bool Scale(unsigned int);
		bool Scale(double[3]);			//scales the model to a certain size, retaining structure, and bond lengths
		int Trim(void);					//trims off all atoms that are outside of bounds or non-extant
		void RemoveAtom(vector<atom_cls*>::iterator&);	//removes atom from sim.
		int PassivatedPore(double, coordinate* center=0, string="H");	//makes a passivated hole by recursion.
		int PassivatedPore(double, vector<atom_cls*>::iterator&, string="H", coordinate* center=0);	//fastest hole-maker. (specify hole by atom)
		vector<atom_cls*>::iterator Closest(coordinate, string, int=1);	//closest element to the specified coordinate
		vector<atom_cls*>::iterator Closest(coordinate, unsigned int=-1, int=1);	//returns the atom closest to the coordinate of given element
		vector<atom_cls*>::iterator Center(unsigned int=(unsigned int)-1);	//returns the center most atom of specified element;
		//data
		double Volume(void);				//volume of lattice in cm^3
		double Mass(void);					//mass of extant atoms
		unsigned int Atoms(string);		//#atoms of type in sim
		unsigned int Atoms(unsigned int);	//#extant atoms of element
		unsigned int Atoms(void);			//total # of extant atoms
		double Density(void);				//density of system g/cm^3
		double operator%(const string);//percent of element
		double operator%(const unsigned int);//percent of element
		//distances
		double RealDistance(coordinate, coordinate);	//the real distance to a coord
		double ModDistance(coordinate, coordinate);		//shortest distance to another atom
		double ModDistance(atom_cls*, atom_cls*);
		//variables-----------------
		string title;								//title of the file :P
		double multiplier;							//multiplier for system... unused so far...
		
		unsigned int ElementNum(void);	//number of elements

		string tag;									//unknown tag... (direct)
		vector<atom_cls*> atom;						//vector of all atom*s (unsorted by element)
		// map<unsigned int, unsigned int> element;	//map of atomic numbers to counts
		double lattice[3];			//XXX change to vector or coord later!				//lattice axis scale... unit->angstroms?
};
#endif