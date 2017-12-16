#if !defined(AFX_FTPSTATUSBAR_H__112288__INCLUDED_)
#define AFX_FTPSTATUSBAR_H__112288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FtpStatusBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFtpStatusBar dialog

class CFtpStatusBar : public CDialogBar
{
// Construction
public:
	DECLARE_DYNCREATE(CFtpStatusBar);
	
	CFtpStatusBar(CWnd* pParent = NULL);   // standard constructor
	void AddText(DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);

// Dialog Data
	//{{AFX_DATA(CFtpStatusBar)
	enum { IDD = CG_IDD_FTPSTATUSBAR };
	CString	m_sHis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpStatusBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD m_dwBytesReceived;
	DWORD m_dwBytesSent;
	

	// Generated message map functions
	//{{AFX_MSG(CFtpStatusBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPSTATUSBAR_H__112288__INCLUDED_)
