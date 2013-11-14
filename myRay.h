#pragma once

#include "face.h"
#include "myVector.h"
#include "utilTool.h"

class myRay
{
public:

	myVector startPoint;
	myVector direction;

	//check the distance between startPoint and intersection point, return -1 if not intersected
	double checkIntersect(face f);
	myVector checkIntersect(myVector p1,myVector p2,myVector p3);

	myRay(void);
	~myRay(void);
};
