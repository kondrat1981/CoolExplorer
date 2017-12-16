#if !defined(AFX_FLATBUTTON_H__E6970775_D3D9_4231_A870_17970C0A2DE0__INCLUDED_)
#define AFX_FLATBUTTON_H__E6970775_D3D9_4231_A870_17970C0A2DE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlatButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFlatButton window

class CFlatButton : public CButton
{
// Construction
public:
	CFlatButton();
	DECLARE_DYNCREATE(CFlatButton);
// Attributes
public:
	void SetSelected(BOOL bState){m_bSelected = bState; RedrawWindow();}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlatButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bMouseIn;
	virtual ~CFlatButton();

	// Generated message map functions
protected:
	BOOL m_bSelected;
	//{{AFX_MSG(CFlatButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATBUTTON_H__E6970775_D3D9_4231_A870_17970C0A2DE0__INCLUDED_)
