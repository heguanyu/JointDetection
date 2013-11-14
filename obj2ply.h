#pragma  once

#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include "string"

#include "face.h"
#include "myVector.h"
#include "myColor.h"

#include "ColorRamp.h"
#include "utilTool.h"
#include "Lib3D/Base3d.h"



using namespace std;

class myMain
{
public:

	static myVector* p; 
	static int p_num,f_num;
	static face* f;
	static ballPoint* queue;
	static geodist** distMatrix;
	static bool* Visible;
	static CMesh3d *pMeshDebug;

	static int selectedShowFace;

	static int USEJUNCTIONDETECTION;
	static int USEINTEGRAL;
	static int USEBOUNDARY;
	static int DEBUGMODE;

	string filename;
	string filename_noext;
	string filename_wrl;
	string filename_ply;
	string filename_viv;


	myColor* fColor;
	myColor* pColor;
	int* p_face;
	CColorRamp c;

	int minf,maxf;
	double avgf, maxi, mini ,avg;


	myMain();
	~myMain();
	void output_ply();
	void output_wrl();
	void conv_filename();
	void readfile();
	void computeVIV(bool refreshMeshDebug);
	void computeResult();
	void junction_detection();
	void loadMesh(CSceneGraph3d* pSceneGraph);
	void main(char* fileName,CSceneGraph3d *pSceneGraph ,int debugmode);
};