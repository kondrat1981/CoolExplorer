#if !defined(AFX_DELDLG_H__7FF9D165_BCD6_4AE5_AC00_261432A8E117__INCLUDED_)
#define AFX_DELDLG_H__7FF9D165_BCD6_4AE5_AC00_261432A8E117__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDelDlg dialog
#include "FilesTree.h"
#include "CopyDlg.h"

class CDelDlg : public CNewProgressDlg
{
// Construction
public:
//	static void DelFun(DelInfo *pDelInfo);
	CDelDlg(CFtpConnection *pFtpConnection, LPCTSTR sPath, CStringList *plFiles, CWnd* pParent = NULL, CWnd* pRefresh = NULL);   // standard constructor
	void Create();
	BOOL IsSharedFtpCon(CFtpConnection * pFtpCon);

// Dialog Data
	//{{AFX_DATA(CDelDlg)
	enum { IDD = IDD_PROGRESS_NEW };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void Cancel();
	virtual BOOL ThreadFun();
	virtual BOOL InitWnd();
	CFtpConnection *m_pFtpConnection;
//	static void ListingProgress(CopyInfo *pCopyInfo);
//	DelInfo *m_pDelInfo;
	
//	void Enum(CDirTree *pDir, LPCTSTR sSrc);
	// Generated message map functions
	//{{AFX_MSG(CDelDlg)
	virtual void OnCancel();
	afx_msg void OnDelCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELDLG_H__7FF9D165_BCD6_4AE5_AC00_261432A8E117__INCLUDED_)
