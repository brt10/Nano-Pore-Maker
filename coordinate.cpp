#include "coordinate.h"	//for coords
coordinate::coordinate()
{
	for(int a=0; a<3; a++)
		ord[a] = 0;
}
coordinate::coordinate(const double k)
{
	for(int a=0; a<3; a++)
		ord[a] = k;
}
double coordinate::Distance(coordinate a, coordinate b)
{
	double sum = 0;
	for(int i=0; i<3; i++)
		sum += pow(a.ord[i]-b.ord[i], 2);
	return sqrt(sum);
}
coordinate coordinate::operator+(const coordinate& co)
{
	coordinate sum;
	for(int a=0; a<3; a++)
		sum.ord[a] = ord[a] + co.ord[a];
	return sum;
}
coordinate coordinate::operator+(const double k)
{
	coordinate sum;
	for(int a=0; a<3; a++)
		sum.ord[a] = ord[a] + k;
	return sum;
}
double coordinate::operator[](const unsigned int i)
{
	return ord[i];
}
coordinate coordinate::operator=(const coordinate& co)
{
	for(int a=0; a<3; a++)
		this->ord[a] = co.ord[a];
	return *this;
}
coordinate coordinate::operator=(const double k)
{
	for(int a=0; a<3; a++)
		this->ord[a] = k;
	return *this;
}
void coordinate::operator+=(const coordinate& co)
{
	(*this)=(*this)+co;
	return;
}
void coordinate::operator+=(const double k)
{
	(*this)=(*this)+k;
	return;
}
coordinate coordinate::operator*(const coordinate& co)
{
	coordinate product;
	for(int a=0; a<3; a++)
		product.ord[a] = ord[a] * co.ord[a];
	return product;

}
coordinate coordinate::operator*(const double k)
{
	coordinate product;
	for(int a=0; a<3; a++)
		product.ord[a] = ord[a] * k;
	return product;
}
coordinate coordinate::operator*(const double k[3])
{
	coordinate product;
	for(int a=0; a<3; a++)
		product.ord[a] = ord[a] * k[a];
	return product;
}