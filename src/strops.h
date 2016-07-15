#ifndef STROPS_H
#define	STROPS_H

#include <string>		//strings

using namespace std;	//for now...makes strings easier

//chars
char Uppercase(char);
char Lowercase(char);
//strings
string Uppercase(string);
string Lowercase(string);
string Trim(string, string=" \a\b\t\n\v\f\r");
//conversions	//use string definitions! had to compile as c++ 11...
/*template<typename To, typename From> To Convert(From);
string Convert(int);
string Convert(double);
int Convert(string);
double Convert(string);

string I_Str(int);	//will be old news soon.. :)*/

#endif