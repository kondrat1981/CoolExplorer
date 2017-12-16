// CoolViewerView.cpp : implementation of the CCoolViewerView class
//

#include "stdafx.h"
#include "CoolViewer.h"

#include "CoolViewerDoc.h"
#include "MainFrm.h"
#include "CoolViewerView.h"
#include "TTextView.h"

#include <new>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolViewerView

IMPLEMENT_DYNCREATE(CCoolViewerView, CView)

BEGIN_MESSAGE_MAP(CCoolViewerView, CView)
	//{{AFX_MSG_MAP(CCoolViewerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_SIZING()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolViewerView construction/destruction

CCoolViewerView::CCoolViewerView():m_Ctrl(NULL),m_mode(eEmptyView),file_buffer(NULL)
{
	ASSERT(GetApp());

	if (!GetApp()->m_filename.IsEmpty())
	{
		m_filename = GetApp()->m_filename;

		m_mode = CheckFileType();

	}


}

BOOL CCoolViewerView::CreateViewerCtrl()
{
	ASSERT(m_mode);

	if (m_Ctrl)
	{
		DestroyViewerCtrl();
	}

	m_Ctrl = new TTextView();

	RECT r;
	GetClientRect(&r);
	r.bottom = r.bottom - 18;

	if (m_Ctrl)
	{
		m_Ctrl->Create(WS_CHILD|WS_VISIBLE,r,this,12222,NULL);
	}
	else
	{
		return FALSE;
	}


	
	return TRUE;
}

BOOL CCoolViewerView::DestroyViewerCtrl()
{
	if (m_Ctrl != NULL)
	{
		delete m_Ctrl;
		m_Ctrl = NULL;
	}

	return TRUE;
}


CCoolViewerView::~CCoolViewerView()
{
	DestroyViewerCtrl();
}

BOOL CCoolViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCoolViewerView drawing

void CCoolViewerView::OnDraw(CDC* pDC)
{
	CCoolViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCoolViewerView printing

BOOL CCoolViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCoolViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCoolViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

eViewMode CCoolViewerView::CheckFileType()
{
	ASSERT(!m_filename.IsEmpty());
	eViewMode mode = eTextNormal;

	


	return mode;
}


/////////////////////////////////////////////////////////////////////////////
// CCoolViewerView diagnostics




#ifdef _DEBUG
void CCoolViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CCoolViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCoolViewerDoc* CCoolViewerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCoolViewerDoc)));
	return (CCoolViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCoolViewerView message handlers

void CCoolViewerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	if (m_mode != eEmptyView)
	{
		if (!CreateViewerCtrl())
		{
			ASSERT(0);
		}
		LoadTextFile(m_filename);
	}

	// TODO: Add your specialized code here and/or call the base class
}

void CCoolViewerView::LoadTextFile(LPCTSTR pFilename)
{
	ASSERT(pFilename);
	void *ptr = NULL;

	if (pFilename == NULL)
		return;

	if (file.GetFilePtr(pFilename,ptr))
	{
		m_Ctrl->SetMode( eTextNormal );
		m_Ctrl->SetTextBinding(file.GetPtr(),file.GetSize());
		int num = m_Ctrl->GetTotalLines();
		GetFrame()->SetStatusbarText(_T(""));
		int i = 0;
	}


}
void CCoolViewerView::OnSizing(UINT fwSide, LPRECT pRect)
{
	CView::OnSizing(fwSide, pRect);
	// TODO: Add your message handler code here
}

void CCoolViewerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_Ctrl)
		m_Ctrl->Resize(cx, cy);
	// TODO: Add your message handler code here
}

BOOL CCoolViewerView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_UP:
			{
				m_Ctrl->MoveToPrevLine( TRUE );
			}
			break;
		case VK_DOWN:
			{
				m_Ctrl->MoveToNextLine( TRUE );
			}
			break;
		case VK_PRIOR:
			{
				m_Ctrl->MoveToPrevPage();
			}
			break;
		case VK_NEXT:
			{
				m_Ctrl->MoveToNextPage();
			}
			break;

		}
	}
	return CView::PreTranslateMessage(pMsg);
}
