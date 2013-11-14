//********************************************
// Face3d.cpp
//********************************************
// class CFace3d
//********************************************
// alliez@usc.edu
// Created : 10/12/97
// Modified : 09/02/98
//********************************************

#include "stdafx.h"

#include "Base3d.h"
#include "Line3d.h"

//////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////

//********************************************
// Constructor
//********************************************
CLine3d::CLine3d()
{
	for(int i=0;i<2;i++)
	  m_pVertex[i] = NULL;
	m_Flag = 0;
}

//********************************************
// Constructor
//********************************************
CLine3d::CLine3d(CVertex3d *pVertex1,
				 CVertex3d *pVertex2)
{
	Set(pVertex1,pVertex2);
	m_Flag = 0;
}

//********************************************
// Constructor
//********************************************
CLine3d::CLine3d(CLine3d *pLine)
{
	Set(pLine);
	m_Flag = 0;
}


//////////////////////////////////////////////
// DATAS
//////////////////////////////////////////////

//********************************************
// Set
//********************************************
inline void CFace3d::Clear()
{
	for(int i=0;i<3;i++)
	  m_pFace[i] = NULL;
	for(int i=0;i<6;i++)
	  m_pVertex[i] = NULL;
	m_Flag = 0;
}

//********************************************
// Equal
//********************************************
int CLine3d::Equal(CLine3d *pLine)
{
	return (HasVertex(pLine->v1()) && 
		      HasVertex(pLine->v2()) );
}

//********************************************
// IsValid
//********************************************
int CLine3d::IsValid()
{
	return 1;
}

//********************************************
// Set
//********************************************
inline void CLine3d::Set(CVertex3d *pVertex1,
												 CVertex3d *pVertex2)
{
	m_pVertex[0] = pVertex1;
	m_pVertex[1] = pVertex2;
}

//********************************************
// Set
//********************************************
inline void CLine3d::Set(CLine3d *pLine)
{
	Set(pLine->v1(),pLine->v2());
}

//********************************************
// SetFlagOnVerticesIfDiff
//********************************************
void CLine3d::SetFlagOnVerticesIfDiff(int FlagDiff,
																			int flag)
{
	for(int i=0;i<2;i++)
	{
		CVertex3d *pVertex = v(i);
		if(pVertex->GetFlag() != FlagDiff)
			pVertex->SetFlag((char)flag);
	}
}


//********************************************
// IndexFrom
//********************************************
int CLine3d::IndexFrom(CVertex3d *pVertex)
{
	ASSERT(HasVertex(pVertex));
	for(int i=0;i<2;i++)
		if(m_pVertex[i]==pVertex)
			return i;
	return 0;
}


//********************************************
// GetCenter
// Allocate on the heap
//********************************************
CVertex3d* CLine3d::GetCenter(void)
{
	CVertex3d* pVertex = new CVertex3d;
	pVertex->x((m_pVertex[0]->x()+m_pVertex[1]->x())/2.0f);
	pVertex->y((m_pVertex[0]->y()+m_pVertex[1]->y())/2.0f);
	pVertex->z((m_pVertex[0]->z()+m_pVertex[1]->z())/2.0f);
	return pVertex;
}

//////////////////////////////////////////////
// DATA ACCESS
//////////////////////////////////////////////

//********************************************
// GetType
//********************************************
int CLine3d::GetType()
{
	return TYPE_LINE3D;
}

//////////////////////////////////////////////
// PROCESSING
//////////////////////////////////////////////


//////////////////////////////////////////////
// MISC
//////////////////////////////////////////////

//********************************************
// HasVertex
//********************************************
int CLine3d::HasVertex(CVertex3d *pVertex)
{
	return (m_pVertex[0] == pVertex ||
		      m_pVertex[1] == pVertex );
}

//********************************************
// HasVertexWithFlag
//********************************************
int CLine3d::HasVertexWithFlag(int flag)
{
	return (m_pVertex[0]->GetFlag() == flag ||
		      m_pVertex[1]->GetFlag() == flag);
}

//********************************************
// HasVertex
//********************************************
int CLine3d::HasVertex(CVertex3d *pVertex,
											 int *index)
{
	for(int i=0;i<2;i++)
		if(m_pVertex[i] == pVertex)
			{
			*index = i;
			return 1;
			}
	return 0;
}

//********************************************
// UpdateVertex
//********************************************
int CLine3d::UpdateVertex(CVertex3d *pOld,
													CVertex3d *pNew)
{
	int index;
	if(HasVertex(pOld,&index))
		{
		(index,pNew);
		return 1;
		}
	return 0;
}

//////////////////////////////////////////////
// DEBUG
//////////////////////////////////////////////

//********************************************
// Trace
//********************************************
void CLine3d::Trace()
{
	TRACE("\n");
	TRACE("Line %x\n",this);
	TRACE("Vertices : (%g,%g,%g) (%g,%g,%g) \n",
		m_pVertex[0]->x(),m_pVertex[0]->y(),m_pVertex[0]->z(),
		m_pVertex[1]->x(),m_pVertex[1]->y(),m_pVertex[1]->z());
	// ** DEBUG **
	/*
	for(int i=0;i<3;i++)
		m_pVertex[i]->Trace();
		*/
}


//////////////////////////////////////////////
// OPENGL
//////////////////////////////////////////////

//********************************************
// glDraw
// Highlights face and its neighbors
//********************************************
void CLine3d::glDraw(unsigned char *ColorFace,
										 CMesh3d *pMesh /* = NULL */,
										 unsigned char *ColorNeightbor /* = NULL */)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Transform 
	if(pMesh != NULL)
		{
			CTransform *pTransform = pMesh->GetTransform();
		::glPushMatrix();

			// Position / translation / scaling
			glTranslatef(pTransform->GetTranslation()->x(),
									 pTransform->GetTranslation()->y(),
									 pTransform->GetTranslation()->z());

			glScalef(pTransform->GetScale()->x(),
							 pTransform->GetScale()->y(),
							 pTransform->GetScale()->z());

			glRotatef(pTransform->GetValueRotation(),
								pTransform->GetRotation()->x(),
								pTransform->GetRotation()->y(),
								pTransform->GetRotation()->z());
		}

	// Main face
	AfxMessageBox("Just Hi!");

	::glLineWidth(0.5f);
	::glBegin(GL_LINES);
		glVertex3f(m_pVertex[0]->x(),m_pVertex[0]->y(),m_pVertex[0]->z());
		glVertex3f(m_pVertex[1]->x(),m_pVertex[1]->y(),m_pVertex[1]->z());
	::glEnd();

	if(pMesh != NULL)
	{
		::glPopMatrix();
	}
}


// ** EOF **





