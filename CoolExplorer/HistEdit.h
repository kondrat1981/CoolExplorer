#if !defined(AFX_HISTEDIT_H__2297E15D_F1D5_4940_A57D_27C9F496468E__INCLUDED_)
#define AFX_HISTEDIT_H__2297E15D_F1D5_4940_A57D_27C9F496468E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistEdit window

class CHistEdit : public CEdit
{
// Construction
public:
	std::vector<CString> m_str_history;
	CHistEdit();

// Attributes
public:

// Operations
public:
	void AddToHistory(LPCTSTR pStr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHistEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHistEdit)
	afx_msg void OnChange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTEDIT_H__2297E15D_F1D5_4940_A57D_27C9F496468E__INCLUDED_)
