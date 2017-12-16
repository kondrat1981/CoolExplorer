# Microsoft Developer Studio Project File - Name="CoolExplorer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CoolExplorer - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CoolExplorer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CoolExplorer.mak" CFG="CoolExplorer - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CoolExplorer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CoolExplorer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "CoolExplorer - Win32 Debug Unicode" (based on "Win32 (x86) Application")
!MESSAGE "CoolExplorer - Win32 Release Unicode" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CoolExplorer - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x415 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x415 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Netapi32.lib Wininet.lib Mpr.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "CoolExplorer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x415 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x415 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Wininet.lib Mpr.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "CoolExplorer - Win32 Debug Unicode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CoolExplorer___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "CoolExplorer___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "CoolExplorer___Win32_Debug_Unicode"
# PROP Intermediate_Dir "CoolExplorer___Win32_Debug_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../CoolViewer" /I "../devil/sdk/include/IL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x415 /d "_DEBUG"
# ADD RSC /l 0x415 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Wininet.lib Netapi32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Netapi32.lib Wininet.lib Mpr.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /profile /debug /machine:I386 /libpath:"../d_lib" /libpath:"../DevIL/sdk/lib/"
# SUBTRACT LINK32 /map
# Begin Special Build Tool
OutDir=.\CoolExplorer___Win32_Debug_Unicode
WkspDir=.
ProjDir=.
TargetName=CoolExplorer
SOURCE="$(InputPath)"
PostBuild_Cmds=if                                 not                                 exist                                 $(WkspDir)\u_bin\                                 mkdir                                 $(WkspDir)\u_bin\                              	if                                 not                                 exist                                 $(WkspDir)\u_bin\lang                                 mkdir                                 $(WkspDir)\u_bin\lang                                 	copy                                 $(WkspDir)\lang                                 $(WkspDir)\u_bin\lang                                 	copy                                  $(OutDir)\$(TargetName).exe                                 $(WkspDir)\u_bin\                                  	copy                                  $(ProjDir)\license.txt                                 $(WkspDir)\u_bin\ 
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CoolExplorer - Win32 Release Unicode"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CoolExplorer___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "CoolExplorer___Win32_Release_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "CoolExplorer___Win32_Release_Unicode"
# PROP Intermediate_Dir "CoolExplorer___Win32_Release_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "../CoolViewer" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x415 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x415 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Wininet.lib Netapi32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 Netapi32.lib Wininet.lib Mpr.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /libpath:"../r_lib"
# Begin Special Build Tool
OutDir=.\CoolExplorer___Win32_Release_Unicode
WkspDir=.
ProjDir=.
TargetName=CoolExplorer
SOURCE="$(InputPath)"
PostBuild_Cmds=if                                 not                                 exist                                 $(WkspDir)\u_rbin                                  mkdir                                 $(WkspDir)\u_rbin                                 	if                                 not                                 exist                                 $(WkspDir)\u_rbin\lang                                 mkdir                                 $(WkspDir)\u_rbin\lang                                 	copy                                 $(WkspDir)\lang                                 $(WkspDir)\u_rbin\lang                                 	copy                                          $(OutDir)\$(TargetName).exe                                          $(WkspDir)\u_rbin\                                  	copy                                          $(ProjDir)\license.txt                                           $(WkspDir)\u_rbin\ 
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "CoolExplorer - Win32 Release"
# Name "CoolExplorer - Win32 Debug"
# Name "CoolExplorer - Win32 Debug Unicode"
# Name "CoolExplorer - Win32 Release Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ARACrypt.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeAttribDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeVolumeLabelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\ComboHist.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonFun.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonStruct.cpp
# End Source File
# Begin Source File

SOURCE=.\CompFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectFtpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolExplorer.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolExplorer.rc
# End Source File
# Begin Source File

SOURCE=.\CoolExplorerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolExplorerFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolExplorerView.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\CopyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateBatchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DirMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\DropSource.cpp
# End Source File
# Begin Source File

SOURCE=.\DropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrFile.cpp
# End Source File
# Begin Source File

SOURCE=.\FastFileSrchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\FilesListBase.cpp
# End Source File
# Begin Source File

SOURCE=.\FilesListCOM.cpp
# End Source File
# Begin Source File

SOURCE=.\FilesListFtp.cpp
# End Source File
# Begin Source File

SOURCE=.\FilesListManager.cpp
# End Source File
# Begin Source File

SOURCE=.\FilesListNet.cpp
# End Source File
# Begin Source File

SOURCE=.\FilesListNormal.cpp
# End Source File
# Begin Source File

SOURCE=.\FilesListObservable.cpp
# End Source File
# Begin Source File

SOURCE=.\FilesListObserverInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\FilesListVirtual.cpp
# End Source File
# Begin Source File

SOURCE=.\FilesTree.cpp
# End Source File
# Begin Source File

SOURCE=.\FlatButton.cpp
# End Source File
# Begin Source File

SOURCE=.\FtpConSetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FtpCopyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FtpStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\GridEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\HistEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemsStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemsStorageShell.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemsStorageVirt.cpp
# End Source File
# Begin Source File

SOURCE=.\LangSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LangSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\Language.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginFtpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainAdvSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainBasicSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainNetSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainSearchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkMaskDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\MASKED.CPP
# End Source File
# Begin Source File

SOURCE=.\MoreDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MyDriveBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFtpFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\PanSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\PathBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PathEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\PathHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\PerformanceCounter.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressSimpleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\QuestionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\QueueDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetFiltersDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Settings.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingsMainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Shell.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellIcons.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SubSettings_Display.cpp
# End Source File
# Begin Source File

SOURCE=.\SubSettings_EditView.cpp
# End Source File
# Begin Source File

SOURCE=.\SubSettings_FontsColors.cpp
# End Source File
# Begin Source File

SOURCE=.\SubSettings_Language.cpp
# End Source File
# Begin Source File

SOURCE=.\SubSettings_Operations.cpp
# End Source File
# Begin Source File

SOURCE=.\SubSettings_Search.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TMyGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\TMyGridColumn.cpp
# End Source File
# Begin Source File

SOURCE=.\TMyGridColumns.cpp
# End Source File
# Begin Source File

SOURCE=.\TMyGridDrawer.cpp
# End Source File
# Begin Source File

SOURCE=.\TMyGridRow.cpp
# End Source File
# Begin Source File

SOURCE=.\TMyGridRows.cpp
# End Source File
# Begin Source File

SOURCE=.\TMyGridSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\TMyGridTabStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\TooltipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TooltipDlgBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TryIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\TViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\UnicodeFile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ARACrypt.h
# End Source File
# Begin Source File

SOURCE=.\ChangeAttribDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChangeVolumeLabelDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColorPicker.h
# End Source File
# Begin Source File

SOURCE=.\ComboHist.h
# End Source File
# Begin Source File

SOURCE=.\CommandBar.h
# End Source File
# Begin Source File

SOURCE=.\CommonDef.h
# End Source File
# Begin Source File

SOURCE=.\CommonFun.h
# End Source File
# Begin Source File

SOURCE=.\CommonStruct.h
# End Source File
# Begin Source File

SOURCE=.\ConnectFtpDlg.h
# End Source File
# Begin Source File

SOURCE=.\CoolExplorer.h
# End Source File
# Begin Source File

SOURCE=.\CoolExplorerDoc.h
# End Source File
# Begin Source File

SOURCE=.\CoolExplorerFrame.h
# End Source File
# Begin Source File

SOURCE=.\CoolExplorerView.h
# End Source File
# Begin Source File

SOURCE=.\CopyBuffer.h
# End Source File
# Begin Source File

SOURCE=.\CopyDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreateBatchDlg.h
# End Source File
# Begin Source File

SOURCE=.\DelDlg.h
# End Source File
# Begin Source File

SOURCE=.\DirMonitor.h
# End Source File
# Begin Source File

SOURCE=.\DriveButton.h
# End Source File
# Begin Source File

SOURCE=.\DriveCombo.h
# End Source File
# Begin Source File

SOURCE=.\DropSource.h
# End Source File
# Begin Source File

SOURCE=.\DropTarget.h
# End Source File
# Begin Source File

SOURCE=.\ErrFile.h
# End Source File
# Begin Source File

SOURCE=.\FastFileSrchDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileFinder.h
# End Source File
# Begin Source File

SOURCE=.\FilesListBase.h
# End Source File
# Begin Source File

SOURCE=.\FilesListCOM.h
# End Source File
# Begin Source File

SOURCE=.\FilesListFtp.h
# End Source File
# Begin Source File

SOURCE=.\FilesListManager.h
# End Source File
# Begin Source File

SOURCE=.\FilesListNet.h
# End Source File
# Begin Source File

SOURCE=.\FilesListNormal.h
# End Source File
# Begin Source File

SOURCE=.\FilesListObjectInterface.h
# End Source File
# Begin Source File

SOURCE=.\FilesListObservable.h
# End Source File
# Begin Source File

SOURCE=.\FilesListObserverInterface.h
# End Source File
# Begin Source File

SOURCE=.\FilesListVirtual.h
# End Source File
# Begin Source File

SOURCE=.\FilesTree.h
# End Source File
# Begin Source File

SOURCE=.\FirstRunCfgDlg.h
# End Source File
# Begin Source File

SOURCE=.\FlatButton.h
# End Source File
# Begin Source File

SOURCE=.\FtpConSetDlg.h
# End Source File
# Begin Source File

SOURCE=.\FtpCopyDlg.h
# End Source File
# Begin Source File

SOURCE=.\FtpStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\GridEdit.h
# End Source File
# Begin Source File

SOURCE=.\HistEdit.h
# End Source File
# Begin Source File

SOURCE=.\ItemsStorage.h
# End Source File
# Begin Source File

SOURCE=.\ItemsStorageShell.h
# End Source File
# Begin Source File

SOURCE=.\ItemsStorageVirt.h
# End Source File
# Begin Source File

SOURCE=.\Language.h
# End Source File
# Begin Source File

SOURCE=.\ListData.h
# End Source File
# Begin Source File

SOURCE=.\ListDataStorage.h
# End Source File
# Begin Source File

SOURCE=.\LoginFtpDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainAdvSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainBasicSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MainNetSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainSearchDlg.h
# End Source File
# Begin Source File

SOURCE=.\MarkMaskDlg.h
# End Source File
# Begin Source File

SOURCE=.\Markup.h
# End Source File
# Begin Source File

SOURCE=.\MASKED.H
# End Source File
# Begin Source File

SOURCE=.\MoreDialog.h
# End Source File
# Begin Source File

SOURCE=.\MyDriveBar.h
# End Source File
# Begin Source File

SOURCE=.\MyFtpFile.h
# End Source File
# Begin Source File

SOURCE=.\MyTimer.h
# End Source File
# Begin Source File

SOURCE=.\PanSplitter.h
# End Source File
# Begin Source File

SOURCE=.\PathBar.h
# End Source File
# Begin Source File

SOURCE=.\PathEdit.h
# End Source File
# Begin Source File

SOURCE=.\PathHistory.h
# End Source File
# Begin Source File

SOURCE=.\PerformanceCounter.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressSimpleDlg.h
# End Source File
# Begin Source File

SOURCE=.\QuestionDlg.h
# End Source File
# Begin Source File

SOURCE=.\QueueDlg.h
# End Source File
# Begin Source File

SOURCE=.\ResizeDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetFiltersDlg.h
# End Source File
# Begin Source File

SOURCE=.\Settings.h
# End Source File
# Begin Source File

SOURCE=.\SettingsMainDlg.h
# End Source File
# Begin Source File

SOURCE=.\Shell.h
# End Source File
# Begin Source File

SOURCE=.\ShellIcons.h
# End Source File
# Begin Source File

SOURCE=.\SimpleEdit.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SubSettings_Display.h
# End Source File
# Begin Source File

SOURCE=.\SubSettings_EditView.h
# End Source File
# Begin Source File

SOURCE=.\SubSettings_FontsColors.h
# End Source File
# Begin Source File

SOURCE=.\SubSettings_Language.h
# End Source File
# Begin Source File

SOURCE=.\SubSettings_Operations.h
# End Source File
# Begin Source File

SOURCE=.\SubSettings_Search.h
# End Source File
# Begin Source File

SOURCE=.\ThreadManager.h
# End Source File
# Begin Source File

SOURCE=.\TMyGrid.h
# End Source File
# Begin Source File

SOURCE=.\TMyGridColumn.h
# End Source File
# Begin Source File

SOURCE=.\TMyGridColumns.h
# End Source File
# Begin Source File

SOURCE=.\TMyGridDrawer.h
# End Source File
# Begin Source File

SOURCE=.\TMyGridEnums.h
# End Source File
# Begin Source File

SOURCE=.\TMyGridGlobalTypes.h
# End Source File
# Begin Source File

SOURCE=.\TMyGridRow.h
# End Source File
# Begin Source File

SOURCE=.\TMyGridRows.h
# End Source File
# Begin Source File

SOURCE=.\TMyGridSettings.h
# End Source File
# Begin Source File

SOURCE=.\TMyGridTabStorage.h
# End Source File
# Begin Source File

SOURCE=.\TooltipDlg.h
# End Source File
# Begin Source File

SOURCE=.\TooltipDlgBar.h
# End Source File
# Begin Source File

SOURCE=.\TryIcon.h
# End Source File
# Begin Source File

SOURCE=.\TViewer.h
# End Source File
# Begin Source File

SOURCE=.\UnicodeFile.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CoolExplorer.ico
# End Source File
# Begin Source File

SOURCE=.\res\CoolExplorer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\CoolExplorerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\deleted.ico
# End Source File
# Begin Source File

SOURCE=.\res\drive_bar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Header.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\left_right.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pane_bar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\return.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar16_a.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar24_a.bmp
# End Source File
# Begin Source File

SOURCE=".\res\toolbar24b-16p.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\toolbar24b-32p.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar32.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar32_a.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\manifest.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
