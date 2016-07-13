#include "K.h"

unsigned int K::ElementIndex(std::string e)
{
	unsigned int a;
	for(a=0; a<NUM_ELEMENTS; ++a)
		if(Uppercase(e) == Uppercase(SYM[a])) break;
	if(a < NUM_ELEMENTS) return a;//EXISTS
	return (unsigned int)-1;	//DNE
}