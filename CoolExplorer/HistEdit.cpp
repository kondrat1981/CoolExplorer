// HistEdit.cpp : implementation file
//

#include "stdafx.h"
#include "coolexplorer.h"
#include "HistEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistEdit

CHistEdit::CHistEdit()
{
}

CHistEdit::~CHistEdit()
{
}

void CHistEdit::AddToHistory(LPCTSTR pStr)
{
	if (!pStr)
		return;

	m_str_history.push_back(pStr);
}


BEGIN_MESSAGE_MAP(CHistEdit, CEdit)
	//{{AFX_MSG_MAP(CHistEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistEdit message handlers

BOOL CHistEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CEdit::PreTranslateMessage(pMsg);
}

void CHistEdit::OnChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CHistEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CHistEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CEdit::OnMouseMove(nFlags, point);
}

BOOL CHistEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}

void CHistEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	// Do not call CEdit::OnPaint() for painting messages
}
