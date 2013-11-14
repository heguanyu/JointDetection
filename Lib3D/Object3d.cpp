//********************************************
// Object3d.cpp
//********************************************
// class CObject3d, the base 3d object
//********************************************
// alliez@usc.edu
// Created : 17/12/97
// Modified : 15/01/98
//********************************************

#include "stdafx.h"

#include "Base3d.h"
#include "Object3d.h"


//********************************************
// GetType
//********************************************
int CObject3d::GetType()
{
	return TYPE_OBJECT3D;
}

//********************************************
// glBuildList
//********************************************
int CObject3d::glBuildList()
{
	//AfxMessageBox("CObject3d::BuildList : virtual function");
	return 1;
}

//********************************************
// glDraw
//********************************************
int CObject3d::glDraw()
{
	//AfxMessageBox("CObject3d::BuildList : virtual function");
	return 1;
}


// ** EOF **



