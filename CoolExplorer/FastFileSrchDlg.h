#if !defined(AFX_FASTFILESRCHDLG_H__1DC9BFF4_6FFB_4263_B8E5_6396A404647E__INCLUDED_)
#define AFX_FASTFILESRCHDLG_H__1DC9BFF4_6FFB_4263_B8E5_6396A404647E__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FastFileSrchDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFastFileSrchDlg dialog
class CCoolExplorerView;
#include "resource.h"
class CFastFileSrchDlg : public CDialog
{
// Construction
public:
	CFastFileSrchDlg(CWnd* pParent = NULL);   // standard constructor
	void SetPointers( CCoolExplorerView* pView );
	void PutLetter(TCHAR letter);

// Dialog Data
	//{{AFX_DATA(CFastFileSrchDlg)
	enum { IDD = IDD_FASTFILESRCH };
	CButton	m_FullMaskedCheckbox;
	CEdit	m_FileNameFind_Edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFastFileSrchDlg)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL

// Implementation
protected:
	CCoolExplorerView *m_pView;
    int m_found_items;
    int m_selected_item;

	// Generated message map functions
	//{{AFX_MSG(CFastFileSrchDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKillfocusFastfilesrchEdit();
	afx_msg void OnChangeFastfilesrchEdit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	void ChangeFastfilesrchEdit();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTFILESRCHDLG_H__1DC9BFF4_6FFB_4263_B8E5_6396A404647E__INCLUDED_)
