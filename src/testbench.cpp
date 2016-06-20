#include "testbench.h"
//CONSTANTS
	// const int testbench::MAX_SECTIONS = 10;		//max # of sections
	// const int testbench::MAX_SETTINGS = 10;		//max # of settings per section
	// const int testbench::MAX_FILES = 10;			//max # of datafiles
	// const int testbench::MAX_SCALES = 10;			//max # of scales to make
	// const int testbench::MAX_PORES = 10;			//max # of pores

//SETTINGS
	//INPUT
		string testbench::Input_Filename(string line)
		{
			if(line=="") return "FILENAME";	//default
			unsigned int split = line.find('\t');
			string scaleText;
			string filename;
			stringstream ss;
			//get filename and scale if exists
			if(split == string::npos)	//if no scale, Default will pick it up
				filename = line;
			else
			{
				filename = line.substr(0,split);//get filename
				scaleText = line.substr(split);
				scaleText = Trim(scaleText);	//trim off whitespace
				ss << scaleText;
				for(int a=0; a<3; a++)					//set fileScale
					ss >> fileScale[dataFileNum][a];	//note that this already duplicates if out of output... default built in
			}
			if(!FileExists(filename))
			{
				cerr << '\"' << filename << "\" does not exist or cannot be opened!\n";
				return "";	//XXX NEED TO ADD ERROR CATCHING!
			}
			if(Extension(filename) == "tsv")	//read recursively from this file.
			{
				ifstream header;
				string headerLine;	//a witty remix of headline
				dataFileNum = 0;	//reset... defaut = 1;
				header.open(filename.c_str());
				//allready checked if could be opened.
				while(getline(header,headerLine) && dataFileNum < MAX_FILES)
				{
					headerLine = Trim(headerLine);
					Input_Filename(headerLine);
				}
					// dataFileNum++;	//keep track of number of files
				header.close();	
			}
			else	//assume vasp format
			{
				dataFilename[dataFileNum] =  filename;
				dataFileNum++;
			}
			return "";
		}
	//SCALING
		string testbench::Scaling_Filename(string line)
		{
			if(line=="") return "FILENAME";	//default
			ifstream scaleFile;
			stringstream ss;
			string scaleLine;			
			if(!FileExists(line))
			{
				cerr << '\"' << line << "\" does not exist or cannot be opened!\n";
				return "";	//XXX add error catching!
			}
			scaleFile.open(line.c_str());	//allready checked if is file
			while(getline(scaleFile, scaleLine))	//read each line
			{
				ss.clear();
				ss << scaleLine;
				for(int a=0; a<3; a++)
					ss >> scale[scaleNum][a];
				scaleNum++;
			}
			scaleFile.close();
			return "";
		}
	//BONDING
		string testbench::Bonding_Tolerance(string line)
		{
			if(line=="") return "TOLERANCE";	//default
			stringstream ss;
			ss << line;
			// ss >> K::BOND_TOLERANCE;
			return "";
		}
		string testbench::Bonding_Lengths(string line)
		{
			if(line=="") return "LENGTHS";	//default
			
			return "";
		}
	//PORE
		/*string testbench::Pore_Number(string line)
		{
			if(line=="") return "NUMBER";	//default
			stringstream ss;
			ss << line;
			ss >> poreNum;			
			return "";
		}*/
		string testbench::Pore_Coordinate(string line)
		{
			if(line=="") return "COORDINATE";	//default
			stringstream ss;			//for conversion between strings and numbers
			ifstream coordFile;			//ifile obj for reading recursively
			string coordLine;			//line in coordfile
			unsigned int randCoordNum;	//number of random coords to generate
			unsigned int mark;					//marks place in tag

			if(Extension(line) == "tsv")	//if arg is file
			{
				coordFile.open(line.c_str());
				if(coordFile.fail())
				{
					cerr << "Coordinate File \"" << line << "\" Does not exist!\n";
					return "";	//XXX need error catching
				}
				while(getline(coordFile,coordLine) && poreNum < MAX_PORES)
				{
					coordLine = Trim(coordLine);	//trim whitespace
					Pore_Coordinate(coordLine);		//recurse
				}
				coordFile.close();
			}
			else if(Uppercase(line.substr(0,6))=="RANDOM")	//if random coord
			{
				mark = line.find('\t');
				if(mark != string::npos)	//if tab found
				{
					line = Trim(line.substr(mark));	//cut off tag and trim
					ss << line;
					ss >> randCoordNum;	//read as number of coords
				}
				else	//default
					randCoordNum = 1;
				for(unsigned int a=0; a<randCoordNum; a++, poreNum++)
					poreCoord[poreNum] = RandCoord();
			}
			else	//if coordinate
			{
				ss << line;
				for(int a=0; a<3; a++)
				{
					ss >> poreCoord[poreNum].ord[a];	//XXX will have to change when dynamic
				}
				poreNum++;	//increment # of pores
			}
			return "";
		}
		string testbench::Pore_Center(string line)
		{
			if(line=="") return "CENTER";	//default
			centering = Uppercase(line[0]);
			switch(centering)
			{
				case 'A':	//atomic
					break;
				case 'C':	//coordinate
					break;
				default:
					cerr << "Centering \'" << centering << "\' not recognized\n";
			}
			return "";
		}
		string testbench::Pore_Radius(string line)
		{
			if(line=="") return "RADIUS";	//default
			stringstream ss;
			ss << line;
			ss >> poreRadius;
			return "";
		}
		/*string testbench::Pore_Iterations(string line)
		{
			if(line=="") return "interactions";	//default
			stringstream ss;
			ss << line;
			ss >> poreIterations;
			return "";
		}*/
		string testbench::Pore_Passivation(string line)
		{
			if(line=="") return "PASSIVATION";	//default
			
			return "";
		}
	//OUTPUT
		string testbench::Output_Path(string line)
		{
			if(line=="") return "PATH";	//default
			path = line;
			return "";
		}
		string testbench::Output_Filename(string line)
		{
			if(line=="") return "FILENAME";	//default
			customName = line;	//strait up copy it over.
			return "";
		}
		string testbench::Output_Convention(string line)
		{
			if(line=="") return "CONVENTION";	//default
			convention = line;	//strait up copy it over.
			return "";
		}
		string testbench::Output_Delimiter(string line)
		{
			if(line=="") return "DELIMITER";	//default
			switch(line[0])
			{
				case 't':
					delimiter += "\t";
					break;
				case 's':
					delimiter += " ";
					break;
				default:
					delimiter += line;
			}
			
			return "";
		}
		string testbench::Output_Extension(string line)
		{
			if(line=="") return "EXTENSION";	//default
			extension = line;
			return "";
		}
//----------------------------------------------------------------------------------------
//string operations
string testbench::Trim(string line)
{
	static const char whitespace[] = {'\0', ' ', '\a', '\b', '\t', '\n', '\v', '\f', '\r'};
	static const unsigned int whiteNum = sizeof(whitespace)/sizeof(char);
	
	for(int a=0; a<2; a++)
		for(unsigned int b=0; b<whiteNum; b++)
			if( line[a==0 ? 0:line.length()-1] == whitespace[b])
			{
				line.erase(a==0 ? 0:line.length()-1,1);
				line = Trim(line);
			}
	return line;
}
string testbench::I_Str(int a)
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
char testbench::Uppercase(char c)
{
	if(c>='a' && c<='z')
		return c-'a'+'A';
	return c;
}
string testbench::Uppercase(string s)
{
	for(unsigned int a=0; a<s.length(); a++)
		s[a] = Uppercase(s[a]);
	return s;
}
//file operations
bool testbench::FileExists(string filename)
{
	ifstream file;
	file.open(filename.c_str());
	if(file.fail())
		return 0;
	file.close();
	return 1;
}
string testbench::Extension(string filename)
{
	unsigned int period = filename.find('.');
	if(period == string::npos)
		return "";
	return filename.substr(period+1);
}
string testbench::CreateFilename(void)
{
	string fn;
	fn += customName;
	for(unsigned int c=0; c<convention.length(); c++)
	{
		if(c>0 || fn.length()>0)
			outFilename += delimiter;
		switch(Uppercase(convention[c]))//make output name
		{
			case 'F':
				fn += "f";
				break;
			case 'S':
				fn += "s";
				break;
			case 'P':
				fn += "p";
				break;
			case 'N':
				fn += I_Str(outFileCount);
				break;
			default:
				cerr << "Convention \'" << convention[c] << "\' was not recognized\n"; 
		}
	}
	return fn;
}
//coordinate operations
coordinate testbench::RandCoord(void)
{
	coordinate co;
	for(int a=0; a<3; a++)
		co.ord[a] = double(rand())/RAND_MAX;
	return co;
}
//---------------------------------------------------------------------------------------------
//constructor
testbench::testbench(void)
{
	//used to count functions
	unsigned int i = 0;
	unsigned int n[MAX_SECTIONS] = {0};
	//initialize function pointers and counts
	section[i] = "INPUT";
		setting[i][n[i]] = &testbench::Input_Filename;		n[i]++;
		i++;
	section[i] = "SCALING";
		setting[i][n[i]] = &testbench::Scaling_Filename;	n[i]++;
		i++;
	section[i] = "BONDING";
		setting[i][n[i]] = &testbench::Bonding_Tolerance;	n[i]++;
		setting[i][n[i]] = &testbench::Bonding_Lengths;		n[i]++;
		i++;
	section[i] = "PORE";
		// setting[i][n[i]] = &testbench::Pore_Number;			n[i]++;
		setting[i][n[i]] = &testbench::Pore_Coordinate;		n[i]++;
		setting[i][n[i]] = &testbench::Pore_Center;			n[i]++;
		setting[i][n[i]] = &testbench::Pore_Radius;			n[i]++;
		// setting[i][n[i]] = &testbench::Pore_Iterations;		n[i]++;
		setting[i][n[i]] = &testbench::Pore_Passivation;	n[i]++;
		i++;
	section[i] = "OUTPUT";
		setting[i][n[i]] = &testbench::Output_Path;			n[i]++;
		setting[i][n[i]] = &testbench::Output_Filename;		n[i]++;
		setting[i][n[i]] = &testbench::Output_Convention;	n[i]++;
		setting[i][n[i]] = &testbench::Output_Delimiter;	n[i]++;
		setting[i][n[i]] = &testbench::Output_Extension;	n[i]++;
		i++;
	//initialize the numbers of functions
	sectionNum = i;
	for(unsigned int a=0; a<i; a++)
		settingNum[a] = n[a];
}
int testbench::Read(string inputName)
{
	ofstream copy;	//copy of input file
	ifstream input;	//infile
	string line;	//line of file
	string tag;		//tag to compare
	stringstream ss;
	unsigned int sectionIndex = MAX_SECTIONS;	//index of section
	unsigned int settingIndex = MAX_SETTINGS;	//index of setting

	input.open(inputName.c_str());
	if(input.fail())
	{
		cerr << "Failed to open: \"" << inputName << "\"" << endl;
		return -1;
	}
	//read file line by line
	while(getline(input,line))
	{
		if(line[0]=='#')	//comment
			continue;
		if(line[0]!='\t')	//if heading text, set section
		{
			tag = Uppercase(Trim(line));	//cut off trailing whitespace and make uppercase
			for(sectionIndex=0; sectionIndex < sectionNum; sectionIndex++)	//find tag
				if(tag == section[sectionIndex])
					break;
			if(sectionIndex >= sectionNum)				//if not a section
				cerr << "\"" << tag  << "\"" << " Not recognized as a section" << endl;	//careful! there is no line-feed on the end of the lines!
		}
		else if(sectionIndex < sectionNum)	//line[0]=='\t' && the section is recognized
		{
			line = Trim(line);						//remove tab and any leading/trailing whitespace
			tag = line.substr(0,line.find('\t'));	//retreive tag
			tag = Uppercase(Trim(tag));				//trim any whitespace from tag and make uppercase
			line = line.substr(tag.length());		//cut off tag
			line = Trim(line);						//remove any whitespace

			for(settingIndex = 0; settingIndex < settingNum[sectionIndex]; settingIndex++)	//find tag
				if(tag == (this->*setting[sectionIndex][settingIndex])(""))	//search for function matches	//XXX ugh, default values for function pointers are a pain...
				{
					(this->*setting[sectionIndex][settingIndex])(line);	//pass line over to be used as a setting
					break;										//discontinue search
				}
			if(settingIndex >= settingNum[sectionIndex])	//unrecognized
			{
				cerr << "\"" << tag << "\"" << " Not recognized as a setting under " << section[sectionIndex] << endl;
				continue;
			}				
		}
	}
	if(!input.eof())
	{
		cerr << "there was trouble reading the entirity of: \"" << inputName << "\"\n";
		return -1;
	}

	input.close();
	input.open(inputName.c_str());
	copy.open( (path+CreateFilename()+"-settings.tsv").c_str() );
	while(getline(input,line))
		copy << line << '\n';
	copy.close();
	input.close();
	//make scale models
	//make 


	return 0;
}
int testbench::Test(void)
{
	unsigned int f,s,p;	//indexing
	for(f=0; f<dataFileNum; f++)//for each datafile
		for(s=0; s<scaleNum; s++)//for each scale
		{
			sim << dataFilename[f];		//read in file
			sim.Scale(fileScale[f]);	//file-specific scale
			sim.Scale(scale[s]);		//scale
			sim.Associate();			//create bonds
			for(p=0; p<poreNum; p++)//for each pore
			{
				// sim.PassivatedHole(poreRadius, poreCoord[p]);
				sim.PassivatedHole(poreRadius, &poreCoord[p]);
				// for(int i=0; i<poreIterations; i++)//for each iteration
				// {
				// 	sim << dataFilename[f];	//XXX replace with sim = sim2, etc. (faster rates)
				// 	sim.Scale(fileScale[f]);
				// 	sim.Scale(scale[s]);
				// 	removed = sim.PassivatedHole(poreRadius, poreCoord[p]);
				// }
				//untill new pore
					//make and compare atoms removed
			}
			outFilename = CreateFilename();
			sim >> (path+outFilename+extension);//output
			outFileCount++;
		}
	return 0;
}
int testbench::Run(string inputName)
{
	Default();			//set all defaut settings
	if(Read(inputName))	//incorporate settings from file
		return 1;
	Test();
	return 0;
}
void testbench::Default(void)
{
	dataFileNum = 0;//no datafiles... incremented as read from.
	scaleNum = 1;
	poreNum = 0;	//no pores
	poreRadius = 0;	//no size to pores
	path = "";
	customName = "";
	convention = "";	//default output defiined after filename
	delimiter = "";		//no delimiter by default
	extension = ".vasp";
	outFileCount = 0;	//number of files outputed so far

	unsigned int a;	//indexing

	for(a=0; a<MAX_FILES; a++)
		for(int b=0; b<3; b++)
			fileScale[a][b] = 1;
	for(a=0; a<MAX_SCALES; a++)
		for(int b=0; b<3; b++)
			scale[a][b] = 1;
	for(a=0; a<MAX_PORES; a++)
		poreCoord[a]=.5;

	//seed rand
	srand(time(NULL));
	return;
}