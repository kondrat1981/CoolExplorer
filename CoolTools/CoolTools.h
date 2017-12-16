// CoolTools.h : main header file for the COOLTOOLS DLL
//

#if !defined(AFX_COOLTOOLS_H__80ABCC5E_B997_4974_9B2E_3415AC5FABC0__INCLUDED_)
#define AFX_COOLTOOLS_H__80ABCC5E_B997_4974_9B2E_3415AC5FABC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCoolToolsApp
// See CoolTools.cpp for the implementation of this class
//

class CCoolToolsApp : public CWinApp
{
public:
	CCoolToolsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolToolsApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCoolToolsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLTOOLS_H__80ABCC5E_B997_4974_9B2E_3415AC5FABC0__INCLUDED_)
