#if !defined(AFX_CHANGEATTRIBDLG_H__A1B20458_F896_496A_B477_BECCB9B3BA50__INCLUDED_)
#define AFX_CHANGEATTRIBDLG_H__A1B20458_F896_496A_B477_BECCB9B3BA50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangeAttribDlg.h : header file
//
//#include <afxdtctl.h>
#include "StdAfx.h"	// Added by ClassView
#include "MaskEd.h"	// Added by ClassView
#include "MoreDialog.h"


/////////////////////////////////////////////////////////////////////////////
// CChangeAttribDlg dialog

class CChangeAttribDlg : public CMoreDialog
{
// Construction
public:
	CChangeAttribDlg(LPCTSTR sPath, CStringList *pFiles, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangeAttribDlg)
	enum { IDD = IDD_CHANGE_ATTRIB };
	CDateEdit m_ModyficationDate;
	CDateEdit m_CreationDate;
	CDateEdit m_LastAccessDate;
	CTimeEdit m_CreationTime;
	CTimeEdit m_LastAccessTime;
	CTimeEdit m_ModyficationTime;
	CString	m_sMask;
	//}}AFX_DATA
	/*	int		m_nHidden;
	int		m_nArchive;
	int		m_nSystem;
	int		m_nReadOnly;
	BOOL	m_bModyfication;
	BOOL	m_bLastAccess;
	BOOL	m_bCreation;
*/


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangeAttribDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
//	BOOL CreateDateTimeCtrl(CDateTimeCtrl *Ctrl, UINT nID, BOOL bDate);

	// Generated message map functions
	//{{AFX_MSG(CChangeAttribDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnModyficationCheck();
	afx_msg void OnCreationCheck();
	afx_msg void OnLastmodyficationCheck();
	virtual void OnOK();
//	afx_msg void OnAdvanced();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	ChangeStatusInfo m_Info;
	CStringList m_lFiles;
	CString m_sPath;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEATTRIBDLG_H__A1B20458_F896_496A_B477_BECCB9B3BA50__INCLUDED_)
