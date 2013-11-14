// MeshView.cpp : implementation of the CMeshView class
//

#include "stdafx.h"
#include "Mesh.h"

#include "MeshDoc.h"
#include "MeshView.h"

#include "Lib3d/PsRender.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshView

IMPLEMENT_DYNCREATE(CMeshView, CView)

BEGIN_MESSAGE_MAP(CMeshView, CView)
	//{{AFX_MSG_MAP(CMeshView)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EXPORT_EPS, OnExportEps)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_EPS, OnUpdateExportEps)
	ON_COMMAND(ID_OPENGL_CLEARCOLOR, OnOpenglClearcolor)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshView construction/destruction

CMeshView::CMeshView()
{
	// OpenGL
	m_hGLContext = NULL;
	m_GLPixelIndex = 0;
	
	// Mouse
	m_LeftButtonDown = FALSE;
	m_RightButtonDown = FALSE;

	// Colors
	m_ClearColorRed   = 0.96f;
	m_ClearColorGreen = 0.96f;
	m_ClearColorBlue  = 0.96f;

	// Animation
	m_StepRotationX = 0.0f;
	m_StepRotationY = 5.0f;
	m_StepRotationZ = 0.0f;

	InitGeometry();
}

CMeshView::~CMeshView()
{
}

//********************************************
// InitGeometry
//********************************************
void CMeshView::InitGeometry(void)
{
	m_xRotation = 0.0f;
	m_yRotation = 0.0f;
	m_zRotation = 0.0f;

	m_xTranslation = 0.0f;
	m_yTranslation = 0.0f;
	m_zTranslation = -5.0f;

	m_xScaling = 1.0f;
	m_yScaling = 1.0f;
	m_zScaling = 1.0f;

	m_SpeedRotation = 1.0f / 3.0f;
	m_SpeedTranslation = 1.0f / 50.0f;

	m_xyRotation = 1;
}

BOOL CMeshView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMeshView drawing

void CMeshView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CMeshView printing

BOOL CMeshView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMeshView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CMeshView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CMeshView diagnostics

#ifdef _DEBUG
void CMeshView::AssertValid() const
{
	CView::AssertValid();
}

void CMeshView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMeshDoc* CMeshView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMeshDoc)));
	return (CMeshDoc*)m_pDocument;
}
#endif //_DEBUG



//////////////////////////////////////////////
//////////////////////////////////////////////
// OPENGL
//////////////////////////////////////////////
//////////////////////////////////////////////


//********************************************
// OnCreate
// Create OpenGL rendering context 
//********************************************
int CMeshView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);

	if(SetWindowPixelFormat(hDC)==FALSE)
		return 0;
	
	if(CreateViewGLContext(hDC)==FALSE)
		return 0;

	// Default mode
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);

  glShadeModel(GL_FLAT);
	glEnable(GL_NORMALIZE);

	// Lights properties
  float	ambientProperties[]  = {1.0f, 1.0f, 1.0f, 1.0f};
  float	diffuseProperties[]  = {1.0f, 1.0f, 1.0f, 1.0f};
  float	specularProperties[] =  {1.0f, 1.0f, 1.0f, 1.0f};
  float positionProperties[]={100.0f,100.0f,100.0f,0};
	
  glLightfv(GL_LIGHT0,GL_POSITION,positionProperties);
  glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
  glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
  glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientProperties);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

	glClearColor(m_ClearColorRed,m_ClearColorGreen,m_ClearColorBlue,1.0f);
	glClearDepth(1.0f);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// Perspective
	CRect rect;
	GetClientRect(&rect);
	double aspect = (rect.Height() == 0) ? rect.Width() : (double)rect.Width()/(double)rect.Height();
	gluPerspective(45,aspect,0.1,1000.0);

	// Default : lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	

	// Default : blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Default : material
  float	MatAmbient[]  = {0.0f, 0.5f, 0.75f, 1.0f};
  float	MatDiffuse[]  = {0.0f, 0.5f, 1.0f, 1.0f};
  float	MatSpecular[]  = {0.75f, 0.75f, 0.75f, 1.0f};
  float	MatShininess[]  = { 200 };
  float	MatEmission[]  = {0.0f, 0.0f, 0.0f, 1.0f};
  float MatAmbientBack[]  = {0.0f, 0.5f, 0.0f, 1.0f}; // green material behind

	glMaterialfv(GL_FRONT,GL_AMBIENT,MatAmbient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,MatDiffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,MatSpecular);
	glMaterialfv(GL_FRONT,GL_SHININESS,MatShininess);
	glMaterialfv(GL_FRONT,GL_EMISSION,MatEmission);
	glMaterialfv(GL_BACK,GL_AMBIENT,MatAmbientBack);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// CMeshView message handlers

//*********************************
// OnPaint 
//*********************************
void CMeshView::OnPaint() 
{
	// Device context for painting
	CPaintDC dc(this); 
	
	// Model is stored in Document
	CMeshDoc *pDoc = (CMeshDoc *)GetDocument();
	//ASSERT_VALID(pDoc);

	// Useful in multidoc templates
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC,m_hGLContext);
	
	glClearColor(m_ClearColorRed,m_ClearColorGreen,m_ClearColorBlue,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	
	// Position / translation / scale
	glTranslated(m_xTranslation,m_yTranslation,m_zTranslation);
	glRotatef(m_xRotation, 1.0, 0.0, 0.0);
	glRotatef(m_yRotation, 0.0, 1.0, 0.0);
	glRotatef(m_zRotation, 0.0, 0.0, 1.0);
	glScalef(m_xScaling,m_yScaling,m_zScaling);

	// Start rendering...
	pDoc->RenderScene();
	
	glPopMatrix();

	// Double buffer
	SwapBuffers(dc.m_ps.hdc);
	glFlush();

	// Release
	::ReleaseDC(hWnd,hDC);
}


//********************************************
// SetWindowPixelFormat
//********************************************
BOOL CMeshView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;
	
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	
	m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
	if(m_GLPixelIndex == 0) // Choose default
	{
		m_GLPixelIndex = 1;
		if(DescribePixelFormat(hDC,m_GLPixelIndex,
			sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
			return FALSE;
	}
	
	if(!SetPixelFormat(hDC,m_GLPixelIndex,&pixelDesc))
		return FALSE;
	return TRUE;
}



//********************************************
// CreateViewGLContext
// Create an OpenGL rendering context
//********************************************
BOOL CMeshView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	
	if(m_hGLContext==NULL)
		return FALSE;
	
	if(wglMakeCurrent(hDC,m_hGLContext)==FALSE)
		return FALSE;
	
	return TRUE;
}

//*********************************
// OnDestroy 
//*********************************
void CMeshView::OnDestroy() 
{
	
	if(wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL,NULL);
	
	if(m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
	CView::OnDestroy();

}

//*********************************
// OnSize 
//*********************************
void CMeshView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC,m_hGLContext);

	// Set OpenGL perspective, viewport and mode
	double aspect = (cy == 0) ? cx : (double)cx/(double)cy;
	
	glViewport(0,0,cx,cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,aspect,0.1,1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDrawBuffer(GL_BACK);

	// Release
	::ReleaseDC(hWnd,hDC);
}

void CMeshView::pickIt()
{
	if(GetDocument()->o.DEBUGMODE==0)return;

	CPoint mouse;
	GetCursorPos(&mouse);
	ScreenToClient(&mouse);

	GLuint selectBuf[256];
	GLint hits;
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(256, selectBuf);

	glRenderMode(GL_SELECT);


	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();						// ���õ�ǰָ���ľ���Ϊ��λ����
		CRect rect;GetWindowRect(&rect);
		gluPickMatrix((GLdouble) mouse.x, (GLdouble) (viewport[3] - mouse.y),1.0, 1.0, viewport);
		gluPerspective( 45.0f, 	(GLfloat)rect.Width()/(GLfloat)rect.Height(), 0.1f,	 1000.0f);
		glMatrixMode(GL_MODELVIEW);
		
		CMeshDoc *pDoc = (CMeshDoc *)GetDocument();

		glClearColor(m_ClearColorRed,m_ClearColorGreen,m_ClearColorBlue,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glInitNames();
		glPushName(0);

		glPushMatrix();
			glTranslated(m_xTranslation,m_yTranslation,m_zTranslation);
			glRotatef(m_xRotation, 1.0, 0.0, 0.0);
			glRotatef(m_yRotation, 0.0, 1.0, 0.0);
			glRotatef(m_zRotation, 0.0, 0.0, 1.0);
			glScalef(m_xScaling,m_yScaling,m_zScaling);
			pDoc->RenderScene();
		glPopMatrix();


		hits = glRenderMode(GL_RENDER);

		
		::glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	::glMatrixMode(GL_MODELVIEW);


	int answer=processHits2(hits,selectBuf);
	if(answer>pDoc->o.f_num) return;

	pDoc->o.selectedShowFace=answer;
	pDoc->m_SceneGraph.RemoveAll();

	if(pDoc->o.DEBUGMODE==CHECKVISIBILITY)	pDoc->o.computeVIV(true);

	pDoc->o.loadMesh(&pDoc->m_SceneGraph);
	InvalidateRect(NULL,FALSE);

}

int CMeshView::processHits2 (GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

	ptr = (GLuint *) buffer;
	minZ = 0xffffffff;
	for (i = 0; i < hits; i++) {	
		names = *ptr;
		ptr++;
		if (*ptr < minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr+2;
		}
		ptr += names+2;
	}

	ptr = ptrNames;
	return *ptr;
}


void CMeshView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_LeftButtonDown = TRUE;
	m_LeftDownPos = point;
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}
void CMeshView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_RightButtonDown = FALSE;
	m_LeftButtonDown = FALSE;
	m_MiddleButtonDown = FALSE;
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}
void CMeshView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	m_MiddleButtonDown = TRUE;
	m_MiddleDownPos = point;
	SetCapture();
	CView::OnMButtonDown(nFlags,point);
}
void CMeshView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	m_RightButtonDown = FALSE;
	m_LeftButtonDown = FALSE;
	m_MiddleButtonDown = FALSE;
	ReleaseCapture();
	CView::OnMButtonUp(nFlags,point);
	pickIt();
}

void CMeshView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_RightButtonDown = TRUE;
	m_RightDownPos = point;
	SetCapture();
	CView::OnRButtonDown(nFlags, point);
}

void CMeshView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_RightButtonDown = FALSE;
	m_LeftButtonDown = FALSE;
	m_MiddleButtonDown = FALSE;
	ReleaseCapture();
	CView::OnRButtonUp(nFlags, point);
}

void CMeshView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// Both : rotation
	if(m_LeftButtonDown && m_RightButtonDown)
	{
		if(m_xyRotation)
		{
			m_yRotation -= (float)(m_LeftDownPos.x - point.x) * m_SpeedRotation;
			m_xRotation -= (float)(m_LeftDownPos.y - point.y) * m_SpeedRotation;
		}
		else
		{
			m_zRotation -= (float)(m_LeftDownPos.x - point.x) * m_SpeedRotation;
			m_xRotation -= (float)(m_LeftDownPos.y - point.y) * m_SpeedRotation;
		}
		m_LeftDownPos = point;
		m_RightDownPos = point;
		InvalidateRect(NULL,FALSE);
	}

	else

	// Left : x / y translation
	if(m_LeftButtonDown)
	{
		m_xTranslation -= (float)(m_LeftDownPos.x - point.x) * m_SpeedTranslation;
		m_yTranslation += (float)(m_LeftDownPos.y - point.y) * m_SpeedTranslation;
		m_LeftDownPos = point;
		InvalidateRect(NULL,FALSE);
	}

	else

	// Right : z translation
	if(m_RightButtonDown)
	{
		m_zTranslation += (float)(m_RightDownPos.y - point.y) * m_SpeedTranslation;
		m_RightDownPos = point;
		InvalidateRect(NULL,FALSE);
	}

	/*
	TRACE("\nPosition\n");
	TRACE("Translation : %g %g %g\n",m_xTranslation,m_yTranslation,m_zTranslation);
	TRACE("Rotation    : %g %g %g\n",m_xRotation,m_yRotation,m_zRotation);
	*/
	
	CView::OnMouseMove(nFlags, point);
}

BOOL CMeshView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	
	return CView::OnEraseBkgnd(pDC);
}

void CMeshView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC,m_hGLContext);
	::ReleaseDC(hWnd,hDC);
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//*********************************
// OnEditCopy 
//*********************************
void CMeshView::OnEditCopy() 
{
  // Clean clipboard of contents, and copy the DIB.
  if(OpenClipboard())
   {

    BeginWaitCursor();

		// Snap
		CSize size;
		unsigned char *pixel = SnapClient(&size);

		// Image
		CTexture image;

		// Link image - buffer
		VERIFY(image.ReadBuffer(pixel,size.cx,size.cy,24));

		// Cleanup memory
		delete [] pixel;

    EmptyClipboard();

    SetClipboardData(CF_DIB,image.ExportHandle());
    CloseClipboard();
    EndWaitCursor();
   }
}

// Hand-made client snapping
unsigned char *CMeshView::SnapClient(CSize *pSize)
{
	BeginWaitCursor();

	// Client zone
	CRect rect;
	GetClientRect(&rect);
	//CSize size(CTexture::LowerPowerOfTwo(rect.Width()),rect.Height());
	CSize size(rect.Width(),rect.Height());
	*pSize = size;

	ASSERT(size.cx > 0);
	ASSERT(size.cy > 0);

	// Alloc
	unsigned char *pixel = new unsigned char[3*size.cx*size.cy];
	ASSERT(pixel != NULL);
	
	// Capture frame buffer
	TRACE("Start reading client...\n");
	TRACE("Client : (%d,%d)\n",size.cx,size.cy);

	CRect ClientRect,MainRect;
	this->GetWindowRect(&ClientRect);
	CWnd *pMain = AfxGetApp()->m_pMainWnd;
	CWindowDC dc(pMain);
	pMain->GetWindowRect(&MainRect);
	int xOffset = ClientRect.left - MainRect.left;
	int yOffset = ClientRect.top - MainRect.top;
	for(int j=0;j<size.cy;j++)
		for(int i=0;i<size.cx;i++)
		{
			COLORREF color = dc.GetPixel(i+xOffset,j+yOffset);
			pixel[3*(size.cx*(size.cy-1-j)+i)] = (BYTE)GetBValue(color);
			pixel[3*(size.cx*(size.cy-1-j)+i)+1] = (BYTE)GetGValue(color);
			pixel[3*(size.cx*(size.cy-1-j)+i)+2] = (BYTE)GetRValue(color);
		}
	EndWaitCursor();
	return pixel;
}

//*********************************
// OnExportEps 
//*********************************
void CMeshView::OnExportEps() 
{
	static char BASED_CODE filter[] = "EPS Files (*.eps)|*.eps";
	CFileDialog SaveDlg(FALSE,"*.eps","mesh.eps",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filter);
	if(SaveDlg.DoModal() == IDOK)
	{
		CString string = SaveDlg.GetPathName();
		char *pFilename = string.GetBuffer(MAX_PATH);

		// Alloc
		const int size = (int)6e6; // dirty code
		GLfloat *pFeedbackBuffer = new GLfloat[size];
		ASSERT(pFeedbackBuffer);

		CDC *pDC = GetDC();

		// Useful in multidoc templates
		::wglMakeCurrent(pDC->m_hDC,m_hGLContext);

		// Set feedback mode
	  ::glFeedbackBuffer(size,GL_3D_COLOR,pFeedbackBuffer);
		::glRenderMode(GL_FEEDBACK);

		// Render
		::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Position / translation / scale
		::glPushMatrix();

			::glTranslated(m_xTranslation,m_yTranslation,m_zTranslation);
			::glRotatef(m_xRotation, 1.0, 0.0, 0.0);
			::glRotatef(m_yRotation, 0.0, 1.0, 0.0);
			::glRotatef(m_zRotation, 0.0, 0.0, 1.0);
			::glScalef(m_xScaling,m_yScaling,m_zScaling);

			// Start rendering
			CMeshDoc *pDoc = GetDocument();

			// Std rendering (no superimposed lines anyway)
			pDoc->m_SceneGraph.glDrawDirect(pDoc->o.DEBUGMODE);

		::glPopMatrix();

		// Double buffer
		SwapBuffers(pDC->m_hDC);

		int NbValues = glRenderMode(GL_RENDER);

		// The stuff here
		CPsRenderer PsRenderer;
		PsRenderer.Run(pFilename,pFeedbackBuffer,NbValues,TRUE);

		// Cleanup
		string.ReleaseBuffer();
		delete [] pFeedbackBuffer;
		ReleaseDC(pDC);
	}
}
void CMeshView::OnUpdateExportEps(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->m_SceneGraph.NbObject() > 0);
}

//*********************************
// OnOpenglClearcolor 
//*********************************
void CMeshView::OnOpenglClearcolor() 
{
	COLORREF color = RGB((unsigned char)(m_ClearColorRed*255.0f),
		                   (unsigned char)(m_ClearColorGreen*255.0f),
											 (unsigned char)(m_ClearColorBlue*255.0f));
	CColorDialog dlg(color);
	if(dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();
		m_ClearColorRed   = (float)GetRValue(color)/255.0f;
		m_ClearColorGreen = (float)GetGValue(color)/255.0f;
		m_ClearColorBlue  = (float)GetBValue(color)/255.0f;
		InvalidateRect(NULL,FALSE);
	}
}
