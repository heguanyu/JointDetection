#pragma once

#include "myVector.h"
#include "myColor.h"


#include "utilTool.h"




class ballPoint
{

public:
	double fai;
	double theta;
	double r;

	int faiIndex, thetaIndex;

	ballPoint();
	~ballPoint();
	ballPoint(myVector v);
	void getFromVector(myVector v);
	int findSpaceIndex(double x, double segLength);

};

class face
{
	geodist** distMatrix;
	myVector* p;
	int p_num;
	face* f;
	int f_num;
	ballPoint* queue;
	bool* Visible;
	CMesh3d* pMeshDebug;

	bool initialized;


public:
	int p1index;
	int p2index;
	int p3index;													//the index of three vertexes of the triangle

	double thetaseglength;
	double faiseglength;

	int validNum;
	
	myVector p1,p2,p3;								//the Euclid Coordinator of thtree vertexes
	
	double area;

	myVector center;
	myVector normal;



	myVector getCenter();				//return the center of the surface
	myVector getNormal();				//return the normal of the surface
	double getArea();						//return the area of the surface

	double maximumInnerSphere(int now, int sampleNum, double coneAngle);
														//return the radius of the maximum Inner sphere that inscribed on the surface
	double visibleArea2(int now);									//abandoned function
	double visibleVolume(int now, int DEBUGMODE);	//the calculation of VIV on surface "now"
	double visibleVolume2(int now);								//abandoned function

	double getZAxis(myVector a, myVector b, myVector c);			//abandoned function

	bool checkInSurfaceTriangle(ballPoint p0, ballPoint a, ballPoint b, ballPoint c);	
								//check whether p0 is in the geodesic triangle abc, in ball-coordinator 
	bool inPlaneTriangle(myVector p0, myVector a, myVector b, myVector c);
						//check whether p0 is in the triangle abc, in Euclid Space
	void initialize();

	face(void);
	~face(void);
};
