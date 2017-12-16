/*
test
$Id: ColorPicker.cpp,v 1.1 2005/11/07 11:48:37 roberto Exp $

*/

// ColorPicker.cpp : implementation file
//dupa 2


#include "stdafx.h" //
#include "CoolExplorer.h" //
#include "ColorPicker.h"

#ifdef _DEBUG//
#define new DEBUG_NEW//
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorPicker

CColorPicker::CColorPicker() //
{//
	m_rgbColor = RGB(255, 255, 255);//
}//
//
CColorPicker::~CColorPicker()//
{
}


BEGIN_MESSAGE_MAP(CColorPicker, CStatic)
	//{{AFX_MSG_MAP(CColorPicker)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPicker message handlers

void CColorPicker::SetColor(COLORREF rgb)
{
	m_rgbColor = rgb;
	Invalidate(FALSE);
}

void CColorPicker::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	CBrush brush(m_rgbColor);

	GetClientRect(rc);
	dc.SelectObject(brush);
	dc.Rectangle(rc);
	brush.DeleteObject();
}
COLORREF CColorPicker::GetColor()
{
    return m_rgbColor;
}
