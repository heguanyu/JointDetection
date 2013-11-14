//********************************************
// Face3d.h
// class CFace3d
//********************************************
// A triangular face :
// + three vertex ref (subdivision added)
// + three faces ref
// + normal vector
// + color
// + material ref
//********************************************
// m_pFace : shares edges
//********************************************
// alliez@usc.edu
// Created : 15/12/97
// Modified : 18/02/98
//********************************************

#ifndef _LINE_3D_
#define _LINE_3D_

#include "Object3d.h"
#include "Vector3d.h"
#include "Color.h"

class CVertex3d;
class CVectorSet3d;
class CMesh3d;
template <class T>class CArray3d;

class CLine3d : public CObject3d
{

private :

	CVertex3d *m_pVertex[2]; 
	CColor m_Color;               // Color
	unsigned int m_IndexMaterial; // Material
	int m_Flag;                   // Flag (processings)

public :

	// Constructors
	CLine3d();
	CLine3d(CVertex3d *pVertex1,CVertex3d *pVertex2);
	CLine3d(CLine3d *pLine);
	virtual ~CLine3d() {}

	// General
	int IsValid();
	void Clear();
	int Equal(CLine3d *pLine);
	void Set(CVertex3d *pVertex1,CVertex3d *pVertex2);
	void Set(CLine3d *pLine);
	CVertex3d* GetCenter(void);

	// Color
	CColor *GetColor(void) { return &m_Color; }
	void SetColor(CColor &color) { m_Color.Set(color); }
	void SetColor(unsigned char r,unsigned char g,unsigned char b) { m_Color.Set(r,g,b); }
	void SetColor(unsigned char r,unsigned char g,unsigned char b, unsigned char a){m_Color.Set(r,g,b,a);}

	// Per vertex
	void v1(CVertex3d *pVertex) { m_pVertex[0] = pVertex; }
	void v2(CVertex3d *pVertex) { m_pVertex[1] = pVertex; }

	int IndexFrom(CVertex3d *pVertex);
		
	// Per vertex
	CVertex3d *v1(void) { return m_pVertex[0]; }
	CVertex3d *v2(void) { return m_pVertex[1]; }
	CVertex3d *v(int index) { return m_pVertex[index%2]; }


	// Flag
	int GetFlag(void) { return m_Flag; }
	void SetFlag(int flag) { m_Flag = flag; }
	void SetFlagOnVerticesIfDiff(int FlagDiff,int flag);

	// Find whenever face contain pVertex
	int HasVertex(CVertex3d *pVertex);
	int HasVertex(CVertex3d *pVertex,int *index);
	int HasVertexWithFlag(int flag);

	// Find whenever face contain pOld, and update it to pNew
	int UpdateVertex(CVertex3d *pOld,CVertex3d *pNew);


	virtual int GetType();

	// OpenGL
	void glDraw(unsigned char *ColorLine,CMesh3d *pMesh = NULL,unsigned char *ColorNeightbor = NULL);

	// Debug
	void Trace();

	// Geometric values

};

#endif // _LINE_3D_
