; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDialogWmf
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mesh.h"
LastPage=0

ClassCount=7
Class1=CChildFrame
Class2=CMainFrame
Class3=CMeshApp
Class4=CAboutDlg
Class5=CMeshDoc
Class6=CMeshView

ResourceCount=4
Resource1=IDD_ABOUTBOX (English (U.S.))
Resource2=IDR_MESHTYPE (English (U.S.))
Resource3=IDR_MAINFRAME (English (U.S.))
Class7=CDialogWmf
Resource4=IDD_DIALOG_WMF

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CMeshApp]
Type=0
BaseClass=CWinApp
HeaderFile=Mesh.h
ImplementationFile=Mesh.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Mesh.cpp
ImplementationFile=Mesh.cpp
LastObject=CAboutDlg

[CLS:CMeshDoc]
Type=0
BaseClass=CDocument
HeaderFile=MeshDoc.h
ImplementationFile=MeshDoc.cpp
LastObject=CMeshDoc
Filter=N
VirtualFilter=DC

[CLS:CMeshView]
Type=0
BaseClass=CView
HeaderFile=MeshView.h
ImplementationFile=MeshView.cpp
Filter=C
VirtualFilter=VWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_DIALOG_WMF]
Type=1
Class=CDialogWmf
ControlCount=13
Control1=ID_BUTTON_RUN,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COLOR_FACE,static,1342177288
Control4=IDC_COLOR_LINE,static,1342177288
Control5=65535,static,1342308352
Control6=65535,static,1342308352
Control7=65535,button,1342177287
Control8=IDC_RADIO_LINE,button,1342177289
Control9=IDC_RADIO_FACE,button,1342177289
Control10=65535,static,1342308352
Control11=IDC_EDIT_RATIO,edit,1350631552
Control12=65535,static,1342308352
Control13=IDC_EDIT_RATIO_NBFACES,edit,1350631552

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_PASTE
Command6=ID_FILE_PRINT
Command7=ID_APP_ABOUT
Command8=ID_EDIT_COPY
Command9=ID_EDIT_WMF
Command10=ID_EXPORT_EPS
Command11=ID_GL_VERTEX
Command12=ID_GL_LINE
Command13=ID_GL_FACE
Command14=ID_GL_ADD_WIREFRAME
Command15=ID_GL_SMOOTH
Command16=ID_GL_LIGHT
Command17=ID_MESH_COLOR_CHOOSE
Command18=ID_MESH_COLOR_HEIGHT
Command19=ID_OPENGL_CLEARCOLOR
Command20=ID_MESH_LOOP
CommandCount=20

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MESHTYPE (English (U.S.))]
Type=1
Class=CMeshDoc
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_GL_VERTEX
Command12=ID_GL_LINE
Command13=ID_GL_FACE
Command14=ID_GL_ADD_WIREFRAME
Command15=ID_GL_SMOOTH
Command16=ID_GL_LIGHT
Command17=ID_GL_CULLING
Command18=ID_OPENGL_CLEARCOLOR
Command19=ID_EDIT_WMF
Command20=ID_EXPORT_EPS
Command21=ID_EDIT_COPY
Command22=ID_MESH_LOOP
Command23=ID_MESH_COLOR_HEIGHT
Command24=ID_MESH_COLOR_CHOOSE
Command25=ID_VIEW_TOOLBAR
Command26=ID_VIEW_STATUS_BAR
Command27=ID_WINDOW_NEW
Command28=ID_WINDOW_CASCADE
Command29=ID_WINDOW_TILE_VERT
Command30=ID_WINDOW_ARRANGE
Command31=ID_APP_ABOUT
CommandCount=31

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_MESH_LOOP
Command3=ID_FILE_NEW
Command4=ID_FILE_OPEN
Command5=ID_FILE_PRINT
Command6=ID_FILE_SAVE
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_NEXT_PANE
Command11=ID_PREV_PANE
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_EDIT_CUT
Command15=ID_EDIT_UNDO
CommandCount=15

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:CDialogWmf]
Type=0
HeaderFile=DialogWmf.h
ImplementationFile=DialogWmf.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_RATIO_NBFACES
VirtualFilter=dWC

