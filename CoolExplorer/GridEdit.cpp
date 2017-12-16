// GridEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CoolExplorer.h"
#include "GridEdit.h"



// CGridEdit

IMPLEMENT_DYNAMIC(CGridEdit, CEdit)

CGridEdit::CGridEdit(CWnd *parent,int nItem)
{
	m_parent = parent;
	m_Item = nItem;
}

CGridEdit::~CGridEdit()
{
}


BEGIN_MESSAGE_MAP(CGridEdit, CEdit)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CGridEdit message handlers


void CGridEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
}

void CGridEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
}

BOOL CGridEdit::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			m_parent->PostMessage(WM_GRID_EDIT_END,0,0);
			return FALSE;
		}
		if(pMsg->wParam == VK_RETURN)
		{
			CString temp;
			GetWindowText(temp);
			m_parent->PostMessage(WM_GRID_EDIT_END,TRUE,(LPARAM)((LPCTSTR)temp));
			return FALSE;
		}
		/*if (pMsg->wParam == VK_END)
		{
			CString temp;
			GetWindowText(temp);
			int size = temp.GetLength();
			this->SetSel(size,size,TRUE);
			return TRUE;
		}
		
		if (pMsg->wParam == VK_HOME) 
		{
			this->SetSel(0,0,TRUE);
			return TRUE;
		}*/
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

int CGridEdit::GetEditedItem()
{
	return m_Item;
}
CString CGridEdit::GetEditedText()
{
	CString text = _T("");

	GetWindowText(text);

	return text;
}
void CGridEdit::SetEditedText(CString &text)
{
	m_OriginalText = text;
	
	SetWindowText(text);
}