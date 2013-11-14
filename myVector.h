#pragma once

#include "utilTool.h"

class myVector
{
public:

	double x,y,z;

	int octant;

	myVector(void);
	myVector(double xx, double yy, double zz);
	~myVector(void);

	myVector Normalize();											 //return the normalized vector, indicating that the length is 1
	myVector productReal(double number);			//return the answer of this vector * real number
	double innerProduct(myVector v2);					//return the answer of this vector innerproduct v2
	myVector sub(myVector v2);									//return the answer of this vector-v2
	myVector add(myVector v2);								//return the answer of this vector+v2
	double length();														//return the length of this vector
	int getOctant();															//return the octant of this vector
	double dist(myVector target);								//return the distance between two vectors
	double computeAngle(myVector target);			//return the angel between two vectors
};
