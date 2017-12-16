// CommandBar.cpp : implementation file
//

#include "stdafx.h"
#include "coolexplorer.h"
#include "HistEdit.h"
#include "CommandBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommandBar dialog


CCommandBar::CCommandBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CCommandBar)
	//}}AFX_DATA_INIT
	m_pFilesListObject = NULL;
}


void CCommandBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommandBar)
	DDX_Control(pDX, IDC_COMMAND_EDIT, m_command);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommandBar, CDialogBar)
	//{{AFX_MSG_MAP(CCommandBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EVENTUPDATE, OnEventUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommandBar message handlers

BOOL CCommandBar::DoCommand(CString &str) 
{

	if (!str.IsEmpty())
	{
		LRESULT iReturn = GetParentFrame()->SendMessage(WM_DO_COMMAND, 0, (LPARAM)(LPCTSTR)str);
		EraseText();
		if (iReturn != -1)
		{//add to command history

		}

	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCommandBar::AppendText(const CString &strText, BOOL bFocus/* = FALSE*/) 
{
	CString strOld;
	CString newText;

	CHistEdit *pCtrl = (CHistEdit*)GetDlgItem(IDC_COMMAND_EDIT);

	if (pCtrl != NULL)
	{
		if (pCtrl->GetWindowTextLength() > 0)
		{
			pCtrl->GetWindowText(strOld);

			newText = strOld + _T(" ") + strText;
		}
		else
		{
			newText = strText;
		}

		pCtrl->SetWindowText(newText);
		
		if (bFocus)
		{
			pCtrl->SetFocus();
			int length = newText.GetLength();

			pCtrl->SetSel(length,length);
		}

		
		return FALSE;
	}
	

	return TRUE;
}


BOOL CCommandBar::IsEmptyCommandText()
{
	int iLength = 0;

	CHistEdit *pCtrl = (CHistEdit*)GetDlgItem(IDC_COMMAND_EDIT);

	if (pCtrl != NULL)
	{
		iLength = pCtrl->GetWindowTextLength();
	}

	return iLength == 0?TRUE:FALSE;
}

BOOL CCommandBar::EraseText()
{
	int iLength = 0;

	CHistEdit *pCtrl = (CHistEdit*)GetDlgItem(IDC_COMMAND_EDIT);

	if (pCtrl != NULL)
	{
		pCtrl->SetWindowText(_T(""));
	}

	return TRUE;
}

CString CCommandBar::GetCommandText() 
{
	CString strRet = _T("");

	CHistEdit *pCtrl = (CHistEdit*)GetDlgItem(IDC_COMMAND_EDIT);

	if (pCtrl != NULL)
	{
		pCtrl->GetWindowText(strRet);
	}

	return strRet;
}




BOOL CCommandBar::PreTranslateMessage(MSG* pMsg) 
{

	if (pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			
		case VK_RETURN:
			{
					CString sCommand = _T("");
					GetDlgItemText( IDC_COMMAND_EDIT, sCommand );
					DoCommand( sCommand );

					
				return TRUE;
			}
			break;
			case VK_ESCAPE:
			{
				GetParentFrame()->SendMessage(WM_DO_FOCUS, 0, 0);
				
				return TRUE;
			}
			break;
		
		default:
			break;
		}
	}
	return CDialogBar::PreTranslateMessage(pMsg);
}

void CCommandBar::PathChanged(LPCTSTR sPath)
{
	if(sPath && _tcslen(sPath))
	{
		CWnd *pWnd = GetDlgItem(IDC_CURRENT_DIR_STATIC);
		if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
		{
			if (!_tcscmp(sPath,_T_LNGSTR(9)))
			{
				pWnd->SetWindowText(_T_LNGSTR(9));
			}
			else
			{
				pWnd->SetWindowText(sPath);
			}
		}
	}
}

void CCommandBar::SetWidth(int nWidth)
{
	nWidth = nWidth - 10;
	if(nWidth < 0)
		return;
	CWnd *pStatic = GetDlgItem(IDC_CURRENT_DIR_STATIC);
	CWnd *pCombo = GetDlgItem(IDC_COMMAND_EDIT);
	
	int nStat = (nWidth*2)/5;
	int nCombo = nWidth - nStat;
	CRect rect;
	pStatic->GetWindowRect(rect);
	ScreenToClient(rect);
	rect.left = 5;
	rect.right = rect.left + nStat;
	pStatic->MoveWindow(rect);

	pCombo->GetWindowRect(rect);
	ScreenToClient(rect);
	rect.left = 10 + nStat;
	rect.right = rect.left + nCombo;
	pCombo->MoveWindow(rect);

}



int CCommandBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}


void CCommandBar::Update(CFilesListObservable *pObject, int nDataVersion, BOOL Result)
{
	m_pFilesListObject = pObject;
	PostMessage(WM_EVENTUPDATE);
	
}


LRESULT CCommandBar::OnEventUpdate(WPARAM, LPARAM)
{
	if(m_pFilesListObject)
		PathChanged(m_pFilesListObject->GetDir());	
	return 1L;
}