#include "StdAfx.h"
#include "myVector.h"


myVector::myVector(void)
{
	x=0;y=0;z=0;
}

myVector::~myVector(void)
{
}

myVector::myVector(double xx, double yy, double zz)
{
	x=xx;y=yy;z=zz;
}

myVector myVector::Normalize()
{
	double dist=sqrt(x*x+y*y+z*z);
	if(dist!=0)
	{
		return myVector(x/dist,y/dist,z/dist);
	}
	else
		return myVector();
}

myVector myVector::productReal(double number)
{
	return myVector(x*number,y*number,z*number);
}

double myVector::innerProduct(myVector v2)
{
	return x*v2.x+y*v2.y+z*v2.z;
}

myVector myVector::sub(myVector v2)
{
	return myVector(x-v2.x,y-v2.y,z-v2.z);
}
myVector myVector::add(myVector v2)
{
	return myVector(x+v2.x,y+v2.y,z+v2.z);
}

double myVector::length()
{
	return sqrt(x*x+y*y+z*z);
}


double myVector::dist(myVector target)
{
	return sqrt((x-target.x)*(x-target.x)+(y-target.y)*(y-target.y)+(z-target.z)*(z-target.z));
}

double myVector::computeAngle(myVector target)
{
	double l1=length();
	double l2=target.length();
	double l3=dist(target);
	return acos((l1*l1+l2*l2-l3*l3)/(2*l1*l2));
}

int myVector::getOctant()
{
	int ax=(x<0)?1:0;
	int ay=(y<0)?1:0;
	int az=(z<0)?1:0;

	octant=ax*4+ay*2+az;
	return octant;
}

