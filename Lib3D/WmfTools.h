//********************************************
// WmfTools.h
//********************************************
//********************************************

#ifndef _WMF_TOOLS_
#define _WMF_TOOLS_

#include "Avlind.h"


// A projected face
class CWmfFace
{
public :
	double x1,x2,x3,xc;
	double y1,y2,y3,yc;
	double z1,z2,z3,zc;
	unsigned char m_Draw;
	AVLStruct_t avl;

public :

	// Constructor
	CWmfFace() { m_Draw = 0; }
	~CWmfFace() {}

};


#endif // _WMF_TOOLS_

