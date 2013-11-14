// DialogWmf.cpp : implementation file
//

#include "stdafx.h"
#include "mesh.h"
#include "DialogWmf.h"

#include "MeshDoc.h"
#include "MeshView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWmf dialog


CDialogWmf::CDialogWmf(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWmf::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogWmf)
	m_Ratio = 30.0;
	m_RatioNbFaces = 0.0f;
	//}}AFX_DATA_INIT
}

CDialogWmf::CDialogWmf(CWnd* pParent,
											 CMeshDoc *pDoc)
	: CDialog(CDialogWmf::IDD, pParent)
{
	m_pDoc = pDoc;
	m_ColorLine = RGB(0,0,0);
	m_ColorFace = RGB(255,255,255);
	m_Mode = MODE_LINE;
	m_Ratio = 30.0;
	m_RatioNbFaces = 1.0f;
}

//*********************************
// OnInitDialog 
//*********************************
BOOL CDialogWmf::OnInitDialog() 
{
	CDialog::OnInitDialog();
	((CButton *)GetDlgItem(IDC_RADIO_LINE))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO_FACE))->SetCheck(0);
	((CButton *)GetDlgItem(ID_COLOR_VIV))->SetCheck(1);
	((CButton *)GetDlgItem(ID_COLOR_DIFFSCORE))->SetCheck(1);
	((CButton *)GetDlgItem(ID_COLOR_GAUSS_INTEGRAL))->SetCheck(1);
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		m_tooltip.AddTool(GetDlgItem(IDC_EDIT_RATIO_NBFACES), "Enter the face number ratio (1.0 means that every face are taken into account)");
		m_tooltip.AddTool(GetDlgItem(IDC_EDIT_RATIO), "Select the scaling ratio (30.0 is default)");
		m_tooltip.AddTool(GetDlgItem(IDC_COLOR_FACE), "Click into this frame to select the face color");
		m_tooltip.AddTool(GetDlgItem(IDC_COLOR_LINE), "Click into this frame to select the line color");
		m_tooltip.AddTool(GetDlgItem(IDC_RADIO_FACE), "Faces are sorted according to z, filled and outlined");
		m_tooltip.AddTool(GetDlgItem(IDC_RADIO_LINE), "Edges are painted without sorting nor culling");
	}
	checkedItem=0;
	return TRUE;
}


void CDialogWmf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWmf)
	DDX_Control(pDX, IDC_COLOR_FACE, m_FrameFace);
	DDX_Control(pDX, IDC_COLOR_LINE, m_FrameLine);
	DDX_Text(pDX, IDC_EDIT_RATIO, m_Ratio);
	DDX_Text(pDX, IDC_EDIT_RATIO_NBFACES, m_RatioNbFaces);
	DDV_MinMaxFloat(pDX, m_RatioNbFaces, 1.e-003f, 1.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWmf, CDialog)
	//{{AFX_MSG_MAP(CDialogWmf)
	ON_BN_CLICKED(ID_BUTTON_RUN, OnButtonRun)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_RADIO_FACE, OnRadioFace)
	ON_BN_CLICKED(IDC_RADIO_LINE, OnRadioLine)


	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWmf message handlers

//*********************************
// OnButtonRun 
//*********************************
void CDialogWmf::OnButtonRun() 
{
	
	BeginWaitCursor();

	UpdateData(TRUE);

	// Get DC
	CDC *pDC = m_pDoc->GetView()->GetDC();
	ASSERT(pDC);

	// Get view rect
	CRect rect;
	m_pDoc->GetView()->GetClientRect(&rect);
	rect.InflateRect(5,5);

	// Create metafile device context
	HDC hMetaDC = CreateEnhMetaFile(pDC->m_hDC,"metafile.emf",NULL,NULL);
	if(!hMetaDC)
	{
		AfxMessageBox("Unable to create MetaFile");
		ReleaseDC(pDC);
		return;
	}

	// Get DC from handle
	CDC *pMetaDC = CDC::FromHandle(hMetaDC);
	ASSERT(pMetaDC);
	pMetaDC->SetMapMode(MM_TEXT); 

	// Position / translation / scale
	glPushMatrix();
	CMeshView *pView = (CMeshView *)m_pDoc->GetView();
	glTranslated(pView->m_xTranslation,pView->m_yTranslation,pView->m_zTranslation);
	glRotatef(pView->m_xRotation, 1.0, 0.0, 0.0);
	glRotatef(pView->m_yRotation, 0.0, 1.0, 0.0);
	glRotatef(pView->m_zRotation, 0.0, 0.0, 1.0);
	glScalef(pView->m_xScaling,pView->m_yScaling,pView->m_zScaling);

	// Get OpenGL parameters
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
	glGetIntegerv(GL_VIEWPORT,viewport);

	// Start rendering via std GDI 2D drawing functions
	CSceneGraph3d *pScene = &m_pDoc->m_SceneGraph;
	for(int i=0;i<pScene->NbObject();i++)
	{
		CObject3d *pObject = pScene->GetAt(i);
		if(pObject->GetType() == TYPE_MESH3D) // meshes only
			// The line mode (no sort)
			if(m_Mode == MODE_LINE) 
				((CMesh3d *)pObject)->glDrawProjectLine(pMetaDC,
				                                        modelMatrix,
																								projMatrix,
																								viewport,
																								m_ColorLine,
																								m_Ratio,
																								rect.Height());
			else 
				// The face mode (faces are z-sorted 
				// according to their barycenter)
				((CMesh3d *)pObject)->glDrawProjectFace(pMetaDC,
				                                        modelMatrix,
																								projMatrix,
																								viewport,
																								m_ColorLine,
																								m_ColorFace,
																								m_Ratio,
																								rect.Height(),
																								m_RatioNbFaces);
	}

	glPopMatrix();

	// Close metafile
	HENHMETAFILE hMetaFile = CloseEnhMetaFile(hMetaDC);

	// Fill the clipboard (direct sent to wmf2eps or 
	// any windows app such as Powerpoint)
	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_ENHMETAFILE,CopyEnhMetaFile(hMetaFile,NULL));
	CloseClipboard();

	// Cleanup
	DeleteEnhMetaFile(hMetaFile);
	ReleaseDC(pDC);

	EndWaitCursor();
}

void CDialogWmf::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	CBrush brush;
	
	// Line color
	m_FrameLine.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	brush.CreateSolidBrush(m_ColorLine);
	dc.FillRect(&rect,&brush);
	brush.DeleteObject();

	// Face color
	m_FrameFace.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	brush.CreateSolidBrush(m_ColorFace);
	dc.FillRect(&rect,&brush);
	brush.DeleteObject();
}

//*********************************
// OnLButtonUp 
//*********************************
void CDialogWmf::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rect;

	// Line color
	m_FrameLine.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(m_ColorLine);
		if(dlg.DoModal()==IDOK)
		{
			m_ColorLine = dlg.GetColor();	
			InvalidateRect(&rect,FALSE);
		}
	}

	// Face color
	m_FrameFace.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(m_ColorFace);
		if(dlg.DoModal()==IDOK)
		{
			m_ColorFace = dlg.GetColor();	
			InvalidateRect(&rect,FALSE);
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

///////////////////////////////////
// Options (face or line)
///////////////////////////////////

//*********************************
// OnRadioFace 
//*********************************
void CDialogWmf::OnRadioFace() 
{
	AfxMessageBox("sayhi");
	m_Mode = MODE_FACE;
	((CButton *)GetDlgItem(IDC_RADIO_LINE))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_RADIO_FACE))->SetCheck(1);
}

//*********************************
// OnRadioFace 
//*********************************
void CDialogWmf::OnRadioLine() 
{
	m_Mode = MODE_LINE;
	((CButton *)GetDlgItem(IDC_RADIO_LINE))->SetCheck(1);
	((CButton *)GetDlgItem(IDC_RADIO_FACE))->SetCheck(0);
}



BOOL CDialogWmf::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}


