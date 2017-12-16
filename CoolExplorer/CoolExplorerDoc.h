// CoolExplorerDoc.h : interface of the CCoolExplorerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COOLEXPLORERDOC_H__49934B7C_9343_4249_9BE0_05C84FE2ED5E__INCLUDED_)
#define AFX_COOLEXPLORERDOC_H__49934B7C_9343_4249_9BE0_05C84FE2ED5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCoolExplorerDoc : public CDocument
{
protected: // create from serialization only
	CCoolExplorerDoc();
	DECLARE_DYNCREATE(CCoolExplorerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolExplorerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	CImageList m_imgIcons;
	virtual ~CCoolExplorerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCoolExplorerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLEXPLORERDOC_H__49934B7C_9343_4249_9BE0_05C84FE2ED5E__INCLUDED_)
