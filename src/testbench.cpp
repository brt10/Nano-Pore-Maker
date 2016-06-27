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
			unsigned int split = line.find('\t');
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
		string testbench::Pore_Coordinate(string line)
		{
			if(line=="") return "COORDINATE";	//default
			stringstream ss;			//for conversion between strings and numbers
			ifstream coordFile;			//ifile obj for reading recursively
			string coordLine;			//line in coordfile
			unsigned int randCoordNum;	//number of random coords to generate
			unsigned int mark;					//marks place in tag
			const string randomTag = "RANDOM";
			const string distributeTag = "DISTRIBUTE";

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
			else if(Uppercase(line.substr(0,randomTag.length())) == randomTag)	//if random coord
			{
				mark = line.find('\t');
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
			else if(Uppercase(line.substr(0,distributeTag.length())) == distributeTag)	//if evenly distributed:
			{
				//get number of pores desired
				mark = line.find('\t');
				if(mark != unsigned(string::npos))	//if tab found
				{
					line = Trim(line.substr(mark));	//cut off tag and trim
					ss << line;
					ss >> poreDistribute;	//read as number of pores to add
				}
				else	//default
				{
					cerr << "No number of pores selected to distribute, defaults to 0\n";
					poreDistribute = 0;
				}
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
			center = line;
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
	unsigned int sectionIndex = MAX_SECTIONS;	//index of section
	unsigned int settingIndex = MAX_SETTINGS;	//index of setting
	const char comment = '#';

	input.open(inputName.c_str());
	if(input.fail())
	{
		cerr << "Failed to open: \"" << inputName << "\"" << endl;
		return -1;
	}
	//read file line by line
	while(getline(input,line))
	{
		if(line[0]==comment)	//comment
			continue;
		line = line.substr(0,line.find(comment));	//cut off any trailing comments

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
	unsigned int f,s,p;		//indexing
	unsigned int removed;	//# atoms removed by pore
	unsigned int lastRem;	//last # atoms removed
	int poisson;			//takes care of cuting proper number of pores

	if(dataTag!="")			//if outputing to a datafile
	{
		sim << inputFilename[0];		//read in file
		if(DataHeader())
		{
			cerr << "Error writing to the datafile: \"" << dataFilename << "\"" << endl;
			return 0;
		}
	}
	//routine
	for(f=0; f<inputFileNum; f++)//for each inputfile
		for(s=0; s<scaleNum; s++)//for each scale
		{
			lastRem = 0;				//initialize # removed
			for(double r=poreRadMin; r<=poreRadMax; r+=poreRadStep)	//for each pore size
			{
				sim << inputFilename[f];		//read in file
				sim.Standardize();			//standardize input
				sim.Scale(fileScale[f]);	//file-specific scale
				sim.Scale(scale[s]);		//scale
				sim.Associate();			//create bonds
				removed = 0;				//reset count
				
				poisson = PoissonDistribution(r);		//distribute pores to min number
				cout << poisson << "pores added" << endl;
				if(poisson == -1) cerr << "Error distributing pores!\n";
				else
					for(p=0; p<(unsigned)poisson; p++)
						removed += sim.PassivatedHole(r, &poreDistCoord[p]);

				for(p=0; p<poreNum; p++)	//for each pore
					removed += sim.PassivatedHole(r, &poreCoord[p]);	//XXX may have probs with multiple pores if any overlap!

				
				if(removed > lastRem)		//ifunique: output file
				{
					cout << removed << endl;
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
				sim.Scale(scale[s]);		//scale
				sim.Associate();			//create bonds
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
		switch(Uppercase(dataTag[a]))
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
				data << '%';	//in case no elements are initialized... XXX ugly bug. :(
				for(unsigned int e=0; e<sim.elementNum; e++)
				{
					if(e>0) data << "\t%";
					data << sim.element[e];
				}
				break;
			case 'N':
				data << '#';	//XXX fixes ugly bug
				for(unsigned int e=0; e<sim.elementNum; e++)
				{
					if(e>0) data << "\t#";
					data << sim.element[e];
				}
				break;
			case 'D':
				data << "Density";
				break;
			case 'R':
				data << "Radius";
				break;
			case 'S':
				data << "FullScale_X\tFullScale_Y\tFullScale_Z";
				break;
			case 'I':
				data << "InputFilename";
				break;
			default:
				cerr << "unrecognized tag: \'" << dataTag[a] << '\'' << endl;
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
	for(unsigned int a=0; a<dataTag.length(); a++)
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
			case '%':	//all percentages
				for(unsigned int e=0; e<sim.elementNum; e++)
					data << sim%e << '\t';
				break;
			case 'N':	//all numbers
				for(unsigned int e=0; e<sim.elementNum; e++)
					data << sim.Extant(e) << '\t';
				break;
			case 'D':	//density
				data << sim.Density();
				break;
			case 'R':	//radius
				data << r;
				break;
			case 'S':	//fullscale
				for(int b=0; b<3; b++)
					data << (fileScale[f][b]*scale[s][b]) << '\t';
				break;
			case 'I':	//inputfile
				data << inputFilename[f];
				break;
			default:
				cerr << "unrecognized tag: \'" << dataTag[a] << '\'' << endl;
		}
	}
	data << endl;
	data.close();
	return 0;
}
//----------------------------------------------------------------------------------------
void testbench::Default(void)
{
	inputFileNum = 0;//no inputfiles... incremented as read from.
	scaleNum = 1;
	poreNum = 0;	//no pores
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
	dataTag = "";	//default to no datafile
	poreDistribute = 0;	//default to no added pores to reach total


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
//--------------------------------------------------------------------------------------
int testbench::PoissonDistribution(double r)
{
	if(poreDistribute == 0)	
		return 0;	//not to add any pores
	//constants
	const double FILL = M_PI/6;	//% of cube filled by a sphere
	const double RADIUS = cbrt(sim.Volume()*1E24/(poreNum+poreDistribute)*3/4/M_PI*(FILL))*2;	//maximum radius of the distance between pores, accounting for emptyness to bring to average
	const unsigned int MAX_ATTEMPTS = 10;	//max number of attempts at finding a suitible location for pore
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
		
		for(a = 0; a<MAX_ATTEMPTS; a++)	//try set number of times to find fit
		{
			poreDistCoord[poreAdded] = RandCoordFrom(*activePore[activeIndex], RADIUS, RADIUS*2);	//make random coordinate
			for(i=0; i<pn; i++)	//check distance to all pores
				if(sim.ModDistance(*p[i], poreDistCoord[poreAdded]) < RADIUS)	//if didnt pass, break
					break;
			if(i==pn)	//if passed all tests, stop searching
				break;
		}
		if(a==MAX_ATTEMPTS)	//unableto find any spots-> deactivate
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