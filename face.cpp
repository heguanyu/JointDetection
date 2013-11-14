#include "StdAfx.h"

#include "face.h"
#include "myRay.h"
#include "obj2ply.h"

#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include "string"

using namespace std;

static double FAIRANGE=PI;
static double THETARANGE=2*PI;


static int  precesion=PRECESION;
static int segmentnum=2*PRECESION;

static double FAISEGLENGTH=FAIRANGE/segmentnum;
static double THETASEGLENGTH=THETARANGE/segmentnum;

double depthBuffer[SEGMENTNUM][SEGMENTNUM];
bool available[SEGMENTNUM][SEGMENTNUM];
int depthIndex[SEGMENTNUM][SEGMENTNUM];
int inTriangle[SEGMENTNUM][SEGMENTNUM];

int refreshqueuetheta[SEGMENTNUM*SEGMENTNUM];
int refreshqueueyfai[SEGMENTNUM*SEGMENTNUM];
int refreshqueuelength;




#define  UNCHECK 0
#define  INTHETRIANGLE 1
#define OUTOFTRIANGLE 2


	
ballPoint::ballPoint()
{
	fai=0;theta=0;r=0;faiIndex=0;thetaIndex=0;
}
ballPoint::~ballPoint()
{
	fai=0;theta=0;r=0;faiIndex=0;thetaIndex=0;
}
ballPoint::ballPoint(myVector v)
{
	getFromVector(v);
}

void ballPoint::getFromVector(myVector v)
{
	if(v.z<0.00000001 && v.z>-0.00000001) fai=PI/2;
	else
	{
		fai=atan(sqrt(v.x*v.x+v.y*v.y)/v.z);
		if(fai<0) fai+=PI;
	}

	if(v.x<0.00000001 && v.x>-0.00000001)
	{
		theta=(v.y>0)?PI/2:PI*3/2;
	}
	else
	{
		theta=atan(v.y/v.x);
		if(v.x<0)theta+=PI;
		else if(v.y<0)theta+=PI*2;
	}
	r=v.length();
	faiIndex=findSpaceIndex(fai,FAISEGLENGTH);
	if(faiIndex==0 || faiIndex==segmentnum-1) thetaIndex=0;
	else thetaIndex=findSpaceIndex(theta,THETASEGLENGTH);
}

int ballPoint::findSpaceIndex(double x, double segLength)
{
	int ans=(int)(x/segLength);
	if(ans==segmentnum) ans--;
	return ans;
}

void face::initialize()
{
	distMatrix=myMain::distMatrix;
	p=myMain::p;
	p_num=myMain::p_num;
	f=myMain::f;
	f_num=myMain::f_num;
	queue=myMain::queue;
	initialized=true;
}

face::face(void)
{
	thetaseglength=THETASEGLENGTH;
	faiseglength=FAISEGLENGTH;
	initialized=false;
}

face::~face(void)
{
}

double face::getZAxis(myVector a, myVector b, myVector c)
{
	double x1=a.x,y1=a.y,z1=a.z;
	double x2=b.x,y2=b.y,z2=b.z;
	double x3=c.x,y3=c.y,z3=c.z;

	double aa=x2*y3-x3*y2, bb=x1*y3-x3*y1, cc=x1*y2-x2*y1;
	double A=aa+bb+cc;
	if(A==0) return 1;
	aa/=A;bb/=A;cc/=A;
	return aa*z1+bb*z2+cc*z3;
}

myVector face::getCenter()
{
	center.x=(p1.x+p2.x+p3.x)/3.0;
	center.y=(p1.y+p2.y+p3.y)/3.0;
	center.z=(p1.z+p2.z+p3.z)/3.0;
	return center;
}

myVector face::getNormal()
{
	myVector a=p2.sub(p1);
	myVector b=p3.sub(p2);

	myVector answer(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);
	normal=answer.Normalize();
	return normal;
}

double face::maximumInnerSphere(int now,int sampleNum, double coneAngle)
{
	utilTool u;
	double maxr=tan(coneAngle/2);
	normal=getNormal();
	myVector bottomPoint= center.sub(normal);

	double maxDiameter=MAXdouble;


	for(int i=0;i<sampleNum;i++)
	{
		double tmp=0;
	
		myRay ray;	
		myVector targetPoint=bottomPoint;
		ray.startPoint=center;
		ray.direction=targetPoint.sub(center);

		for(int j=0;j<f_num;j++) if(j!=now)
		{
		
			if(center.dist(f[j].center)>maxDiameter)continue;
			tmp=ray.checkIntersect(f[j]);
			if(tmp<0) continue;
			if(tmp<maxDiameter)
			{
				maxDiameter=tmp;
			}
		}

	}

	if(maxDiameter==MAXdouble)return 0;
	return maxDiameter/2;

//	if(maxDiameter==MAXdouble)return center;

	//return center.sub(normal.productReal(maxDiameter/2));

}


double face::getArea()
{
	myVector a=p3.sub(p1);
	myVector b=p2.sub(p1);
	myVector c=p2.sub(p3);

	double la=a.length();
	double lb=b.length();
	double lc=c.length();

	
	double answer=0.5*la*lb*sin(acos((la*la+lb*lb-lc*lc)/(2*la*lb)));
	if (answer<0) answer=-answer;
	
	area=answer;
	return answer;
}


double face::visibleArea2(int now)
{

	for(int i=0;i<SEGMENTNUM;i++)for(int j=0;j<SEGMENTNUM;j++)depthBuffer[i][j]=MAXdouble;

	ballPoint fp;
	utilTool u;
	for(int i=0;i<p_num;i++)
	{
		if(getNormal().innerProduct(p[i].sub(center).Normalize())>0)continue;
		fp.getFromVector(p[i].sub(center));
		depthBuffer[fp.thetaIndex][fp.faiIndex]=u.getMin(depthBuffer[fp.thetaIndex][fp.faiIndex],fp.r);
	}

	double answer=0;
	for(int i=0;i<SEGMENTNUM;i++)for(int j=0;j<SEGMENTNUM;j++)
	{
		if(depthBuffer[i][j]<MAXdouble)
			answer+=depthBuffer[i][j]*depthBuffer[i][j];
	}
	return answer;
}

double face::visibleVolume(int nowf ,int DEBUGMODE)
{

	utilTool u;

	ballPoint f_p1,f_p2,f_p3,f_center;

	if(!initialized)initialize();
	
	//初始化深度缓存depthbuffer
	//available表示某个方向是否在体内
	for(int i=0;i<segmentnum;i++)for(int j=0;j<segmentnum;j++)
	{
		depthBuffer[i][j]=MAXdouble;
		myVector x(sin(FAISEGLENGTH*j)*cos(THETASEGLENGTH*i),sin(FAISEGLENGTH*j)*sin(THETASEGLENGTH*i),cos(FAISEGLENGTH*j));
		available[i][j]=(normal.innerProduct(x.Normalize())<-0.02);
	}

	ballPoint now,next;
	myVector centerPoint=center;

	//inTriangle表示某个方向的深度缓存是否被特定三角面覆盖。这个数组每扫描一个面都会被重置一次
	//refreshqueue用来记录需要被重置的深度缓存。这样可以大幅提升运算速度


	refreshqueuelength=0;
	for(int i=0;i<SEGMENTNUM;i++)for(int j=0;j<SEGMENTNUM;j++)inTriangle[i][j]=UNCHECK;
						
	for(int i=0;i<f_num;i++)if(i!=nowf)
	{
		//检查这个面是否通过体内可以看到（连线与起始面的内法向是同向的）
		if(normal.innerProduct(f[i].getCenter().sub(centerPoint).Normalize())>-0.02)continue;
		if(f[i].getNormal().innerProduct(centerPoint.sub(f[i].center).Normalize())>-0.02) continue;


		f_center.getFromVector(f[i].getCenter().sub(centerPoint));
		f_p1.getFromVector(f[i].p1.sub(centerPoint));
		f_p2.getFromVector(f[i].p2.sub(centerPoint));
		f_p3.getFromVector(f[i].p3.sub(centerPoint));
		//检查这个面是否已经比深度缓存中的数值远了
		if(f_p1.r>depthBuffer[f_p1.thetaIndex][f_p1.faiIndex] &&
			f_p2.r>depthBuffer[f_p2.thetaIndex][f_p2.faiIndex] &&
			f_p3.r>depthBuffer[f_p3.thetaIndex][f_p3.faiIndex] &&
			f_center.r>depthBuffer[f_center.thetaIndex][f_center.faiIndex])
			continue;

		//按照refreshqueue重置inTriangle矩阵
		for(int j=0;j<refreshqueuelength;j++) inTriangle[refreshqueuetheta[j]][refreshqueueyfai[j]]=UNCHECK;

		int head=0,tail=1;
		
		queue[0]=f_center;

		//将目标面的中心点标记为inthetriangle，并且进入队列
		inTriangle[f_center.thetaIndex][f_center.faiIndex]=INTHETRIANGLE;

		refreshqueuelength=1;
		refreshqueuetheta[0]=f_center.thetaIndex;
		refreshqueueyfai[0]=f_center.faiIndex;
		
		//修改面中心点对应的深度缓存的值
		//if(checkInSurfaceTriangle(f_center,f_p1,f_p2,f_p3))
		{

			if(depthBuffer[f_center.thetaIndex][f_center.faiIndex]>f_center.r) depthIndex[f_center.thetaIndex][f_center.faiIndex]=i;
			depthBuffer[f_center.thetaIndex][f_center.faiIndex]=u.getMin(depthBuffer[f_center.thetaIndex][f_center.faiIndex],f_center.r);
		}

		//开始广度优先搜索
		while(head<tail)
		{
			now=queue[head];
		
			if(now.faiIndex>0 && now.faiIndex<segmentnum-1)
			{
				//广度优先：向8个方向扩展。有两点非常重要:
				//1. theta=0和2pi的位置是同一个点，程序中注意循环处理
				//2. phi=0和pi的位置上，对于任何theta值都是同一个点（参考南北极点），所以一律计算到theta=0上
				//对于每一个扩展的点检查其是否被单位球上的测地三角面投影覆盖。如果被覆盖，则将其加入队列，否则标记为out
				for(int dtheta=-1; dtheta<=1; dtheta++) for(int dfai=-1;dfai<=1;dfai++)
				{
					next=now;
					next.thetaIndex+=dtheta;next.thetaIndex+=segmentnum;next.thetaIndex%=segmentnum;
					next.faiIndex+=dfai;
					if(next.faiIndex==0 || next.faiIndex==segmentnum-1)next.thetaIndex=0;
					if(inTriangle[next.thetaIndex][next.faiIndex]==UNCHECK)
					{
						refreshqueuetheta[refreshqueuelength]=next.thetaIndex;
						refreshqueueyfai[refreshqueuelength]=next.faiIndex;
						refreshqueuelength++;
						if(checkInSurfaceTriangle(next,f_p1,f_p2,f_p3))
						{
							inTriangle[next.thetaIndex][next.faiIndex]=INTHETRIANGLE;
							if(depthBuffer[next.thetaIndex][next.faiIndex]>f_center.r)depthIndex[next.thetaIndex][next.faiIndex]=i;
							depthBuffer[next.thetaIndex][next.faiIndex]=u.getMin(depthBuffer[next.thetaIndex][next.faiIndex],f_center.r);
							queue[tail]=next;
							tail++;
						}
						else
						{
							inTriangle[next.thetaIndex][next.faiIndex]=OUTOFTRIANGLE;
						}
					}
				}
			}
			else 
			{
				next.faiIndex=(now.faiIndex==0)?1:segmentnum-2;
				for(int j=0;j<segmentnum;j++)
				{
					next.thetaIndex=j;
					if(inTriangle[next.thetaIndex][next.faiIndex]==UNCHECK)
					{

						refreshqueuetheta[refreshqueuelength]=next.thetaIndex;
						refreshqueueyfai[refreshqueuelength]=next.faiIndex;
						refreshqueuelength++;

						if(checkInSurfaceTriangle(next,f_p1,f_p2,f_p3))
						{
							inTriangle[next.thetaIndex][next.faiIndex]=INTHETRIANGLE;
							if(depthBuffer[next.thetaIndex][next.faiIndex]>f_center.r)depthIndex[next.thetaIndex][next.faiIndex]=i;
							depthBuffer[next.thetaIndex][next.faiIndex]=u.getMin(depthBuffer[next.thetaIndex][next.faiIndex],f_center.r);
							queue[tail]=next;
							tail++;
						}
						else
						{
							inTriangle[next.thetaIndex][next.faiIndex]=OUTOFTRIANGLE;
						}
					}
				}
			}
			head++;
		}
	}


	//计算结果：按照公式的计算方法来进行积分，也就是累加sin(phi)*depthbuffer^3

	double answer=0;
	validNum=0;
	for(int i=0;i<segmentnum;i++)for(int j=0;j<segmentnum;j++)
	{
		if(depthBuffer[i][j]<MAXdouble && available[i][j])
		{
			answer+=depthBuffer[i][j]*depthBuffer[i][j]*sin(j*FAISEGLENGTH)*depthBuffer[i][j];
			myMain::Visible[depthIndex[i][j]]=true;
			validNum++;
		}
	}
	//如果需要查看深度缓存，还要添加那些射线
	if(DEBUGMODE==CHECKVISIBILITY)
	{
		CVertex3d* myCenter=new CVertex3d(center.x,center.y,center.z);
		myMain::pMeshDebug->m_ArrayLine.SetSize(validNum);

		int now=0;
		for(int i=0;i<segmentnum;i++)for(int j=0;j<segmentnum;j++)
		{
			myVector zengjia(
									cos(i*THETASEGLENGTH)*sin(j*FAISEGLENGTH),
									sin(i*THETASEGLENGTH)*sin(j*FAISEGLENGTH),
									cos(j*FAISEGLENGTH));
			if(depthBuffer[i][j]<MAXdouble && available[i][j])
			{
				myVector terminal=getCenter().add(zengjia.productReal(depthBuffer[i][j]));
				CLine3d* pLine=new CLine3d(myCenter,new CVertex3d(terminal.x,terminal.y,terminal.z));
				myMain::pMeshDebug->m_ArrayLine.SetAt(now,pLine);
				now++;
			}
		}
	}
	return answer;
}




bool face::checkInSurfaceTriangle(ballPoint p0, ballPoint a, ballPoint b, ballPoint c)
{
	double x0=sin(p0.faiIndex*FAISEGLENGTH)*cos(p0.thetaIndex*THETASEGLENGTH),
				  x1=sin(a.fai)*cos(a.theta),
				  x2=sin(b.fai)*cos(b.theta),
				  x3=sin(c.fai)*cos(c.theta),
				 y0=sin(p0.faiIndex*FAISEGLENGTH)*sin(p0.thetaIndex*THETASEGLENGTH),
				 y1=sin(a.fai)*sin(a.theta),
				 y2=sin(b.fai)*sin(b.theta),
				 y3=sin(c.fai)*sin(c.theta),
				 z0=cos(p0.faiIndex*FAISEGLENGTH),
				 z1=cos(a.fai),
				 z2=cos(b.fai),
				 z3=cos(c.fai);
	myRay r;
	r.startPoint=myVector(0,0,0);
	r.direction=myVector(x0,y0,z0);
	myVector p1(x1,y1,z1);myVector p2(x2,y2,z2);myVector p3(x3,y3,z3);
	myVector intersect=r.checkIntersect(p1,p2,p3);
	if(intersect.x==MAXdouble) return false;
	return inPlaneTriangle(intersect, p1,p2,p3);
	
}

bool face::inPlaneTriangle(myVector p0, myVector a, myVector b, myVector c)
{
	double l12=a.dist(b),
				  l23=b.dist(c),
				  l31=c.dist(a),
				  l1=p0.dist(a),
				  l2=p0.dist(b),
				  l3=p0.dist(c),
				  a12=acos((l1*l1+l2*l2-l12*l12)/2/l1/l2),
				  a23=acos((l2*l2+l3*l3-l23*l23)/2/l3/l2),
				  a31=acos((l1*l1+l3*l3-l31*l31)/2/l1/l3);
	utilTool u;
	return (a12+a23+a31-2*PI)>-2.0;
}

