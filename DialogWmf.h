#if !defined(AFX_DIALOGWMF_H__F12E1D00_9F9D_11D4_9DF3_A0888C751C53__INCLUDED_)
#define AFX_DIALOGWMF_H__F12E1D00_9F9D_11D4_9DF3_A0888C751C53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogWmf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWmf dialog

class CMeshDoc;

class CDialogWmf : public CDialog
{
// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CDialogWmf(CWnd* pParent = NULL);   // standard constructor
	CDialogWmf(CWnd* pParent,CMeshDoc *pDoc);

	CMeshDoc *m_pDoc;
	COLORREF m_ColorLine;
	COLORREF m_ColorFace;
	int m_Mode;

	int checkedItem;
	enum {MODE_LINE,MODE_FACE};

// Dialog Data
	//{{AFX_DATA(CDialogWmf)
	enum { IDD = IDD_DIALOG_WMF };
	CStatic	m_FrameFace;
	CStatic	m_FrameLine;
	double	m_Ratio;
	float	m_RatioNbFaces;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogWmf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;

	// Generated message map functions
	//{{AFX_MSG(CDialogWmf)
	afx_msg void OnButtonRun();
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRadioFace();
	afx_msg void OnRadioLine();


	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGWMF_H__F12E1D00_9F9D_11D4_9DF3_A0888C751C53__INCLUDED_)
