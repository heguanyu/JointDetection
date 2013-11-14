#include "StdAfx.h"

#include "myRay.h"

myRay::myRay(void)
{

}

myRay::~myRay(void)
{
}

double myRay::checkIntersect(face f)
{
	direction=direction.Normalize();
	double left=f.getNormal().innerProduct(myVector(f.p1).sub(startPoint));
	double right=f.getNormal().innerProduct(direction);
	if (right<0.0001) return -1;
	if(left*right<0) return -1;
	double dist=left/right;

	myVector intersectPoint=startPoint.add(direction.productReal(dist));
	myVector v1=myVector(intersectPoint).sub(myVector(f.p1));
	myVector v2=myVector(intersectPoint).sub(myVector(f.p2));
	myVector v3=myVector(intersectPoint).sub(myVector(f.p3));

	double l1=v1.length();
	double l2=v2.length();
	double l3=v3.length();

	double d12=f.p1.dist(f.p2);
	double d23=f.p2.dist(f.p3);
	double d31=f.p3.dist(f.p1);

	double a1=acos((l1*l1+l2*l2-d12*d12)/2/l1/l2); if (a1<0) a1+=PI;
	double a2=acos((l2*l2+l3*l3-d23*d23)/2/l2/l3); if(a2<0) a2+=PI;
	double a3=acos((l1*l1+l3*l3-d31*d31)/2/l1/l3); if(a3<0) a3+=PI;
	utilTool u;
	if(!u.equals(a1+a2+a3, 2*PI)) return -1;
	return dist;
}
myVector myRay::checkIntersect(myVector p1, myVector p2, myVector p3)
{
	direction=direction.Normalize();
	startPoint.x=0;startPoint.y=0;startPoint.z=0;

	face f;
	f.p1=p1;f.p2=p2;f.p3=p3;

	utilTool u;
	double left=f.getNormal().innerProduct(myVector(f.p1).sub(startPoint));
	double right=f.getNormal().innerProduct(direction);
	if (u.equals(right,0) || left*right<0) return myVector(MAXdouble,MAXdouble,MAXdouble);
	double dist=left/right;

	myVector intersectPoint=startPoint.add(direction.productReal(dist));
	return intersectPoint;
}
