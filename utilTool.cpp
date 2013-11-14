#include "StdAfx.h"

#include "utilTool.h"


utilTool::utilTool(void)
{
}

utilTool::~utilTool(void)
{
}

double utilTool::getRand(double maximum)
{

	return (double)(rand()/(double)RAND_MAX*maximum);
}

double utilTool::double2(double x)
{
	return x*x;
}

double utilTool::getMin(double a,double b)
{
	return (a<b)?a:b;
}

double utilTool::getMax(double a,double b)
{
	return (a>b)?a:b;
}

double utilTool::getMax(double a, double b, double c)
{
	double maxbc=getMax(b,c);
	return getMax(a,maxbc);
}

double utilTool::getMin(double a, double b, double c)
{
	double minbc=getMin(b,c);
	return getMin(a,minbc);
}

bool utilTool::checkInt(double a)
{
	if((a-(int)a<0.0001)&&(a-(int)a>-0.0001)) return true;
	return false;
}

int utilTool::getCell(double a)
{
	if(checkInt(a))return (int)a;
	else return (int)a+1;
}

double utilTool::getY(int x1,int y1, int x2, int y2, int x0)
{
	if(x1==x2) return MAXdouble;
	double t=(double)(x0-x2+0.0)/(double)(x1-x2+0.0);
	if(t>1 || t<0) return MAXdouble;
	
	return t*y1+(1-t)*y2;
}

double utilTool::getLengthTriangle(double a, double b, double theta0, double thetax)
{
	if(abs(theta0)<0.000000001) return MAXdouble;
	if(abs(theta0-PI)<0.0000001) return 0;
	if(abs(a)<0.000000001) 
	{
		if(abs(theta0-thetax)<0.00000001) return b;
		else return 0;
	}
	double c=sqrt(a*a+b*b-2*a*b*cos(theta0));
	double B=acos((a*a+c*c-b*b)/2/a/c);
	if(B<0) B+=PI;
	double alph=PI-B-thetax;
	double result=a*sin(B)/sin(alph);
	return result;
}

bool utilTool::inAngle(double theta1, double theta2, double x)
{
	double mintheta=getMin(theta1,theta2);
	double maxtheta=getMax(theta1,theta2);
	if(abs(maxtheta-mintheta-PI)<=0.0001) return true;

	if(maxtheta-mintheta>PI)
	{
		double temp=mintheta;
		mintheta=maxtheta;
		maxtheta=temp+2*PI;
	}

	return (x>=mintheta && x<=maxtheta)||((x+2*PI>=mintheta) &&(x+2*PI<=maxtheta));

}

bool utilTool::equals(double a,double b)
{
	return (abs(a-b)<0.00000001);
}