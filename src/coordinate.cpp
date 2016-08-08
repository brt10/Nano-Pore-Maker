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
double coordinate::Distance(coordinate co)
{
	double sum = 0;
	for(int i=0; i<3; i++)
		sum += pow(static_cast<double>(co[i]-(*this)[i]), 2);
	return sqrt(sum);
}
double Distance(coordinate a, coordinate b)
{
	return a.Distance(b);
}
void coordinate::Mod(double k)
{
	for(int a=0; a<3; a++)	//for each axis
	{
		while(ord[a]<0)
			ord[a]+=k;
		while(ord[a]>=k)
			ord[a]-=k;
	}
	return;
}
void coordinate::Dec(void)
{
	for(int a=0; a<3; a++)
	{
		if(ord[a]<0)
			ord[a] = -ord[a];
		ord[a] -= (int)ord[a];
	}
	return;
}
double& coordinate::operator[](const unsigned int i)
{
	return ord[i%3];
}
// coordinate coordinate::operator=(const coordinate& co)
// {
// 	for(int a=0; a<3; a++)
// 		this->ord[a] = co.ord[a];
// 	return *this;
// }
coordinate& coordinate::operator=(const double k)
{
	for(int a=0; a<3; a++)
		this->ord[a] = k;
	return *this;
}
//+
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
//-
	coordinate coordinate::operator-(const coordinate& co)
	{
		coordinate diff;
		for(int a=0; a<3; a++)
			diff.ord[a] = ord[a] - co.ord[a];
		return diff;
	}
	coordinate coordinate::operator-(const double k)
	{
		coordinate diff;
		for(int a=0; a<3; a++)
			diff.ord[a] = ord[a] - k;
		return diff;
	}
	void coordinate::operator-=(const coordinate& co)
	{
		(*this)=(*this)-co;
		return;
	}
	void coordinate::operator-=(const double k)
	{
		(*this)=(*this)-k;
		return;
	}
//*
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
	void coordinate::operator*=(const coordinate& co)
	{
		(*this) = (*this) * co;
		return;
	}
	void coordinate::operator*=(const double k)
	{
		(*this) = (*this) * k;
		return;
	}
	void coordinate::operator*=(const double k[3])
	{
		(*this) = (*this) * k;
		return;
	}
///
	coordinate coordinate::operator/(const coordinate& co)
	{
		coordinate quotent;
		for(int a=0; a<3; a++)
			quotent.ord[a] = ord[a] / co.ord[a];
		return quotent;

	}
	coordinate coordinate::operator/(const double k)
	{
		coordinate quotent;
		for(int a=0; a<3; a++)
			quotent.ord[a] = ord[a] / k;
		return quotent;
	}
	coordinate coordinate::operator/(const double k[3])
	{
		coordinate quotent;
		for(int a=0; a<3; a++)
			quotent.ord[a] = ord[a] / k[a];
		return quotent;
	}
	void coordinate::operator/=(const coordinate& co)
	{
		(*this) = (*this) / co;
		return;
	}
	void coordinate::operator/=(const double k)
	{
		(*this) = (*this) / k;
		return;
	}
	void coordinate::operator/=(const double k[3])
	{
		(*this) = (*this) / k;
		return;
	}
//compare
	bool coordinate::operator<(const coordinate& co)
	{
		for(int a=0; a<3; a++)
			if(ord[a]>=co.ord[a])
				return 0;
		return 1;
	}
	bool coordinate::operator<(const double k)
	{
		for(int a=0; a<3; a++)
			if(ord[a]>=k)
				return 0;
		return 1;
	}
	bool coordinate::operator<=(const coordinate& co)
	{
		for(int a=0; a<3; a++)
			if(ord[a]>co.ord[a])
				return 0;
		return 1;
	}
	bool coordinate::operator<=(const double k)
	{
		for(int a=0; a<3; a++)
			if(ord[a]>k)
				return 0;
		return 1;
	}
	bool coordinate::operator>(const coordinate& co)
	{
		for(int a=0; a<3; a++)
			if(ord[a]<=co.ord[a])
				return 0;
		return 1;
	}
	bool coordinate::operator>(const double k)
	{
		for(int a=0; a<3; a++)
			if(ord[a]<=k)
				return 0;
		return 1;
	}
	bool coordinate::operator>=(const coordinate& co)
	{
		for(int a=0; a<3; a++)
			if(ord[a]<co.ord[a])
				return 0;
		return 1;
	}
	bool coordinate::operator>=(const double k)
	{
		for(int a=0; a<3; a++)
			if(ord[a]<k)
				return 0;
		return 1;
	}
	bool coordinate::operator==(const coordinate& co)
	{
		for(int a=0; a<3; a++)
			if(ord[a]!=co.ord[a])
				return 0;
		return 1;
	}
	bool coordinate::operator==(const double k)
	{
		for(int a=0; a<3; a++)
			if(ord[a]!=k)
				return 0;
		return 1;
	}
	bool coordinate::operator!=(const coordinate& co)
	{
		for(int a=0; a<3; a++)
			if(ord[a]==co.ord[a])
				return 0;
		return 1;
	}
	bool coordinate::operator!=(const double k)
	{
		for(int a=0; a<3; a++)
			if(ord[a]==k)
				return 0;
		return 1;
	}
