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
			if(line==COMMENT) return "IN_FILE";	//default
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
			/*if(Extension(filename) == "tsv")	//read recursively from this file.
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
			}*/
			/*else	//assume vasp format
			{*/
				inputFilename[inputFileNum] =  filename;
				inputFileNum++;
			// }
			return "";
		}
	//BONDING
		string testbench::Bonding_Tolerance(string line)
		{
			if(line==COMMENT) return "TOLERANCE";	//default
			stringstream ss;
			ss << line;
			// ss >> K::BOND_TOLERANCE;
			return "";
		}
		string testbench::Bonding_Lengths(string line)
		{
			if(line==COMMENT) return "LENGTHS";	//default
			
			return "";
		}
	//PORE
		string testbench::Pore_Center(string line)
		{
			if(line==COMMENT) return "CENTER";	//default
			stringstream ss;			//for conversion between strings and numbers

			ss << line;
			for(int a=0; a<3; a++)
			{
				ss >> poreCoord[poreNum].ord[a];	//XXX will have to change when dynamic
			}
			poreNum++;	//increment # of pores
			
			return "";
		}
		string testbench::Pore_Random(string line)
		{
			if(line==COMMENT) return "RANDOM";	//default
			stringstream ss;			//for conversion between strings and numbers
			unsigned int randCoordNum;	//number of random coords to generate
			
			if(!line.empty())	//if tab found
			{
				ss << line;
				ss >> randCoordNum;	//read as number of coords
			}
			else	//default
				randCoordNum = 1;
			for(unsigned int a=0; a<randCoordNum; ++a, ++poreNum)
				poreCoord[poreNum] = RandCoord();
			
			return "";
		}
		string testbench::Pore_Distribute(string line)
		{
			if(line==COMMENT) return "DISTRIBUTE";	//default
			stringstream ss;			//for conversion between strings and numbers

			
			//get number of pores desired
			if(!line.empty())	//if number
			{
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
			
			return "";
		}
		string testbench::Pore_RandomNoOverlap(string line)
		{
			if(line==COMMENT) return "RANDOM_NO_OVERLAP";	//default
			stringstream ss;			//for conversion between strings and numbers
			
			//get number of pores desired
			if(line.empty()) //0 args 
			{
				cerr << "No number of pores selected to distribute, defaults to 1 (testbench::PoreCoordinate)" << endl;
				poreDistribute = 1;
				
			}
			else //1-2 args <number> [<min_distance>]
			{
				ss << line;
				ss >> poreDistribute;	//read as number of pores to add
				if(poreDistribute > MAX_PORES)
				{
					cerr << "\"" << poreDistribute << "\" pores is above the max of: " << MAX_PORES << " reverting to that max. (testbench::PoreCoordinate)" << endl;
					poreDistribute = MAX_PORES;
				}
				if(line.find_first_of(DELIMITERS) != string::npos)	//2 args
					ss >> poreDistance;
			}
			DistF = &testbench::RandomNoOverlap;
			
			return "";
		}
		string testbench::Pore_Radius(string line)
		{
			if(line==COMMENT) return "RADIUS";	//default
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
			if(line==COMMENT) return "PASSIVATION";	//default
			if(Uppercase(line)=="NONE"|| line.empty()) passivation = "";	//either "none" or "" (case insensitive will set no passivation)
			else passivation = line;
			return "";
		}
	//OUTPUT
		string testbench::Output_Path(string line)
		{
			if(line==COMMENT) return "OUT_PATH";	//default
			path = line;
			return "";
		}
		string testbench::Output_Filename(string line)
		{
			if(line==COMMENT) return "OUT_FILE";	//default
			customName = line;	//strait up copy it over.
			return "";
		}
		string testbench::Output_Convention(string line)
		{
			if(line==COMMENT) return "OUT_CONVENTION";	//default
			convention = line;	//strait up copy it over.
			return "";
		}
		string testbench::Output_Delimiter(string line)
		{
			if(line==COMMENT) return "OUT_DELIMITER";	//default
			
			delimiter = Uppercase(line);
			if(delimiter == "TAB") delimiter = '\t';
			else if(delimiter == "SPACE") delimiter = ' ';
			else delimiter = line;
			
			return "";
		}
		string testbench::Output_Extension(string line)
		{
			if(line==COMMENT) return "OUT_EXTENSION";	//default
			extension = line;
			return "";
		}
	//DATA
		string testbench::Data_Tag(string line)
		{
			if(line==COMMENT) return "DATA_TAG";	//default
			dataTag = line;
			return "";
		}
		string testbench::Data_Filename(string line)
		{
			if(line==COMMENT) return "DATA_FILE";	//default
			dataFilename = line;
			return "";
		}
	//SETTING
		string testbench::Setting_Tag(string line)
		{
			if(line==COMMENT) return "SETTING_TAG";	//default
			settingTag = line;
			return "";
		}
		string testbench::Setting_Filename(string line)
		{
			if(line==COMMENT) return "SETTING_FILE";	//default
			settingFilename = line;
			return "";
		}
		string testbench::Setting_Path(string line)
		{
			if(line==COMMENT) return "SETTING_PATH";	//default
			settingPath = line;
			return "";
		}
	//CONDITIONS
		string testbench::Conditions_Density(string line)
		{
			if(line==COMMENT) return "DENSITY";	//default
			
			return "";
		}
		string testbench::Conditions_Percent(string line)
		{
			if(line==COMMENT) return "PERCENT";	//default
			
			return "";
		}
		string testbench::Conditions_Number(string line)
		{
			if(line==COMMENT) return "NUMBER";	//default
			
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
testbench::testbench(void) : DELIMITERS("\t "), COMMENT("@")
{
	//initialize function pointers and count
	setting[settingNum++] = &testbench::Input_Filename;
	setting[settingNum++] = &testbench::Bonding_Tolerance;
	setting[settingNum++] = &testbench::Bonding_Lengths;
	setting[settingNum++] = &testbench::Pore_Center;
	setting[settingNum++] = &testbench::Pore_Random;
	setting[settingNum++] = &testbench::Pore_Distribute;
	setting[settingNum++] = &testbench::Pore_RandomNoOverlap;
	setting[settingNum++] = &testbench::Pore_Radius;
	setting[settingNum++] = &testbench::Pore_Passivation;
	setting[settingNum++] = &testbench::Output_Path;
	setting[settingNum++] = &testbench::Output_Filename;
	setting[settingNum++] = &testbench::Output_Convention;
	setting[settingNum++] = &testbench::Output_Delimiter;
	setting[settingNum++] = &testbench::Output_Extension;
	setting[settingNum++] = &testbench::Data_Tag;
	setting[settingNum++] = &testbench::Data_Filename;
	setting[settingNum++] = &testbench::Setting_Tag;
	setting[settingNum++] = &testbench::Setting_Filename;
	setting[settingNum++] = &testbench::Setting_Path;
	setting[settingNum++] = &testbench::Conditions_Density;
	setting[settingNum++] = &testbench::Conditions_Percent;
	setting[settingNum++] = &testbench::Conditions_Number;
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
	unsigned int settingIndex = MAX_SETTINGS;	//index of setting

	input.open(inputName.c_str());
	if(input.fail())
	{
		cerr << "Failed to open: \"" << inputName << "\"" << endl;
		return -1;
	}
	//read file line by line
	while(getline(input,line,'\n'))
	{
		line = line.substr(0,line.find(COMMENT));	//cut off any comments
		line = Trim(line);							//trim line
		if(line.empty()) continue;					//if nothing to do

		split = line.find_first_of(DELIMITERS);		//find where the tag ends
		tag = Uppercase(line.substr(0,split));		//get the section tag
		
		for(settingIndex = 0; settingIndex < settingNum; ++settingIndex)	//find tag
		{
			if(tag == (this->*setting[settingIndex])(COMMENT))	//search for function matches //XXX ugh, default values for function pointers are a pain...
			{
				if(split == (int)string::npos) line = "";		//no text
				else line = Trim(line.substr(split));		//cut off tag
				(this->*setting[settingIndex])(line);	//pass line over to be used as a setting
				break;									//discontinue search
			}
		}
		if(settingIndex >= settingNum)	//unrecognized
		{
			cerr << "\"" << tag << "\"" << " Not recognized as a setting" << endl;
			continue;
		}				
	}
	if(!input.eof())
	{
		cerr << "there was trouble reading the entirity of: \"" << inputName << "\"\n";
		return -1;
	}
	input.close();
	return 0;
}
int testbench::Test(void)
{
	//variables
	unsigned int f,p,a;		//indexing
	unsigned int removed;		//# atoms removed by pore
	unsigned int lastRem;		//last # atoms removed
	unsigned int intScale[3];	//scale as an int
	int passNum;		//passivation number
	unsigned int distNum=0;		//takes care of cuting proper number of pores
	bool first;					//always output the first model

	if(DataHeader())			//if fail.
	{
		cerr << "Error writing to the datafile: \"" << dataFilename << "\"" << endl;
		return 0;
	}
	//routine

	for(f=0; f<inputFileNum; f++)//for each inputfile
	{
		// if(!poreTag.empty()) 
		// 	if(PoreHeader())			//if fail.
		// 	{
		// 		cerr << "Error writing to the porefile: \"" << poreFilename << to_string(outFileCount) "\"" << endl;
		// 		return 0;
		// 	} 
		if(poreRadMax == -1) //no pores
		{
			sim << inputFilename[f];	//read in file
			sim.Standardize();			//standardize input
			//Scale
				for(a=0; a<3; ++a)
				{
					intScale[a] = (unsigned int)fileScale[f][a];
					if(intScale[a]<1)
						break;
				}
				if(a<3)	sim.Scale(fileScale[f]);//down
				else sim.Scale(intScale);	//up
			sim >> (path+CreateFilename()+extension);
			DataLine(f,-1);
			++outFileCount;
		}
		lastRem = 0;	//initialize # removed
		first =1;		//this will be first
		for(double r=poreRadMin; r<=poreRadMax; r+=poreRadStep)	//for each pore size
		{
			if(poreNum+poreDistribute == 0)	//if no pores, but radius has ben selelected
				++poreNum;
			sim << inputFilename[f];	//read in file
			sim.Standardize();			//standardize input
			//Scale
				for(a=0; a<3; ++a)
				{
					intScale[a] = (unsigned int)fileScale[f][a];
					if(intScale[a]<1)
						break;
				}
				if(a<3)	sim.Scale(fileScale[f]);//down
				else sim.Scale(intScale);	//up
			sim.Associate();			//create bonds
			removed = 0;				//reset count

			for(p=0; p<poreNum; p++)	//for each pore
			{
				passNum = sim.Extant(passivation);
				removed += sim.PassivatedPore(r, &poreCoord[p], passivation);	//XXX may have probs with multiple pores if any overlap!
				passNum = sim.Extant(passivation) - passNum;
				cout << "delta#H: " << passNum << endl;
				cout << "R:" << r <<" RR:" << RealRadius(poreCoord[p]) << endl;
				// if(!poreTag.empty()) PoreLine(poreCoord[p],r);
			}
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
						// if(!poreTag.empty()) PoreLine(poreDistCoord[p]);
					}
			}
			
			if(removed > lastRem || first)		//ifunique or first: output file
			{
				cout << "Removed " << removed << " atoms making the pores" << endl;
				outFilename = CreateFilename();
				sim >>(path+outFilename+extension);
				DataLine(f,r);		//writes data to line in file
				Setting(distNum,r);	//writes setttings to file-specific setting file
				outFileCount++;
				lastRem = removed;				//remember how many were removed
				first=0;
			}
		}
	}
	sim.ClearData();
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
	if(dataTag.empty()) return 0;	//nothing to write
	ofstream data;
	data.open(dataFilename.c_str(),ofstream::app);
	if(data.fail())
	{
		cerr << "Failed to open datafile for writing." << endl;
		return 1;
	}

	//set precision
	data << fixed;
	data << setprecision(K::PRECISION);

	for(unsigned int a=0; a<dataTag.length(); a++)
	{
		if(a>0) data << '\t';
		switch(dataTag[a])
		{
			case 'C':
			case 'c':
				for(unsigned int p=0; p<poreNum+poreDistribute; ++p)
				{
					if(p>0) data << '\t';
					data << "Coordinate " << p;
				}
				break;
			case 'O':
				data << "Out_File";
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
				data << "Scale_X\tScale_Y\tScale_Z";
				break;
			case 'I':
				data << "In_File";
				break;
			default:
				cerr << "unrecognized tag: \'" << dataTag[a] << "\' (testbench::HeaderLine)" << endl;
		}
	}
	data << endl;
	data.close();
	return 0;
}
int testbench::DataLine(unsigned int f, double r)
{
	if(dataTag.empty()) return 0;	//nothing to write
	ofstream data;
	data.open(dataFilename.c_str(),ofstream::app);
	if(data.fail())
	{
		cerr << "Failed to open datafile for writing." << endl;
		return 1;
	}

	//set precision
	data << fixed;
	data << setprecision(K::PRECISION);
	
	for(unsigned int a=0; a<dataTag.length(); ++a)
	{
		if(a>0) data << '\t';
		switch(dataTag[a])
		{
			case 'C':
			case 'c':	//center of pore
			{
				unsigned int p;
				unsigned int c;
				for(p=0; p<poreNum; ++p)
				{
					if(p>0) data << '\t';
					for(c=0; c<3; ++c)	//write coordinates
					{
						if(c>0) data << ',';
						data << poreCoord[p][c];
					}
				}
				for(p=0; p<poreDistribute; ++p)
				{
					if(poreNum || p>0) data << '\t';	//if anything was written or not the first, write a tab
					for(c=0; c<3; ++c)	//write coordinates
					{
						if(c>0) data << ',';
						data << poreDistCoord[p][c];
					}
				}
				break;
			}
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
					data << fileScale[f][b];
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

	poreNum = 0;		//no pores
	path = "";
	customName = "";
	convention = "N";	//default output definned after filename
	delimiter = "";		//no delimiter by default
	extension = ".vasp";
	outFileCount = 0;	//number of files outputed so far
	poreRadMin = 0;
	poreRadMax = -1;		//prevent holes from being made (less than Min)
	poreRadStep = 1;	
	dataFilename = "DATA.tsv";
	settingFilename = "SETTING.tsv";
	dataTag = "";		//default to no datafile
	settingTag = "";	//default to no settingfile
	poreDistribute = 0;	//default to no added pores to reach total
	passivation = "H";
	outFilename = "OUT";	//default outfilename
	randAttempts = 10;	//#of attempts at random matching
	DistF = 0;			//default to null poiner
	settingPath = "";	//no path
	poreDistance = 0;	//no modification on minimum distance if none selected


	unsigned int a;	//indexing

	for(a=0; a<MAX_FILES; a++)
		for(int b=0; b<3; b++)
			fileScale[a][b] = 1;
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
			for(p=0; p<(poreNum+n); ++p)
				if(sim.ModDistance(*allPore[p], poreDistCoord[n]) < r*2 + poreDistance)	//too close
					break;
		}
		while( p<poreNum+n && attempt<randAttempts);
		if(p==poreNum+n)	//if not too close to other pores, add as a pore
		{
			allPore[poreNum+n] = &poreDistCoord[n];
			++n;
		}
	}
	return n;	//return how many pores were established
}
//porefile
int testbench::SettingHeader(string filename)
{
	ofstream setting;	//file to write to
	const string DIM = "XYZ";

	setting.open( filename.c_str(), ofstream::app);
	if(setting.fail())
	{
		cerr << "Failed to open settingfile for writing. (testbench::settingHeader)" << endl;
		return 1;
	}

	//set precision
	setting << fixed;
	setting << setprecision(K::PRECISION);

	for(unsigned int a=0; a<settingTag.length(); a++)
	{
		if(a>0) setting << '\t';
		switch(settingTag[a])
		{
			case 'C':
			case 'c':
				for(unsigned int c=0; c<3; ++c)
				{
					if(c>0) setting << '\t';
					setting << "Center_" << DIM[c];
				}
				break;
			// case 'r':	//radii used for each //XXX when the radii are unique!
			case 'R':	//radii calculated
				setting << "Radius ";
				break;
			case 'P':	//passivation of pore
				setting << "Passivation #";
				break;
			default:
				cerr << "unrecognized tag: \'" << settingTag[a] << "\' (testbench::settingHeader)" << endl;
		}
	}
	setting << endl;
	setting.close();
	return 0;
}
int testbench::SettingLine(string filename, coordinate center, double r)
{
	ofstream setting;

	setting.open( filename.c_str(), ofstream::app);
	if(setting.fail())
	{
		cerr << "Failed to open settingfile for writing. (testbench::settingLine)" << endl;
		return 1;
	}

	//set precision
	setting << fixed;
	setting << setprecision(K::PRECISION);

	for(unsigned int a=0; a<settingTag.length(); ++a)
	{
		if(a>0) setting << '\t';
		switch(settingTag[a])
		{
			case 'C':
			case 'c':	//center of pore
				for(unsigned int c=0; c<3; ++c)	//write coordinates
				{
					if(c>0) setting << '\t';
					setting << center[c];
				}
				break;
			case 'r':	//radius used
				setting << r;
				break;
			case 'R':	//radii calculated
				setting << RealRadius(center);
				break;
			default:
				cerr << "unrecognized tag: \'" << settingTag[a] << "\' (testbench::settingLine)" << endl;
		}
	}
	setting << endl;
	setting.close();
	return 0;
}
int testbench::Setting(unsigned int distNum, double r)	//XXX put all singl settings in datafile!
{
	if(settingTag.empty()) return 0;	//if nothing to write
	unsigned int p;		//indexing
	string filename = settingPath + CreateFilename() + settingFilename;	//create setting filename

	SettingHeader(filename);
	for(p=0; p<poreNum; ++p)
		SettingLine(filename, poreCoord[p],r);
	if(distNum != (unsigned)(-1))
		for(p=0; p<distNum; ++p)
		{
			SettingLine(filename, poreDistCoord[p],r);
		}
	return 0;
}