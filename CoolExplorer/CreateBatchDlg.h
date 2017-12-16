#if !defined(AFX_CREATEBATCHDLG_H__414C982E_DC57_4B47_AED0_51C60CE614A8__INCLUDED_)
#define AFX_CREATEBATCHDLG_H__414C982E_DC57_4B47_AED0_51C60CE614A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateBatchDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreateBatchDlg dialog

class CCreateBatchDlg : public CTooltipDlg
{
// Construction
public:
	CCreateBatchDlg(LPCTSTR sFromDir = NULL, CWnd* pParent = NULL);   // standard constructor

protected:

// Dialog Data
	//{{AFX_DATA(CCreateBatchDlg)
	enum { IDD = IDD_SMART_COPY };
	CString	m_sComAndDir;
	CString	m_sBatchFile;
	BOOL	m_bRunCommand;
	CString	m_sDestFile;
	CString	m_sDir;
	CString	m_sMask;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateBatchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_sTooltip;

	// Generated message map functions
	//{{AFX_MSG(CCreateBatchDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCreateBatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEBATCHDLG_H__414C982E_DC57_4B47_AED0_51C60CE614A8__INCLUDED_)
