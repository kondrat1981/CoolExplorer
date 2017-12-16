#if !defined(AFX_FTPCOPYDLG_H__0810A8BF_894D_464E_9B3B_F327F2D99853__INCLUDED_)
#define AFX_FTPCOPYDLG_H__0810A8BF_894D_464E_9B3B_F327F2D99853__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FtpCopyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFtpCopyDlg dialog
#include "FilesTree.h"
#include "CopyDlg.h"


class CFtpCopyDlg : public CNewProgressDlg
{
// Construction
public:
	BOOL IsSharedFtpCon(CFtpConnection * pFtpCon);
	CFtpCopyDlg(CFtpConnection *pSourceFtpConection, CFtpConnection *pTargetFtpConection, BOOL bFreeSourceFtp, BOOL bFreeTargetFtp, LPCTSTR sPath,LPCTSTR sPathTo, CStringList *plFiles,CWnd* pParent,CWnd* pSecWnd, FtpProgressInfo *pFtpProgressInfo);   // standard constructor
	~CFtpCopyDlg();

protected:
	virtual void Cancel();
	virtual BOOL ThreadFun();
	virtual BOOL InitWnd();
	//CArray<CWnd *, CWnd *> m_aRefresh;
	//CopyInfo m_info;

	//ULONGLONG m_nSize;
	CFtpConnection *	m_pSourceFtpConection;
	CFtpConnection *	m_pTargetFtpConection;
	BOOL m_bFreeSourceFtp;
	BOOL m_bFreeTargetFtp;
	//CString m_sPath;
	//CStringList m_lstrFiles;
	//CString m_sMask;
	//CDirTree m_DirTree;
	//CString m_sPathTo;
	//BOOL m_bInProgress;
	//int m_nWsp;
	FtpProgressInfo *m_pFtpProgressInfo;

	

// Dialog Data
	//{{AFX_DATA(CFtpCopyDlg)
	enum { IDD = IDD_FTP_COPY };
	//CProgressCtrl	m_ProgressOne;
	//CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpCopyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFtpCopyDlg)
	virtual BOOL OnInitDialog();
	//afx_msg LRESULT OnRefresh(WPARAM ntype, LPARAM sFileName);
	afx_msg void OnCancel();
	afx_msg void OnFtpcopyCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPCOPYDLG_H__0810A8BF_894D_464E_9B3B_F327F2D99853__INCLUDED_)
