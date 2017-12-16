// CoolViewerDoc.cpp : implementation of the CCoolViewerDoc class
//

#include "stdafx.h"
#include "CoolViewer.h"

#include "CoolViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolViewerDoc

IMPLEMENT_DYNCREATE(CCoolViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CCoolViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CCoolViewerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolViewerDoc construction/destruction

CCoolViewerDoc::CCoolViewerDoc()
{
	// TODO: add one-time construction code here

}

CCoolViewerDoc::~CCoolViewerDoc()
{
}

BOOL CCoolViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCoolViewerDoc serialization

void CCoolViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCoolViewerDoc diagnostics

#ifdef _DEBUG
void CCoolViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCoolViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCoolViewerDoc commands
