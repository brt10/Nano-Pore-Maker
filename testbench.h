#include <iostream>	//cout, cerr, etc.
#include <fstream>	//file io
#include <string>	//strings
#include <sstream>	//for parsing strings into integers and doubles especially
#include "simulation.h"	//for simulations
#include "coordinate.h"	//for coordinates of holes, etc.

using namespace std;	//easier for now....

//for setting function integration
typedef string (*functionP)(string);

class testbench
{
	private:
		const unsigned int MAX_SECTIONS;		//max # of sections
		const unsigned int MAX_SETTINGS;		//max # of settings per section
		const unsigned int MAX_FILES;			//max # of datafiles
		const unsigned int MAX_SCALES;			//max # of scales to make

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
		char distribution;						//Random, Centered, File
		string holeFilename;
		double poreRadius;						//radius of pores (only nessesary if constant)
		unsigned int poreIterations;			//iterations of pore sizes
		//MODELS
		
		string bondLengthFilename;

		string path;
		string convention;
		string delimeter;
		string extension;

		ifstream data;
		ifstream hole;
		//INPUT
		string Input_Filename(string line = "");
		//SCALING
		string Scaling_Filename(string line = "");
		//BONDING
		string Bonding_Tolerance(string line = "");
		string Bonding_Lengths(string line = "");
		//PORE
		string Pore_Number(string line = "");
		string Pore_Centering(string line = "");
		string Pore_Radius(string line = "");
		string Pore_Iterations(string line = "");
		string Pore_Passivation(string line = "");
		//OUTPUT
		string Output_Path(string line = "");
		string Output_Convention(string line = "");
		string Output_Delimeter(string line = "");
		string Output_Extension(string line = "");
		//settings
		string section[MAX_SECTIONS];
		FunctionP setting[MAX_SETTINGS];

		void Default(void);				//seta all values to default
		//string manipulation
		string Trim(string);		//trims from and back of string for whitespace, etc.
		string I_Str(int);			//integer -> string
		//file operations
		bool FileExists(string);	//returns 1 if file exists
		string Extension(string);	//returns the extension of file without the period
		int Read(string);	//reads settings from file
		int Test(void);			//runs test with settings

	public:
		testbench(void);		//constructor
		int Run(string);	//runs the testbench from specifications in file
};


// INPUT		
// 	filename		<2H.vasp (unsigned int scale) | files.tsv>
// SCALING
// 	filename		scaling.tsv
// BONDING		
// 	tolerance		(double)
// 	lengths			<default | bonding.tsv>
// PORES		
// 	number			(unsigned int)
// 	centering		<atomic | coordinate>
// 	distribution	<random | centered | (holes.tsv)>
// 	radius			(double)
// 	iterations		<all | (unsigned int)>
// OUTPUT		
// 	path			(directory name)
// 	convention		<Index,Polytype,Center,Radius>
// 	delimeter		<none | space | tab | char(character)>
// 	extension		(extension name)