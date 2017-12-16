// CoolViewerDoc.h : interface of the CCoolViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COOLVIEWERDOC_H__D4BEDC84_490D_4C08_B77B_43744A7D71CD__INCLUDED_)
#define AFX_COOLVIEWERDOC_H__D4BEDC84_490D_4C08_B77B_43744A7D71CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCoolViewerDoc : public CDocument
{
protected: // create from serialization only
	CCoolViewerDoc();
	DECLARE_DYNCREATE(CCoolViewerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCoolViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCoolViewerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLVIEWERDOC_H__D4BEDC84_490D_4C08_B77B_43744A7D71CD__INCLUDED_)
