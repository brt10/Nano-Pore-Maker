//Bond Header Maker 1.0
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int main()
{
	//constants
	const string RAD_TAG = "//RADII GO HERE";					//this line is replaced in the template with the bonding information in the tsv
	const string SYM_LINE = "	const std::string ELE_SYM = {";		//line that primes the symbols
	const string RAD_LINE = "	const double COV_RAD = {";			//line that primes the radii
	const string CAP = "};\n";										//text that caps the sym and rad lines
	const MAX_ELEMENTS = 118;	//XXX kinda hacky, need to be sure that this is not greater than other stuff...
	//variables, objects
	string line;		//line of text...
	unsigned int a,b;	//indexing	//could save a variable by going backwards...
	//from radii.tsv...
	string sym[MAX_ELEMENTS];			//element symbol
	double rad[MAX_ELEMENTS];		//covalent radus
	unsigned int n[MAX_ELEMENTS];		//atomic number
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
	a=0;
	do
	{
		tsv >> sym[a] >> n[a] >> radius[a];
		++a;
	}
	while(!tsv.eof() && a<MAX_ELEMENTS);
	//write tsv info to file
		//write the symbols
		header << SYM_LINE;
		for(b=0; b<a; ++b)
		{
			header << sym[b]
		}
		header << CAP;
		//write the radii
		header << SYM_LINE;
		for(b=0; b<a; ++b)
		{
			header << rad[b]
		}
		header << CAP;
	
	//close tsv
	tsv.close();
	//finish copying in template
	while(getline(temp,line))
		header << line << endl;
	//close template and header
	temp.close();
	header.close();
	//recompile project
	return 0;
}