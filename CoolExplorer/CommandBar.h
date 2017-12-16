#if !defined(AFX_COMMANDBAR_H__DF3A77E2_3EA9_4684_8CAE_9CC5F5404B18__INCLUDED_)
#define AFX_COMMANDBAR_H__DF3A77E2_3EA9_4684_8CAE_9CC5F5404B18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HistEdit.h"
#include "FilesListObserverInterface.h"
// CommandBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommandBar dialog


class CCommandBar : public CDialogBar, public CFilesListObserverInterface
{
// Construction
public:
	CCommandBar(CWnd* pParent = NULL);   // standard constructor
	BOOL DoCommand(CString &str);
	BOOL AppendText(const CString &strText, BOOL bFocus = FALSE);
	BOOL EraseText();
	BOOL IsEmptyCommandText();
	CString GetCommandText();
	virtual void Update(CFilesListObservable *pObject, int nDataVersion, BOOL Result);

// Dialog Data
	//{{AFX_DATA(CCommandBar)
	enum { IDD = CG_IDD_COMMANDBAR };
	CEdit	m_command;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommandBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void PathChanged(LPCTSTR sPath);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetWidth(int nWidth);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommandBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnEventUpdate(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
private:
	CFilesListObservable *m_pFilesListObject;		
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMANDBAR_H__DF3A77E2_3EA9_4684_8CAE_9CC5F5404B18__INCLUDED_)
