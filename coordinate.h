#ifndef COORDINATE_H
#define COORDINATE_H

#include <cmath>	//for sqrt and pow
struct coordinate
{
	double ord[3];
	/*double x=0;
	double y=0;
	double z=0;*/
};
double Distance(coordinate a, coordinate b)
{
	double sum = 0;
	for(int i=0; i<3; i++)
		sum += pow(a.ord[i]-b.ord[i], 2);
	return sqrt(sum);
}
#endif