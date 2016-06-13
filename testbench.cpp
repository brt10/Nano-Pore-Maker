//SETTINGS
	//INPUT
		string testbench::Input_Filename(string line)
		{
			int split = line.find('\t');
			string scaleText;
			string filename;
			stringstream ss;

			if(line=="") return "filename";	//default
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
				cerr << '\"' << filename << "\" does not exist or cannot be opened!/n";
				return "";	//XXX NEED TO ADD ERROR CATCHING!
			}
			if(Extension(filename) == "tsv")	//read recursively from this file.
			{
				ifstream header;
				string headerLine;	//a witty remix of headline
				dataFileNum = 0;	//reset... defaut = 1;
				header.open(filename);
				//allready checked if could be opened.
				while(getline(header,headerline) && dataFileNum < MAX_FILES)
					Input_Filename(headerLine);
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
			ifstream scaleFile;
			stringstream ss;
			string scaleLine;

			if(line=="") return "filename";	//default
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
			scalefile.close();
			return "";
		}
	//BONDING
		string testbench::Bonding_Tolerance(string line)
		{
			if(line=="") return "tolerance";	//default
			stringstream ss;
			ss << line;
			ss >> K::BOND_TOLERANCE;
			return "";
		}
		string testbench::Bonding_Lengths(string line)
		{
			if(line=="") return "lengths";	//default
			
			return "";
		}
	//PORE
		string testbench::Pore_Number(string line)
		{
			if(line=="") return "number";	//default
			stringstream ss;
			ss << line;
			ss >> poreNum;			
			return "";
		}
		string testbench::Pore_Centering(string line)

		{
			if(line=="") return "centering";	//default
			
			return "";
		}
		string testbench::Pore_Radius(string line)
		{
			if(line=="") return "radius";	//default
			stringstream ss;
			ss << line;
			ss >> poreRadius;
			return "";
		}
		string testbench::Pore_Iterations(string line)
		{
			if(line=="") return "interactions";	//default
			stringstream ss;
			ss << line;
			ss >> poreIterations;
			return "";
		}
		string testbench::Pore_Passivation(string line)
		{
			if(line=="") return "passivation";	//default
			
			return "";
		}
	//OUTPUT
		string testbench::Output_Path(string line)
		{
			if(line=="") return "path";	//default
			
			return "";
		}
		string testbench::Output_Convention(string line)
		{
			if(line=="") return "convention";	//default
			
			return "";
		}
		string testbench::Output_Delimeter(string line)
		{
			if(line=="") return "delimeter";	//default
			
			return "";
		}
		string testbench::Output_Extension(string line)
		{
			if(line=="") return "extension";	//default
			
			return "";
		}
//string operations
string testbench::Trim(string line)
{
	static const char whitespace[] = {'\0', ' ', '\a', '\b', '\t', '\n', '\v', '\f', '\r'};
	static const unsigned int whiteNum = sizeof(whitespace)/sizeof(char);
	
	for(int a=0; a<2; a++)
		for(int b=0; b<whiteNum; b++)
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
//file opeerations
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
	int period = filename.find('.');
	if(period == string::npos)
		return "";
	return filename.substr(period+1);
}
//constructor
testbench::testbench : MAX_SECTIONS(10), MAX_SETTINGS(10) (void)
{
	//used to count functions
	unsigned int i = 0;
	unsigned int n[MAX_SECTIONS] = {0};
	//initialize function pointers and counts
	sections[i] = "INPUT";
		setting[i][n[i]] = Input_Filename;	n[i]++;
		i++;
	sections[i] = "SCALING";
		setting[i][0] = Scaling_Filename;	n[i]++;
		i++;
	sections[i] = "BONDING";
		settings[i][0] = Bonding_Tolerance;	n[i]++;
		settings[i][1] = Bonding_Lengths;	n[i]++;
		i++;
	sections[i] = "PORE";
		settings[i][0] = Pore_Number;		n[i]++;
		settings[i][1] = Pore_Centering;	n[i]++;
		settings[i][2] = Pore_Radius;		n[i]++;
		settings[i][3] = Pore_Iterations;	n[i]++;
		settings[i][4] = Pore_Passivation;	n[i]++;
		i++;
	sections[i] = "OUTPUT";
		settings[i][0] = Output_Path;		n[i]++;
		settings[i][1] = Output_Convention;	n[i]++;
		settings[i][2] = Output_Delimeter;	n[i]++;
		settings[i][3] = Output_Extension;	n[i]++;
		i++;
	//initialize the numbers of functions
	sectionNum = i;
	for(int a=0; a<i; a++)
		settingNum = n[a];
}
int testbench::Read(string inputName)
{
	ifstream input;	//infile
	string line;	//line of file
	string tag;		//tag to compare
	stringstream ss;
	unsigned int sectionIndex;	//index of section
	unsigned int settingIndex;	//index of setting

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
			tag = Trim(line);	//cut off trailing whitespace
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
			line = line.substr(tag.length());		//cut off tag
			line = Trim(line);						//remove any whitespace

			for(settingIndex = 0; settingIndex < settingNum[sectionIndex]; settingIndex++)	//find tag
				if(tag == setting[sectionIndex][settingIndex]())	//search for function matches
				{
					setting[sectionIndex][settingIndex](line);	//pass line over to be used as a setting
					break;										//discontinue search
				}
			if(setting >= MAX_SECTIONS)	//unrecognized
			{
				cerr << "\"" << tag << "\"" << " Not recognized as a setting under " << sectionTag[section] << endl;
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

	//make scale models
	//make 


	return 0;
}
int testbench::Test(void)
{
	for(int f=0; f<dataFileNum; f++)//for each datafile
		for(int s=0; s<scaleNum; s++)//for each scale
			for(int p=0; p<poreNum; p++//for each pore
			{
				//for each iteration
				sim << dataFilename;		//read in file
				sim.Scale(fileScale[f]);	//file-specific scale
				sim.Scale(scale[s]);		//scale
				//untill new pore
					//make and compare atoms removed
				//make output name
				sim >> //output
			}
	return 0;
}
int testbench::Run(string inuputName)
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
	scaleNum = 0;	//no scales
	poreNum = 0;	//no pores
	poreRadius = 0;	//no size to pores

	for(int a=0; a<MAX_FILES; a++)
		for(int b=0; b<3; b++)
			fileScale[a][b] = 1;
	for(int a=0; a<MAX_SCALES; a++)
		for(int b=0; b<3; b++)
			scale[a][b] = 1;
	return;
}