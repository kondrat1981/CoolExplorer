// FlatButton.cpp : implementation file
//

#include "stdafx.h"
#include "FlatButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlatButton
IMPLEMENT_DYNCREATE(CFlatButton, CButton)


CFlatButton::CFlatButton()
{
	m_bMouseIn = FALSE;
	m_bSelected = FALSE;
}

CFlatButton::~CFlatButton()
{
}


BEGIN_MESSAGE_MAP(CFlatButton, CButton)
	//{{AFX_MSG_MAP(CFlatButton)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatButton message handlers

void CFlatButton::PreSubclassWindow() 
{
	ModifyStyle(NULL, BS_OWNERDRAW, SWP_FRAMECHANGED);
	
	CButton::PreSubclassWindow();
}

void CFlatButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	BOOL bPressed = m_bSelected;//(lpDIS->itemState & ODS_SELECTED);
	COLORREF rgbBkg = bPressed ? RGB(210, 210, 210) : GetSysColor(COLOR_3DFACE);
	CRect rcItem(lpDIS->rcItem);

	if(m_bMouseIn && !bPressed)
		rgbBkg = RGB(230, 230, 230);

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	CBrush brBkg(rgbBkg);

	pDC->FillRect(rcItem, &brBkg);
	//pDC->Draw3dRect(rcItem, GetSysColor(bPressed ? COLOR_3DSHADOW : COLOR_3DHILIGHT), GetSysColor(bPressed ? COLOR_3DHILIGHT : COLOR_3DSHADOW));
	
	if(bPressed)
		rcItem.OffsetRect(1, 1);

//	DrawIconEx(pDC->GetSafeHdc(),3,2,m_hIcon,0,0,0,0,DI_NORMAL);
	
	CString sText;
	GetWindowText(sText);
	//CFont font;
	//pDC->SelectObject(GetFont());
	if(lpDIS->itemState & ODS_DISABLED)
	{
		pDC->SetTextColor(GetSysColor(COLOR_3DHILIGHT));
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(sText, rcItem + CPoint(1,1), DT_VCENTER|DT_CENTER |DT_SINGLELINE);
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		pDC->DrawText(sText, rcItem , DT_VCENTER|DT_CENTER |DT_SINGLELINE);

	}
	else
	{
		pDC->SetBkColor(rgbBkg);
		pDC->DrawText(sText, rcItem , DT_VCENTER|DT_CENTER |DT_SINGLELINE);
	}
	
	
	pDC->Draw3dRect(rcItem, GetSysColor(bPressed ? COLOR_3DSHADOW : COLOR_3DHILIGHT), GetSysColor(bPressed ? COLOR_3DHILIGHT : COLOR_3DSHADOW));

	brBkg.DeleteObject();
}

LRESULT CFlatButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_LBUTTONDBLCLK)
		message = WM_LBUTTONDOWN;
	return CButton::WindowProc(message, wParam, lParam);
}

void CFlatButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	CWnd *pWnd = WindowFromPoint(point);
	if(pWnd == this && !m_bMouseIn)
	{
		m_bMouseIn = TRUE;
		Invalidate();

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		::_TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}

LRESULT CFlatButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bMouseIn = FALSE;
	Invalidate();
	return 0;
}

BOOL CFlatButton::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	
	//return CButton::OnEraseBkgnd(pDC);
}
