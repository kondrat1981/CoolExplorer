// CoolViewerView.h : interface of the CCoolViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COOLVIEWERVIEW_H__45C86FC5_E08B_4EDB_91C5_C7B711044EF4__INCLUDED_)
#define AFX_COOLVIEWERVIEW_H__45C86FC5_E08B_4EDB_91C5_C7B711044EF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MMFile.h"
#include "TTextView.h"

class CCoolViewerApp;
class CMainFrame;
class CCoolViewerView : public CView
{
protected: // create from serialization only
	CCoolViewerView();
	DECLARE_DYNCREATE(CCoolViewerView)

	eViewMode m_mode;
	CString m_filename;
	TTextView *m_Ctrl;	
	void *file_buffer;
	CMMFile file;
// Attributes
protected:
	
public:
	
	CCoolViewerDoc* GetDocument();
	CCoolViewerApp *GetApp()
	{
		return (CCoolViewerApp*)AfxGetApp();
	}
	CMainFrame *GetFrame()
	{
		return (CMainFrame*)GetParentFrame();
	}


	BOOL CreateViewerCtrl();
	BOOL DestroyViewerCtrl();
	eViewMode CheckFileType(); 
	void LoadTextFile(LPCTSTR pFilename);
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolViewerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCoolViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCoolViewerView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // debug version in CoolViewerView.cpp
inline CCoolViewerDoc* CCoolViewerView::GetDocument()
   { return (CCoolViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLVIEWERVIEW_H__45C86FC5_E08B_4EDB_91C5_C7B711044EF4__INCLUDED_)
