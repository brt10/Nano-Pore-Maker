using namespace std;	//for now... easier

#include <iostream>	//for cout
#include <fstream>	//for file io
#include <string>	//strings
#include "simulation.h"
#include "coordinate.h"

int main()
{
	//prototypes
	string I_Str(int);
	//constants
	const unsigned int N = 5;	//number of distinct outcomes
	const unsigned int SCALE[2] = {3,1};	//scale of system (3c,64)
	const double STEP = .1;		//step value for radius
	const string ipath = "data/";
	const string opath = "holyData/";
	const string polytype[2] = {"3C","2H"};	//name of polytype
	const string ext = ".vasp";
	const string data[2] = {"UnitCell","64"};
	const string datafilename = "data.tsv";

	//variables
	string filename; 	//filename of output
	simulation sim;		//simulation object
	ofstream datafile; 	//datafile object
	unsigned int E=0;	//specifies element for center
	double holeRadius =0;	//radius of hole primed for first step
	unsigned int lastNum=0;	//last distinct number of atoms removed
	unsigned int removed=lastNum;	//# of atoms removed
	// double cell[3] = {.25,.5,.5};
	
	datafile.open((opath+datafilename).c_str());
	if(datafile.fail())
	{
		cerr << "unable to open datafile\n";
		return 0;
	}

	//set precision for output
	datafile << fixed;
	datafile << setprecision(K::PRECISION);

	//header
	datafile << "Step value of: " << STEP << endl;
	datafile << "Fname\t%Si\t%C\t%H\t#Si\t#C\t#H\tDensity\tRadius\tL_X\tL_Y\tL_Z\tplytyp\telement\tNUM\n";

	//for each polytype
	for(int p=0; p<2; p++)
	{	//output standard
		sim << (ipath+data[p]+ext);	//read in
		sim.Scale(SCALE[p]);		//scale
		filename = (polytype[p]+"00");
		sim >> (opath+filename+ext);
		datafile << (filename+ext) <<'\t';	///Fname;
		for(int e=0; e<sim.elementNum; e++)	//%
			datafile << sim%e << '\t';
		datafile << "XXX\t";
		for(int e=0; e<sim.elementNum; e++)	//#
			datafile << sim.Extant(e) << '\t';
		datafile << "XXX\t";
		datafile << sim.Density() << '\t';	//density
		datafile << "XXX\t";					//radius = NA
		for(int a=0; a<3; a++)			//lattice
			datafile << sim.lattice[a] << '\t';
		datafile << polytype[p] << '\t';	//polytype
		datafile << "XXX\tXXX\n";				//element = NA; NUM = NA

		for(E=0; E<2; E++)//for each element center
		{
			holeRadius = 0;	//initialize hole rad
			//set lastNumber removed
			sim << (ipath+data[p]+ext);	//read in
			lastNum = sim.PassivatedHole(holeRadius, sim.Center(E));	//make hole with r=0;
			for(int n=0; n<N; n++)//for each distinct outcome
			{
				do{
					holeRadius += STEP;
					sim << (ipath+data[p]+ext);//read in data
					sim.Standardize();		//standardize model
					sim.Scale(SCALE[p]);	//scale for the polytype
					sim.Associate();		//bond atoms
					removed = sim.PassivatedHole(holeRadius, sim.Center(E));//make hole at rough center on an atom

				}while(removed==lastNum);
				lastNum = removed;	//set as new last
				//make filename
				filename = (polytype[p]+I_Str(E+1)+I_Str(n+1));
				//write sims to file
				sim >> (opath+filename+ext);
				//write data
				datafile << (filename+ext) << '\t';	//Fname
				for(int e=0; e<sim.elementNum; e++)	//%
					datafile << sim%e << '\t';
				for(int e=0; e<sim.elementNum; e++)	//#
					datafile << sim.Extant(e) << '\t';
				datafile << sim.Density() << '\t';	//density
				datafile << holeRadius-STEP << '\t';//radius
				for(int a=0; a<3; a++)			//lattice
					datafile << sim.lattice[a] << '\t';
				datafile << polytype[p] << '\t';	//polytype
				datafile << sim.element[E] << '\t';	//element
				datafile << n << endl;				//NUM
				cout << p << ',' << E << ',' << n << '\n';
			}
		}

	}
	datafile.close();
	return 0;
}
string I_Str(int a)
{
	if(a==0)	return "0";
	
	string s;	//string
	bool n=0;	//negative
	
	if(a<0)
	{
		n=1;
		a=-a;
	}
	while(a!=0)
	{
		s=char((a%10)+'0')+s;
		a/=10;
	}
	if(n) s='s'+s;
	return s;
}