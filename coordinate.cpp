#include "coordinate.h"	//for coords
double Distance(coordinate a, coordinate b)
{
	double sum = 0;
	for(int i=0; i<3; i++)
		sum += pow(a.ord[i]-b.ord[i], 2);
	return sqrt(sum);
}