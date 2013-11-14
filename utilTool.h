#pragma once

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Lib3D/Base3d.h"



typedef struct edge
{
	int toVertexIndex;
	int f1Index;
	int f2Index;
}edge;

typedef struct geodist
{
	double dist;
	int toFaceIndex;
	int times;
}geodist;

class utilTool
{
public:

	double getRand(double maximum);									//return a random num smaller than maximum
	double double2(double x);													//return x^2
	double getMin(double a, double b, double c);					//return the minimum of a,b,c
	double getMax(double a, double b, double c);				//return the maximum of a,b,c
	double getMin(double a, double b);									//return the minimum of a,b
	double getMax(double a, double b);									//return the maximum of a,b
	double getY(int x1,int y1, int x2, int y2, int x0);					//(x0,y0)is on the line defined by (x1,y1)(x2,y0),return y0
	double getLengthTriangle(double a, double b, double theta0, double thetax);
								//compute the length of a edge on the triangle, the length of the other
								//with the length of the other two edges and angels are given																	
	bool inAngle(double theta1, double theta2, double x);
								//refelct whether angel x is between angel theta1 and theta2. It is functionally abandoned. Never used	
	int getCell(double a);
								//get the min integer that larger than a
	bool checkInt(double a);
								//check wheter a double a is close enough to an integer
	bool equals(double a,double b);
								//check whether two double number are equal

	static double** distMatrix;

	utilTool(void);
	~utilTool(void);
};
