#ifndef COORDINATE_H
#define COORDINATE_H

#include <cmath>	//for sqrt and pow

class coordinate
{
	public:
		//constructor
		coordinate();
		coordinate(const double);
		// coordinate(const coordinate&);
		//variables
		double ord[3];	//XXX make private later... this involves a lot of organizing of other code...
		//functions
		double Distance(coordinate);				//distance to another coord
		void Mod(double=1);							//takes mod of coordinate XXX currently inefficient
		void Dec(void);		//returns the decimal of the number(=modulus1)
		//operators
		double& operator[](const unsigned int);		//allows one to refer to the coordinate directly. :)
		// coordinate operator=(const coordinate&);	//set equal to another coord. (able to do: coord = coordA = coordB)
		coordinate& operator=(const double);			//set all axis the same.	(able to do: coordA = coordB = 9;)
		//+
			coordinate operator+(const coordinate&);	//vector addition
			coordinate operator+(const double);			//scalar addition
			void operator+=(const coordinate&);			//...nuff said
			void operator+=(const double);
		//-
			coordinate operator-(const coordinate&);	//vector subtraction
			coordinate operator-(const double);			//scalar subtraction
			void operator-=(const coordinate&);			//...nuff said
			void operator-=(const double);
		//*
			coordinate operator*(const coordinate&);	//multiply strait across... not vector multiplication
			coordinate operator*(const double);			//scalar multiplication
			coordinate operator*(const double[3]);
			void operator*=(const coordinate&);
			void operator*=(const double);
			void operator*=(const double[3]);
		///
			coordinate operator/(const coordinate&);	//divide strait across... not vector division
			coordinate operator/(const double);			//scalar division
			coordinate operator/(const double[3]);		//scalar division for each pair
			void operator/=(const coordinate&);	//multiply strait across... not vector multiplication
			void operator/=(const double);			//scalar multiplication
			void operator/=(const double[3]);
		//compare
			bool operator<(const coordinate&);
			bool operator<(const double);
			bool operator<=(const coordinate&);
			bool operator<=(const double);
			bool operator>(const coordinate&);
			bool operator>(const double);
			bool operator>=(const coordinate&);
			bool operator>=(const double);
			bool operator==(const coordinate&);
			bool operator==(const double);
			bool operator!=(const coordinate&);
			bool operator!=(const double);
};

double Distance(coordinate, coordinate);	//the euclidean distance

#endif