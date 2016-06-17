#ifndef TESTBENCH_H
#define TESTBENCH_H
#include <iostream>	//cout, cerr, etc.
#include <fstream>	//file io
#include <string>	//strings
#include <sstream>	//for parsing strings into integers and doubles especially
#include "simulation.h"	//for simulations
#include "coordinate.h"	//for coordinates of holes, etc.

using namespace std;	//easier for now....

//for setting function integration
// typedef string (*FunctionP)(string);

class testbench
{
	private:
		static const unsigned int MAX_SECTIONS = 10;		//max # of sections
		static const unsigned int MAX_SETTINGS = 10;		//max # of settings per section
		static const unsigned int MAX_FILES = 10;			//max # of datafiles
		static const unsigned int MAX_SCALES = 10;			//max # of scales to make
		static const unsigned int MAX_PORES = 10;			//max # of pores

		unsigned int sectionNum;				//#of sections
		unsigned int settingNum[MAX_SETTINGS];	//#of settings per section
		

		//simulation	//XXX should we have multiple for faster data-transfer?
		simulation sim;

		//INPUT
		unsigned int dataFileNum;				//#of datafiles to read from
		string dataFilename[MAX_FILES];			//names of datafiles
		unsigned int fileScale[MAX_FILES][3];	//scale associated with file; default = {1,1,1}
		//SCALING
		unsigned int scaleNum;					//#scales to make
		unsigned int scale[MAX_SCALES][3];		//scales to apply to each file
		//PORE
		unsigned int poreNum;					//#of pores to make
		char centering;							//Atomic, Coordinate
		coordinate poreCoord[MAX_PORES];		//coordinate of pores
		char distribution;						//Random, File, Coordinate
		string holeFilename;
		double poreRadius;						//radius of pores (only nessesary if constant)
		unsigned int poreIterations;			//iterations of pore sizes
		//OUTPUT
		string path;
		string customName;
		string convention;
		string delimiter;
		string extension;
		string outFilename;
		unsigned int outFileCount;

		//INPUT
		string Input_Filename(string line = "");
		//SCALING
		string Scaling_Filename(string line = "");
		//BONDING
		string Bonding_Tolerance(string line = "");
		string Bonding_Lengths(string line = "");
		//PORE
		string Pore_Number(string line = "");
		string Pore_Coordinate(string line = "");
		string Pore_Centering(string line = "");
		string Pore_Radius(string line = "");
		string Pore_Iterations(string line = "");
		string Pore_Passivation(string line = "");
		//OUTPUT
		string Output_Path(string line = "");
		string Output_Filename(string line = "");
		string Output_Convention(string line = "");
		string Output_Delimiter(string line = "");
		string Output_Extension(string line = "");
		//settings
		string section[MAX_SECTIONS];
		typedef string (testbench::*FunctionP)(string);
		FunctionP setting[MAX_SECTIONS][MAX_SETTINGS];

		void Default(void);				//seta all values to default
		//string manipulation
		string Trim(string);		//trims from and back of string for whitespace, etc.
		string I_Str(int);			//integer -> string
		char Uppercase(char);
		string Uppercase(string);
		//file operations
		bool FileExists(string);	//returns 1 if file exists
		string Extension(string);	//returns the extension of file without the period
		int Read(string);	//reads settings from file
		int Test(void);			//runs test with settings
		string CreateFilename(void);	//creates filename with current settings.

	public:
		testbench(void);		//constructor
		int Run(string);	//runs the testbench from specifications in file
};
#endif