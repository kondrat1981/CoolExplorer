// CoolExplorer.h : main header file for the COOLEXPLORER application
//

#if !defined(AFX_COOLEXPLORER_H__1C687CD5_8DD3_4285_B5D0_804AF9377DBB__INCLUDED_)
#define AFX_COOLEXPLORER_H__1C687CD5_8DD3_4285_B5D0_804AF9377DBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Lm.h"
#include "CoolExplorerFrame.h"
class CSettings;

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerApp:
// See CoolExplorer.cpp for the implementation of this class
//

// function pointers / to load dynamic
typedef NET_API_STATUS  (NET_API_FUNCTION * NetServerEnum_Fun)(
  LPWSTR servername,     
  DWORD level,           
  LPBYTE *bufptr,        
  DWORD prefmaxlen,      
  LPDWORD entriesread,   
  LPDWORD totalentries,  
  DWORD servertype,      
  LPWSTR domain,         
  LPDWORD resume_handle  
);

typedef NET_API_STATUS (NET_API_FUNCTION * NetServerDiskEnum_Fun)(
  LPWSTR servername,     
  DWORD level,           
  LPBYTE *bufptr,        
  DWORD prefmaxlen,      
  LPDWORD entriesread,   
  LPDWORD totalentries,  
  LPDWORD resume_handle  
);

typedef NET_API_STATUS (NET_API_FUNCTION * NetApiBufferFree_Fun)(
  LPVOID Buffer  
);

typedef NET_API_STATUS (NET_API_FUNCTION * NetShareEnum_Fun)(
  LPCTSTR servername,     
  DWORD level,           
  LPBYTE * bufptr,      
  DWORD prefmaxlen,      
  LPDWORD entriesread,   
  LPDWORD totalentries,  
  LPDWORD resume_handle 
);

//


class CCoolExplorerApp : public CWinApp
{
public:
	CCoolExplorerApp();
	~CCoolExplorerApp();
	int ActivePanel;

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolExplorerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCoolExplorerApp)

	afx_msg void OnAppHomePage();
	afx_msg void OnAppOnlineDocumentation();
	afx_msg void OnAppCheckForNewRelease();
	afx_msg void OnAppReportProblem();
	afx_msg void OnAppAbout();
	afx_msg void OnAppConfiguration();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void ReplaceStrings();
	void ReloadMainMenuItems();
	CSettings *m_configuration;
	eWindowsVersion m_winVer;
	
	BOOL m_bFullFunct;
	HINSTANCE m_hLibNetApi32;

	NetServerEnum_Fun m_ptr_NetServerEnum_Fun;
	NetServerDiskEnum_Fun m_ptr_NetServerDiskEnum_Fun;
	NetShareEnum_Fun m_ptr_NetShareEnum_Fun;
	NetApiBufferFree_Fun m_ptr_NetApiBufferFree_Fun;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLEXPLORER_H__1C687CD5_8DD3_4285_B5D0_804AF9377DBB__INCLUDED_)
