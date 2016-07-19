#include "testbench.h"
//CONSTANTS
	// const int testbench::MAX_SECTIONS = 10;		//max # of sections
	// const int testbench::MAX_SETTINGS = 10;		//max # of settings per section
	// const int testbench::MAX_FILES = 10;			//max # of inputfiles
	// const int testbench::MAX_SCALES = 10;			//max # of scales to make
	// const int testbench::MAX_PORES = 10;			//max # of pores

//SETTINGS
	//INPUT
		string testbench::Input_Filename(string line)
		{
			if(line=="") return "FILENAME";	//default
			unsigned int split = line.find_first_of(DELIMITERS);
			string scaleText;
			string filename;
			stringstream ss;
			//get filename and scale if exists
			if(split == unsigned(string::npos))	//if no scale, Default will pick it up
				filename = line;
			else
			{
				filename = line.substr(0,split);//get filename
				scaleText = line.substr(split);
				scaleText = Trim(scaleText);	//trim off whitespace
				ss << scaleText;
				for(int a=0; a<3; a++)					//set fileScale
					ss >> fileScale[inputFileNum][a];	//note that this already duplicates if out of output... default built in
			}
			//check for existance of file
			if(!FileExists(filename))
			{
				cerr << '\"' << filename << "\" does not exist or cannot be opened!\n";
				return "";	//XXX NEED TO ADD ERROR CATCHING!
			}
			//check if file is list of files
			if(Extension(filename) == "tsv")	//read recursively from this file.
			{
				ifstream header;
				string headerLine;				//a witty remix of headline
				// inputFileNum = 0;				//reset... defaut = 1;
				header.open(filename.c_str());	//allready checked if could be opened.
				while(getline(header,headerLine) && inputFileNum < MAX_FILES)
				{
					headerLine = Trim(headerLine);
					Input_Filename(headerLine);
				}
					// inputFileNum++;	//keep track of number of files
				header.close();	
			}
			else	//assume vasp format
			{
				inputFilename[inputFileNum] =  filename;
				inputFileNum++;
			}
			return "";
		}
	//SCALING
		string testbench::Scaling_Scale(string line)
		{
			if(line=="") return "SCALE";	//default
			
			if(line.find_first_of(DELIMITERS) == string::npos)//if no tabs	-> file of scales
			{
				//check for existance of file
				if(!FileExists(line))
				{
					cerr << '\"' << line << "\" does not exist or cannot be opened as a scale file!\n";
					return "";	//XXX NEED TO ADD ERROR CATCHING!
				}
				//read file
				ifstream header;
				string headerLine;			//a witty remix of headline
				header.open(line.c_str());	//allready checked if could be opened.
				while(getline(header,headerLine) && scaleNum < MAX_SCALES)
				{
					headerLine = Trim(headerLine);
					Scaling_Scale(headerLine);
				}
				header.close();	
			}
			else//if tab seperation -> single addition to scale
			{
				stringstream ss;
				ss << line;
				for(unsigned int a=0; a<3; a++)
					ss >> scale[scaleNum][a];
				scaleNum++;
			}

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
		string testbench::Pore_Coordinate(string line)
		{
			if(line=="") return "COORDINATE";	//default
			stringstream ss;			//for conversion between strings and numbers
			ifstream coordFile;			//ifile obj for reading recursively
			string coordLine;			//line in coordfile
			unsigned int randCoordNum;	//number of random coords to generate
			unsigned int mark;					//marks place in tag
			const string TAG_RANDOM = "RANDOM";
			const string TAG_DISTRIBUTE = "DISTRIBUTE";
			const string TAG_RANDOM_NO = "RANDOMNOOVERLAP";

			if(Extension(line) == "tsv")	//if arg is file
			{
				coordFile.open(line.c_str());
				if(coordFile.fail())
				{
					cerr << "Coordinate File \"" << line << "\" Does not exist! (testbench::PoreCoordinate)" << endl;
					return "";	//XXX need error catching
				}
				while(getline(coordFile,coordLine) && poreNum < MAX_PORES)
				{
					coordLine = Trim(coordLine);	//trim whitespace
					Pore_Coordinate(coordLine);		//recurse
				}
				coordFile.close();
			}
			else if(Uppercase(line.substr(0,TAG_RANDOM.length())) == TAG_RANDOM)			//if random coord
			{
				mark = line.find_first_of(DELIMITERS);
				if(mark != unsigned(string::npos))	//if tab found
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
			else if(Uppercase(line.substr(0,TAG_DISTRIBUTE.length())) == TAG_DISTRIBUTE)	//if evenly distributed:
			{
				//get number of pores desired
				mark = line.find_first_of(DELIMITERS);
				if(mark != unsigned(string::npos))	//if tab found
				{
					line = Trim(line.substr(mark));	//cut off tag and trim
					ss << line;
					ss >> poreDistribute;	//read as number of pores to add
					if(poreDistribute > MAX_PORES)
					{
						cerr << "\"" << poreDistribute << "\" pores is above the max of: " << MAX_PORES << " reverting to that max. (testbench::PoreCoordinate)" << endl;
						poreDistribute = MAX_PORES;
					}
				}
				else	//default
				{
					cerr << "No number of pores selected to distribute, defaults to 1 (testbench::PoreCoordinate)" << endl;
					poreDistribute = 1;
				}
				DistF = &testbench::Poisson;
			}
			else if(Uppercase(line.substr(0,TAG_RANDOM_NO.length())) == TAG_RANDOM_NO)		// no overlap
			{
				//get number of pores desired
				mark = line.find_first_of(DELIMITERS);
				if(mark != unsigned(string::npos))	//if tab found
				{
					line = Trim(line.substr(mark));	//cut off tag and trim
					ss << line;
					ss >> poreDistribute;	//read as number of pores to add
					if(poreDistribute > MAX_PORES)
					{
						cerr << "\"" << poreDistribute << "\" pores is above the max of: " << MAX_PORES << " reverting to that max. (testbench::PoreCoordinate)" << endl;
						poreDistribute = MAX_PORES;
					}
				}
				else	//default
				{
					cerr << "No number of pores selected to distribute, defaults to 1 (testbench::PoreCoordinate)" << endl;
					poreDistribute = 1;
				}
				DistF = &testbench::RandomNoOverlap;
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
			elementCenter = line;
			//XXX add error-checking for center
			return "";
		}
		string testbench::Pore_Radius(string line)
		{
			if(line=="") return "RADIUS";	//default
			stringstream ss;
			const string tag = "from";	//for comparison to tag
			// char trash;
			double rad[3];

			if(line.substr(0,tag.length()) == tag)//for loop
			{
				ss << line;
				for(int a=0; a<3; a++)
				{
					while((ss.peek()<'0'||ss.peek()>'9') && ss.peek()!='.' && !ss.eof())	//char is not numeric or decimal
						ss.get();	//discard
					if(ss.eof())
					{
						cerr << "Not enough args for Radius, setting any unknown " << a << " to 1" << endl;
						rad[a] = 1;
						continue;
					}
					ss >> rad[a];
				}
				poreRadMin = rad[0];
				poreRadMax = rad[1];
				poreRadStep= rad[2];
				if(poreRadStep == 0)	//infinate loop catching
				{
					cerr << "A step of ZERO won't get you very far... ;)\t Defaulting to 1.0" << endl;
					poreRadStep = 1;
				}
			}
			else
			{
				ss << line;
				ss >> poreRadMin;
				poreRadMax = poreRadMin;	//for just one size of pore
			}
			return "";
		}
		string testbench::Pore_Passivation(string line)
		{
			if(line=="") return "PASSIVATION";	//default
			if(Uppercase(line)=="NONE") passivation = "";	//either "none" or "" (case insensitive will set no passivation)
			else passivation = line;
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
			
			delimiter = Uppercase(line);
			if(delimiter == "TAB") delimiter = '\t';
			else if(delimiter == "SPACE") delimiter = ' ';
			else delimiter = line;
			
			return "";
		}
		string testbench::Output_Extension(string line)
		{
			if(line=="") return "EXTENSION";	//default
			extension = line;
			return "";
		}
	//DATA
		string testbench::Data_Tag(string line)
		{
			if(line=="") return "TAG";	//default
			dataTag = line;
			return "";
		}
		string testbench::Data_Filename(string line)
		{
			if(line=="") return "FILENAME";	//default
			dataFilename = line;
			return "";
		}
	//CONDITIONS
		string testbench::Conditions_Density(string line)
		{
			if(line=="") return "DENSITY";	//default
			
			return "";
		}
		string testbench::Conditions_Percent(string line)
		{
			if(line=="") return "PERCENT";	//default
			
			return "";
		}
		string testbench::Conditions_Number(string line)
		{
			if(line=="") return "NUMBER";	//default
			
			return "";
		}
//----------------------------------------------------------------------------------------
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
			fn += delimiter;
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
				fn += to_string(outFileCount);
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
coordinate testbench::RandCoordFrom(coordinate center, double min, double max)	//may add simulation later
{//Weisstein, Eric W. "Sphere Point Picking." From MathWorld--A Wolfram Web Resource. http://mathworld.wolfram.com/SpherePointPicking.html
	//those ^^^ have FASTER methods, this is mine, the most intuitive... but def not the best. :(
	//XXX make faster someday
	coordinate co;
	double sum;			//for finding if coord is in bounds, and distance later
	double distance = double(rand())/RAND_MAX*(max-min) + min;	//the distance from teh center

	//the following has a PI/6 chance of picking a valid point...
	//about a 1% chance to have not gotten valid point after 6 tries... honestly undeserving of faster method.
	//generate random coord within sphere
	do{
		sum=0;
		co = (RandCoord()*2)-1;	//random coord anywhere from -1 to 1 centered at 0
		for(int a=0; a<3; a++)
			sum += co[a]*co[a];	//^2
	}
	while(sum>1 || sum==0);	//while out of sphere, or without directionality

	co *= (distance/sqrt(sum));	//use vector and distance to plot a point
	co /= sim.lattice;				//fit to lattice
	co += center;				//move around center
	co.Mod();					//wrap over edges of sim...
	return co;
}
double testbench::RealRadius(coordinate center)
{
	atom_cls* close = sim.Closest(center);
	if(close)
		return sim.ModDistance(close->co, center);
	cerr << "There were no atoms to return the distance to! (testbench::RealRadius)" << endl;
	return 0;
}

//---------------------------------------------------------------------------------------------
//constructor
testbench::testbench(void) : DELIMITERS("\t ")
{
	//used to count functions
	unsigned int i = 0;
	unsigned int n[MAX_SECTIONS] = {0};
	//initialize function pointers and counts
	section[i] = "INPUT";
		setting[i][n[i]] = &testbench::Input_Filename;		n[i]++;
		i++;
	section[i] = "SCALING";
		setting[i][n[i]] = &testbench::Scaling_Scale;		n[i]++;
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
	section[i] = "DATA";
		setting[i][n[i]] = &testbench::Data_Tag;			n[i]++;
		setting[i][n[i]] = &testbench::Data_Filename;		n[i]++;
		i++;
	section[i] = "CONDITIONS";
		setting[i][n[i]] = &testbench::Conditions_Density;	n[i]++;
		setting[i][n[i]] = &testbench::Conditions_Percent;	n[i]++;
		setting[i][n[i]] = &testbench::Conditions_Number;	n[i]++;
		i++;
	//initialize the numbers of functions
	sectionNum = i;
	for(unsigned int a=0; a<i; a++)
		settingNum[a] = n[a];
}
//-------------------------------------------------------------------------------------------
int testbench::Read(string inputName)
{
	ofstream copy;	//copy of input file
	ifstream input;	//infile
	string line;	//line of file
	string tag;		//tag to compare
	stringstream ss;
	int split;		//index of split in text
	unsigned int sectionIndex = MAX_SECTIONS;	//index of section
	unsigned int settingIndex = MAX_SETTINGS;	//index of setting
	const char comment = '@';

	input.open(inputName.c_str());
	if(input.fail())
	{
		cerr << "Failed to open: \"" << inputName << "\"" << endl;
		return -1;
	}
	//read file line by line
	while(getline(input,line,'\n'))
	{
		line = line.substr(0,line.find(comment));	//cut off any trailing comments
		if(Trim(line).empty())	//empty	(check after trailing comments trimmed)
			continue;
		if(line[0]==comment)	//comment
			continue;
		if(DELIMITERS.find(line[0]) == string::npos)	//if heading text, set section (no whitespace in front)
		{
			tag = Uppercase(Trim(line));	//cut off trailing whitespace and make uppercase
			for(sectionIndex=0; sectionIndex < sectionNum; ++sectionIndex)	//find tag
				if(tag == section[sectionIndex])
					break;
			if(sectionIndex >= sectionNum)				//if not a section
				cerr << "\"" << tag  << "\"" << " Not recognized as a section" << endl;	//careful! there is no line-feed on the end of the lines!
		}
		else if(sectionIndex < sectionNum)	//line[0]==whitespace && the section is recognized
		{
			line = Trim(line);						//remove tab and any leading/trailing whitespace
			split = line.find_first_of(DELIMITERS);	//find split
			if(split == (int)string::npos)			//if no settings, abort
			{
				cerr << "No settings associated with \"" << line << "\"\n";
				continue;
			}
			tag = line.substr(0,line.find('\t'));	//retreive tag
			tag = Uppercase(Trim(tag));				//trim any whitespace from tag and make uppercase
			line = line.substr(tag.length());		//cut off tag
			line = Trim(line);						//remove any whitespace
			if(line[0]==comment)	//comment
				continue;
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
	// //output data
	// input.open(inputName.c_str());
	// copy.open( (path+CreateFilename()+"-settings.tsv").c_str() );
	// while(getline(input,line))
	// 	copy << line << '\n';
	// copy.close();
	// input.close();
	//make scale models
	//make 


	return 0;
}
int testbench::Test(void)
{
	//variables
	unsigned int f,s,p,a;		//indexing
	unsigned int removed;		//# atoms removed by pore
	unsigned int lastRem;		//last # atoms removed
	unsigned int intScale[3];	//scale as an int
	int passNum;		//passivation number
	unsigned int distNum;		//takes care of cuting proper number of pores

	if(dataTag!="")			//if outputing to a datafile
	{
		sim << inputFilename[0];		//read in file
		if(DataHeader())
		{
			cerr << "Error writing to the datafile: \"" << dataFilename << "\"" << endl;
			return 0;
		}
	}
	if(scaleNum == 0)	//if no scales were set, default to identity (already filled);
		scaleNum++;
	//routine
	for(f=0; f<inputFileNum; f++)//for each inputfile
		for(s=0; s<scaleNum; s++)//for each scale
		{
			lastRem = 0;				//initialize # removed
			for(double r=poreRadMin; r<=poreRadMax; r+=poreRadStep)	//for each pore size
			{
				sim << inputFilename[f];	//read in file
				sim.Standardize();			//standardize input
				sim.Scale(fileScale[f]);	//file-specific scale
				//Scale
				for(a=0; a<3; ++a)
				{
					intScale[a] = (unsigned int)scale[s][a];
					if(intScale[a]<1)
						break;
				}
				if(a<3)	sim.Scale(scale[s]);//down
				else sim.Scale(intScale);	//up
				sim.Associate();			//create bonds
				removed = 0;				//reset count
				
				if(DistF)	//if initialized
				{
					distNum = (this->*DistF)(r);		//distribute pores to min number
					cout << distNum << "pores added" << endl;
					if(distNum == (unsigned)-1) cerr << "Error distributing pores!\n";
					else
						for(p=0; p<distNum; p++)
						{
							passNum = sim.Extant(passivation);
							removed += sim.PassivatedPore(r, &poreDistCoord[p], passivation);
							passNum = sim.Extant(passivation) - passNum;
							cout << "delta#H: " << passNum << endl;
							cout << "R:" << r <<" RR:" << RealRadius(poreDistCoord[p]) << endl;
						}
				}
				
				for(p=0; p<poreNum; p++)	//for each pore
				{
					passNum = sim.Extant(passivation);
					removed += sim.PassivatedPore(r, &poreCoord[p], passivation);	//XXX may have probs with multiple pores if any overlap!
					passNum = sim.Extant(passivation) - passNum;
					cout << "delta#H: " << passNum << endl;
					cout << "R:" << r <<" RR:" << RealRadius(poreCoord[p]) << endl;
				}
				
				if(removed > lastRem)		//ifunique: output file
				{
					cout << "Removed " << removed << " atoms making the pores" << endl;
					outFilename = CreateFilename();
					sim >>(path+outFilename+extension);
					outFileCount++;
					lastRem = removed;				//remember how many were removed
					DataLine(f,s,r);		//writes data to line in file
				}
			}
			if(poreRadMax == -1)	//no pores initialized, output as normal
			{
				sim << inputFilename[f];		//read in file
				sim.Standardize();			//standardize input
				sim.Scale(fileScale[f]);	//file-specific scale
				//Scale
				for(a=0; a<3; ++a)
				{
					intScale[a] = (unsigned int)scale[s][a];
					if(intScale[a]<1)
						break;
				}
				if(a<3)	sim.Scale(scale[s]);//down
				else sim.Scale(intScale);	//up
				//sim.Associate();			//create bonds
				outFilename = CreateFilename();
				sim >> (path+outFilename+extension);//output
				outFileCount++;				//count output
				DataLine(f,s,-1);			//write line to datafile
			}
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
int testbench::DataHeader(void)
{
	ofstream data;
	data.open(dataFilename.c_str(),ofstream::app);
	if(data.fail())
	{
		cerr << "Failed to open datafile for writing." << endl;
		return 1;
	}
	for(unsigned int a=0; a<dataTag.length(); a++)
	{
		if(a>0) data << '\t';
		switch(dataTag[a])
		{
			case 'O':
				data << "OutFilename";
				break;
			case 'P':
				data << "Path";
				break;
			case 'E':
				data << "Extension";
				break;
			case '%':
			{
				unsigned int split = dataTag.find('%',a+1);
				if(split == string::npos)	//if no terminal %
				{
					cerr << "Unable to find terminal \'%\', continuing as if nothing had happened... (testbench::Dataline)" << endl;
					break;
				}
				string sym = dataTag.substr(a+1,split-a-1);	//construct the symbol
				a = split; 									//move the iterator along
				data << '%' << sym;								//write to line
				break;
			}
			case '#':
			{
				unsigned int split = dataTag.find('#',a+1);
				if(split == string::npos)	//if no terminal %
				{
					cerr << "Unable to find terminal \'#\', continuing as if nothing had happened... (testbench::Dataline)" << endl;
					break;
				}
				string sym = dataTag.substr(a+1,split-a-1);	//construct the symbol
				a = split; 									//move the iterator along
				data << '#' << sym;								//write to line
				break;
			}
			case 'D':
				data << "Density";
				break;
			case 'r':
				data << "Radius Used";
				break;
			case 'R':	//radii calculated
				for(unsigned int p=0; p<poreNum+poreDistribute; ++p)
				{
					if(p>0) data << '\t';
					data << "Radius " << p;
				}
				break;
			case 'S':
				data << "FullScale_X\tFullScale_Y\tFullScale_Z";
				break;
			case 'I':
				data << "InputFilename";
				break;
			default:
				cerr << "unrecognized tag: \'" << dataTag[a] << "\' (testbench::HeaderLine)" << endl;
		}
	}
	data << endl;
	data.close();
	return 0;
}
int testbench::DataLine(unsigned int f, unsigned int s, double r)
{
	ofstream data;
	data.open(dataFilename.c_str(),ofstream::app);
	if(data.fail())
	{
		cerr << "Failed to open datafile for writing." << endl;
		return 1;
	}
	for(unsigned int a=0; a<dataTag.length(); ++a)
	{
		if(a>0) data << '\t';
		switch(Uppercase(dataTag[a]))
		{
			case 'O':	//outfilename
				data << outFilename;
				break;
			case 'P':	//path
				data << path;
				break;
			case 'E':	//extension
				data << extension;
				break;
			case '%':	//percentage
			{
				unsigned int split = dataTag.find('%',a+1);
				if(split == string::npos)	//if no terminal %
				{
					cerr << "Unable to find terminal \'%\', continuing as if nothing had happened... (testbench::Dataline)" << endl;
					break;
				}
				string sym = dataTag.substr(a+1,split-a-1);	//construct the symbol
				a = split; 									//move the iterator along
				//search for element in sim
				unsigned int e;
				for(e=0; e<sim.elementNum; ++e)
					if(sym == sim.element[e]) break;
				if(e<sim.elementNum)	//found the element
					data << sim%e;
				else
				{
					cerr << "Found no \"" << sym << "\" in simulation, outputing 0 (testbench::DataLine)" << endl;
					data << 0;
				}
				break;
			}
			case '#':	//number
			{
				unsigned int split = dataTag.find('#',a+1);
				if(split == string::npos)	//if no terminal %
				{
					cerr << "Unable to find terminal \'#\', continuing as if nothing had happened... (testbench::Dataline)" << endl;
					break;
				}
				string sym = dataTag.substr(a+1,split-a-1);	//construct the symbol
				a = split; 									//move the iterator along
				//search for element in sim
				unsigned int e;
				for(e=0; e<sim.elementNum; ++e)
					if(sym == sim.element[e]) break;
				if(e<sim.elementNum)	//found the element
					data << sim.Extant(e);
				else
				{
					cerr << "Found no \"" << sym << "\" in simulation, outputing 0 (testbench::DataLine)" << endl;
					data << 0;
				}
				break;
			}
			case 'D':	//density
				data << sim.Density();
				break;
			case 'r':	//radius used
				data << r;
				break;
			case 'R':	//radii calculated
			{
				unsigned int p;
				for(p=0; p<poreNum; ++p)
				{
					if(p>0) data << '\t';
					data << RealRadius(poreCoord[p]);
				}
				for(p=0; p<poreDistribute; ++p)
				{
					if(poreNum || p>0) data << '\t';	//if anything was written or not the first, write a tab
					data << RealRadius(poreDistCoord[p]);
				}
				break;
			}
			case 'S':	//fullscale
				for(int b=0; b<3; b++)
				{
					if(b>0) data << '\t';
					data << (fileScale[f][b]*scale[s][b]);
				}
				break;
			case 'I':	//inputfile
				data << inputFilename[f];
				break;
			default:
				cerr << "unrecognized tag: \'" << dataTag[a] << "\' (testbench::DataLine)" << endl;
		}
	}
	data << endl;
	data.close();
	return 0;
}
//----------------------------------------------------------------------------------------
void testbench::Default(void)
{
	inputFileNum = 0;	//no inputfiles... incremented as read from.
	scaleNum = 0;		//will check for # of scales at runtime, and default to 1
	poreNum = 0;		//no pores
	path = "";
	customName = "";
	convention = "";	//default output defiined after filename
	delimiter = "";		//no delimiter by default
	extension = ".vasp";
	outFileCount = 0;	//number of files outputed so far
	poreRadMin = 0;
	poreRadMax = -1;		//prevent holes from being made (less than Min)
	poreRadStep = 1;	
	dataFilename = "DATA.tsv";
	dataTag = "";		//default to no datafile
	poreDistribute = 0;	//default to no added pores to reach total
	passivation = "H";
	outFilename = "OUT";	//default outfilename
	randAttempts = 10;	//#of attempts at random matching
	DistF = 0;			//default to null poiner


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
//--------------------------DISTRIBUTIONS------------------------------------------------------------
unsigned int testbench::Poisson(double r)
{
	if(poreDistribute == 0)	
		return 0;	//not to add any pores
	//constants
	const double FILL = M_PI/6;	//% of cube filled by a sphere
	const double RADIUS = cbrt(sim.Volume()*1E24/(poreNum+poreDistribute)*3/4/M_PI*(FILL))*2*2/3;	//maximum radius of the distance between pores, accounting for emptyness to bring to average
	//const unsigned int MAX_ATTEMPTS = 10;	//max number of attempts at finding a suitible location for pore
	// int grid[MAX_PORES][MAX_PORES][MAX_PORES];		//XXX unsure how to impliment well...
	//pointers
	coordinate*	p[MAX_PORES*2];					//pointer to all pores
	coordinate* activePore[MAX_PORES] = {0};	//array of active pores
	//counters
	unsigned int activePoreNum = 0;				//counter of active pores
	unsigned int poreAdded = 0;					//counter for pores added to poreDist
	unsigned int pn = 0;						//# of total pores
	//indexing
	unsigned int i;				//index of pore
	unsigned int a;				//index of attempt
	unsigned int activeIndex;	//index of working active pore

	/*if(radius < r)
	{
		cerr<<"Minimum required radius to distribute the pores of \""<<radius<<"\" is smaller than the radius of the pores of \""<<r<<"\"\n";
		cerr<<"Re-runing with fewer pores...";
		poreDistribute--;	//XXX WILL give probs later, but perhaps better than going ahead?...
		return PoissonDistribution(r);
	}*/
	/*const double cellSize = radius/sqrt(3);
	const unsigned int cellNum = 
	int cells[]*/	//hard to do cells with strange lattice consts...
	//test
	cout << "R:" << RADIUS << endl;
	//initalize active pores
	if(poreNum == 0)	//if no pores, make one to seed
	{
		poreDistCoord[activePoreNum] = RandCoord();				//make random coordinate 
		activePore[activePoreNum] = &poreDistCoord[poreAdded];	//add to array of active pores
		p[pn] = &poreDistCoord[0];								//add to array of all pores
		activePoreNum++;		//increment # active pores
		poreAdded++;			//increment pores added
		pn++;					//increment total num of pores
	}
	else
	{
		for(i=0; i<poreNum; i++)	//set all extant pores to active
		{
			activePore[i] = &poreDistCoord[i];	//set pore to active by pointer
			p[pn] = &poreCoord[i];			//initialize pointer to all pores
			pn++;							//increment count of pores
		}
		activePoreNum = poreNum;
	}

	while(poreAdded < poreDistribute && activePoreNum>0)	//# of pores is low and more places to add, add pores
	{
		activeIndex = rand()%activePoreNum;	//select random pore
		
		for(a = 0; a<randAttempts; a++)	//try set number of times to find fit
		{
			poreDistCoord[poreAdded] = RandCoordFrom(*activePore[activeIndex], RADIUS, RADIUS*2);	//make random coordinate
			for(i=0; i<pn; i++)	//check distance to all pores
				if(sim.ModDistance(*p[i], poreDistCoord[poreAdded]) < RADIUS)	//if didnt pass, break
					break;
			if(i==pn)	//if passed all tests, stop searching
				break;
		}
		if(a>=randAttempts)	//unableto find any spots-> deactivate
		{
			for(i=activeIndex; i < (activePoreNum-1); i++)	//remove from active array
				activePore[i] = activePore[i+1];	//move others to fill spot
			activePoreNum--;						//decrease # of pores active
		}
		else	//good coordinate
		{
			p[pn] = &poreDistCoord[poreAdded];	//add new pore to pointer array
			activePore[activePoreNum] = p[pn];		//add as active pore
			pn++;			//increment total pore number
			activePoreNum++;	//increment # of active pores
			poreAdded++;	//increment # of pores added
		}
	}
	return poreAdded;
}
unsigned int testbench::RandomNoOverlap(double r)
{
	if(poreDistribute == 0)	
		return 0;	//not to add any pores
	unsigned int a,n,p, attempt;	//indexing and counting
	coordinate *allPore[MAX_PORES*2];

	//initialize pore pointer array
	for(p=0; p<poreNum; ++p)
		allPore[p] = &poreCoord[p];
	//make new pores
	n=0;
	for(a=0; a<poreDistribute; ++a)
	{
		attempt = 0;
		do{
			poreDistCoord[n] = RandCoord();
			++attempt;
			for(p=0; p<poreNum+n; ++p)
				if(sim.ModDistance(*allPore[p], poreDistCoord[n]) < r*2)
					break;
		}
		while( p<poreNum+n && attempt<randAttempts);
		if(p==n)	//if not too close to other pores, add as a pore
		{
			allPore[poreNum+n] = &poreDistCoord[n];
			++n;
		}
	}
	return n;	//return how many pores were established
}