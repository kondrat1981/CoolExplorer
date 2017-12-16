// CoolViewer.h : main header file for the COOLVIEWER application
//

#if !defined(AFX_COOLVIEWER_H__0A650C30_1435_446C_94F6_7022009C53DD__INCLUDED_)
#define AFX_COOLVIEWER_H__0A650C30_1435_446C_94F6_7022009C53DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCoolViewerApp:
// See CoolViewer.cpp for the implementation of this class
//

class CCoolViewerApp : public CWinApp
{
public:
	CCoolViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolViewerApp)
public:
	CString m_filename;
	
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCoolViewerApp)
	afx_msg void OnAppAbout();
	afx_msg void OnEscape();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLVIEWER_H__0A650C30_1435_446C_94F6_7022009C53DD__INCLUDED_)
