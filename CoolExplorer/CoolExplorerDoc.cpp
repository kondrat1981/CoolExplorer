// CoolExplorerDoc.cpp : implementation of the CCoolExplorerDoc class
//

#include "stdafx.h"
#include "CoolExplorer.h"

#include "CoolExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerDoc

IMPLEMENT_DYNCREATE(CCoolExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CCoolExplorerDoc, CDocument)
	//{{AFX_MSG_MAP(CCoolExplorerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerDoc construction/destruction

CCoolExplorerDoc::CCoolExplorerDoc()
{	

}

CCoolExplorerDoc::~CCoolExplorerDoc()
{
}

BOOL CCoolExplorerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerDoc serialization

void CCoolExplorerDoc::Serialize(CArchive& ar)
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
// CCoolExplorerDoc diagnostics

#ifdef _DEBUG
void CCoolExplorerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCoolExplorerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerDoc commands
