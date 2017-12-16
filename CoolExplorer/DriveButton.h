#if !defined(AFX_DRIVEBUTTON_H__363473C2_3378_4F87_A4CE_5F7F431554AC__INCLUDED_)
#define AFX_DRIVEBUTTON_H__363473C2_3378_4F87_A4CE_5F7F431554AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DriveButton.h : header file
//
#include "FlatButton.h"
/////////////////////////////////////////////////////////////////////////////
// CDriveButton window

class CDriveButton : public CFlatButton
{
// Construction
public:
	CDriveButton();
	DECLARE_DYNCREATE(CDriveButton);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDriveButton)
	public:
	virtual BOOL Create(LPCTSTR lpszWindowName, LPCTSTR lpszDriveName,DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetDriveName();
	virtual ~CDriveButton();

	// Generated message map functions
protected:
	CString m_sDriveName;
	HICON m_hIcon;
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	//{{AFX_MSG(CDriveButton)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
//	CString m_sDriveName;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRIVEBUTTON_H__363473C2_3378_4F87_A4CE_5F7F431554AC__INCLUDED_)
