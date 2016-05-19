#ifndef COORDINATE_H
#define COORDINATE_H

#include <cmath>	//for sqrt and pow
class coordinate
{
	public:
		//constructor
		coordinate();
		coordinate(const double);
		coordinate(const coordinate&);
		//variables
		double ord[3];	//XXX make private later... this involves a lot of organizing of other code...
		//functions and operators
		double Distance(coordinate, coordinate);	//the euclidean distance
		coordinate operator+(const coordinate&);	//vector addition
		coordinate operator+(const double);			//scalar addition
		double operator[](const unsigned int);		//allows one to refer to the coordinate directly. :)
		coordinate operator=(const coordinate&);	//set equal to another coord. (able to do: coord = coordA = coordB)
		coordinate operator=(const double);			//set all axis the same.	(able to do: coordA = coordB = 9;)
		void operator+=(const coordinate&);			//...nuff said
		void operator+=(const double);
		
		coordinate operator*(const coordinate&);	//multiply strait across... not vector multiplication
		coordinate operator*(const double);			//scalar multiplication
		coordinate operator*(const double[3]);


};
#endif