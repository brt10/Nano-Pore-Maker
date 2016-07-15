#include "strops.h"
//chars
char Uppercase(char c)
{
	if(c>='a' && c<='z')	//if lowercase
		return c-'a'+'A';	//make uppercase
	return c;				//else nothing.
}
char Lowercase(char c)
{
	if(c>='A' && c<='Z')	//if Uppercase
		return c-'A'+'a';	//make Lowercase
	return c;				//else nothing
}
//strings
string Uppercase(string s)
{
	unsigned int len = s.length();
	for(unsigned int a=0; a<len; ++a)
		s[a] = Uppercase(s[a]);
	return s;
}
string Lowercase(string s)
{
	unsigned int len = s.length();
	for(unsigned int a=0; a<len; ++a)
		s[a] = Lowercase(s[a]);
	return s;
}
string Trim(string line)
{
	static const char whitespace[] = {'\0', ' ', '\a', '\b', '\t', '\n', '\v', '\f', '\r'};
	static const unsigned int whiteNum = sizeof(whitespace)/sizeof(char);
	
	for(int a=0; a<2; ++a)	//check front and back
		for(unsigned int b=0; b<whiteNum; ++b)	//recurse through array of possible whitespace
		{
			if(line.empty())	//if empty, return before breaking!
				return line;
			if( line[a==0 ? 0:line.length()-1] == whitespace[b])
			{
				line.erase(a==0 ? 0:line.length()-1,1);
				line = Trim(line);
			}
		}
	return line;
}
/*//conversions	//depreciated! this already exists in the string library dummy.
string Convert(int a)
{
	if(a==0)	return "0";

	string s;	//output
	bool n=0;	//is negative
	
	if(a<0)		//if negative
	{
		n=1;
		a=-a;
	}
	while(a!=0)	//while still digits
	{
		s=char((a%10)+'0')+s;
		a/=10;
	}
	if(n) s='-'+s;	//if was negative
	return s;
}
string Convert(double a)
{
	string s = "";
	return s;
}
int Convert(string s)
{
	return stoi(s);	//well this exists...
}
double Convert(string s)
{
	return stod(s);	//well this exists...
}
string I_Str(int a)
{
	return Convert<string>(a);
}*/

