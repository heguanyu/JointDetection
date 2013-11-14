# Microsoft Developer Studio Project File - Name="Mesh" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Mesh - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Mesh.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Mesh.mak" CFG="Mesh - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Mesh - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Mesh - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Mesh - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib glu32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Mesh - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40c /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Mesh - Win32 Release"
# Name "Mesh - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Lib3D\Array3d.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Color.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\ColorRamp.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogWmf.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Edge3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Face3d.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Material.cpp
# End Source File
# Begin Source File

SOURCE=.\Mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\Mesh.rc
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Mesh3d.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshView.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Object3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\ParserVrml.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\PsRender.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\SceneGraph3d.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Transform.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Utils3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Vector3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Vertex3d.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Lib3D\Array3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\AVLInd.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Base3d.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Color.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\ColorRamp.h
# End Source File
# Begin Source File

SOURCE=.\DialogWmf.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Edge3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Face3d.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Material.h
# End Source File
# Begin Source File

SOURCE=.\Mesh.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Mesh3d.h
# End Source File
# Begin Source File

SOURCE=.\MeshDoc.h
# End Source File
# Begin Source File

SOURCE=.\MeshView.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Object3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\ParserVrml.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\PsRender.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\SceneGraph3d.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Texture.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Transform.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Utils3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Vector3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Vertex3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\WmfTools.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Mesh.ico
# End Source File
# Begin Source File

SOURCE=.\res\Mesh.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MeshDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
