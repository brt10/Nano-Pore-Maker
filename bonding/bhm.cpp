//Bond Header Maker 1.0
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int main()
{
	//constants
		const string RAD_TAG = "//RADII GO HERE";					//this line is replaced in the template with the bonding information in the tsv
		const string SYM_LINE = "	const std::string SYM[NUM_ELEMENTS] = {\"";		//line that primes the symbols
		const string RAD_LINE = "	const double COV_RAD[NUM_ELEMENTS] = {";			//line that primes the radii
		const string MASS_LINE = "	const double MASS[NUM_ELEMENTS] = {";				//line that precedes mass
		const string SYM_CAP = "\"};\n";									//text that caps the sym line
		const string RAD_CAP = "};\n";										//caps the radius line
		const string MASS_CAP = RAD_CAP;
		const string SYM_DEL = "\",\"";		//delimiter for symbol values
		const string RAD_DEL = ",";		//delimiter for radius values
		const string MASS_DEL = RAD_DEL;
		const string MASS_SUFFIX = "/MOLE";
		const unsigned int NUM_ELEMENTS = 118;	//XXX kinda hacky, need to be sure that this is not greater than other stuff...
	//variables, objects
		string line;		//line of text...
		unsigned int a;	//indexing	//could save a variable by going backwards...
	//from radii.tsv...
		string sym[NUM_ELEMENTS];			//element symbol
		double rad[NUM_ELEMENTS] = {0};		//covalent radus
		double mass[NUM_ELEMENTS] = {0};	//atomic mass
		unsigned int n;		//atomic number
	//filenames
		string tsvFn = "radii.tsv";
		string tempFn = "template.h";
		string headerFn = "K.h";
	//file objs
		ifstream tsv;		//radii.tsv
		ifstream temp;		//template.h
		ofstream header;	//K.h

	//open files
		//open tsv
		tsv.open(tsvFn.c_str());
		if(tsv.fail())
		{
			cerr << "Failed to open: \"" << tsvFn << "\" as a bond file\n";
			return 1;
		}
		//open template
		temp.open(tempFn.c_str());
		if(temp.fail())
		{
			cerr << "Failed to open: \"" << tempFn << "\" as a template file\n";
			tsv.close();
			return 1;
		}
		//open header (overwrite)
		header.open(headerFn.c_str());
		if(header.fail())
		{
			cerr << "Failed to open: \"" << headerFn << "\" as a header file\n";
			tsv.close();
			temp.close();
			return 1;
		}
	//copy into header till radii tag
		while(getline(temp,line) && line != RAD_TAG)
			header << line << endl;
	//get tsv info into array
		getline(tsv,line);	//throw out the header
		while(!(tsv >> n).eof())
			tsv >> mass[n-1] >> rad[n-1] >> sym[n-1];
	//write tsv info to file
		//write the symbols
		header << SYM_LINE;
		for(a=0; a<NUM_ELEMENTS; ++a)
		{
			if(a>0) header << SYM_DEL;
			header << sym[a];
		}
		header << SYM_CAP;
		//write the radii
		header << RAD_LINE;
		for(a=0; a<NUM_ELEMENTS; ++a)
		{
			if(a>0) header << RAD_DEL;
			header << rad[a];
		}
		header << RAD_CAP;
		//write the mass
		header << MASS_LINE;
		for(a=0; a<NUM_ELEMENTS; ++a)
		{
			if(a>0) header << MASS_DEL;
			header << mass[a] << MASS_SUFFIX;
		}
		header << MASS_CAP;
	//close tsv
		tsv.close();
	//finish copying in template
		while(getline(temp,line))
			header << line << endl;
	//close template and header
		temp.close();
		header.close();
	//recompile project
		cout << " +------------------------------------------+" << endl;
		cout << " | Your Project will need to be recompiled! |" << endl;
		cout << " +------------------------------------------+" << endl;
	return 0;
}