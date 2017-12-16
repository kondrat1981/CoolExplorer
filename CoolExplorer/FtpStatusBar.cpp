// FtpStatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "coolexplorer.h"
#include "FtpStatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFtpStatusBar dialog
IMPLEMENT_DYNCREATE(CFtpStatusBar, CDialogBar)

void CALLBACK MyFtpCallBack(
    HINTERNET hInternet,
    DWORD_PTR dwContext,
    DWORD dwInternetStatus,
    LPVOID lpvStatusInformation,
    DWORD dwStatusInformationLength
)
{
	FtpProgressInfo *pInfo = (FtpProgressInfo *)dwContext;
	CFtpStatusBar *pWnd = (CFtpStatusBar *)pInfo->pWnd;
	
	if(pWnd->IsKindOf(RUNTIME_CLASS(CFtpStatusBar))
		&& ::IsWindow(pWnd->GetSafeHwnd()))
	{			
		pWnd->AddText(dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);		
	}
	else
		ASSERT(0);

/*	if(dwInternetStatus == INTERNET_STATUS_REQUEST_SENT)
		TRACE("dwInternetStatus == INTERNET_STATUS_REQUEST_SENT\n");
	if(dwInternetStatus == INTERNET_STATUS_RESPONSE_RECEIVED)
		TRACE("dwInternetStatus == INTERNET_STATUS_RESPONSE_RECEIVED\n");
	TRACE("pinfo %d pInfo->pCopyInfo %d pWnd %d\n", pInfo, pInfo->pCopyInfo, pInfo->pCopyInfo? pInfo->pCopyInfo->pWnd:0);
*/
	if(pInfo && pInfo->pCopyInfo && pInfo->pCopyInfo->pWnd)
	{

		TRACE(_T("%d \n"), pInfo->pCopyInfo->bFtpSent);
		

		if(pInfo->pCopyInfo->bFtpSent && dwInternetStatus == INTERNET_STATUS_REQUEST_SENT

			|| !pInfo->pCopyInfo->bFtpSent && dwInternetStatus == INTERNET_STATUS_RESPONSE_RECEIVED)
		{
			pInfo->pCopyInfo->nFileCopied += *(DWORD*)lpvStatusInformation;
			TRACE(_T("Post %d\n"), pInfo->pCopyInfo->nFileCopied);
			//pInfo->pCopyInfo->pWnd->PostMessage(WM_PROGRESS, PROGRESS_POS, (LPARAM) pInfo->pCopyInfo);
		}
	}
}


CFtpStatusBar::CFtpStatusBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CFtpStatusBar)
	m_sHis = _T("");
	//}}AFX_DATA_INIT
	m_dwBytesReceived = 0;
	m_dwBytesSent = 0;
}


void CFtpStatusBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpStatusBar)
	//DDX_Text(pDX, IDC_FSB_STATUS, m_sHis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFtpStatusBar, CDialogBar)
	//{{AFX_MSG_MAP(CFtpStatusBar)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtpStatusBar message handlers

void CFtpStatusBar::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);
	CWnd *pCombo = GetDlgItem(IDC_FSB_STATUS);
	//CWnd *pInf = GetDlgItem(IDC_FSB_REC_SEND);
	if(pCombo /*&& pInf*/)
	{
		CRect rect;
		pCombo->GetWindowRect(rect);
		ScreenToClient(rect);
		pCombo->SetWindowPos(NULL,0,0,cx - rect.left, rect.Height(),SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
/*
		pInf->GetWindowRect(rect);
		ScreenToClient(rect);
		pInf->SetWindowPos(NULL,0,0,cx - rect.left, rect.Height(),SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		*/
	}
}

BOOL CFtpStatusBar::PreTranslateMessage(MSG* pMsg) 
{
	/*if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			UpdateData();
			this->InformAll(m_sPath);
			//GetParent()->SendMessage(WM_UPDATEPATH, FROM_PATHBAR, (LPARAM)(LPCTSTR)m_sPath);
		}
	}*/
	
	return CDialogBar::PreTranslateMessage(pMsg);
}

void CFtpStatusBar::AddText(DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	//////////
	
	//static CString sPrevText;
	CString sText;
	DWORD dwLength;
	//BOOL bForRecSent = FALSE;
	int nControlToUpdate = IDC_FSB_STATUS;
	switch(dwInternetStatus)
	{
	case INTERNET_STATUS_SENDING_REQUEST:
		sText = "SENDING_REQUEST";
		break;
	case INTERNET_STATUS_CONNECTED_TO_SERVER:
		sText = "CONNECTED_TO_SERVER";
		break;
	case INTERNET_STATUS_CONNECTING_TO_SERVER:
		sText = "Conecting to server";
	case INTERNET_STATUS_HANDLE_CREATED:
		sText = "HANDLE_CREATED";
		//INTERNET_ASYNC_RESULT pData * lpvStatusInformation;
		break;
	case INTERNET_STATUS_REQUEST_SENT:
		//sText.Format("REQUEST_SENT %d",(DWORD)lpvStatusInformation);
		dwLength = m_dwBytesSent += *(DWORD*)lpvStatusInformation;
		nControlToUpdate = IDC_FSB_SENT;
		break;
	case INTERNET_STATUS_RESPONSE_RECEIVED:
		dwLength = m_dwBytesReceived += *(DWORD*)lpvStatusInformation;
		nControlToUpdate = IDC_FSB_RECEIVED;
		//sText.Format("RESPONSE_RECEIVED %d",(DWORD)lpvStatusInformation);
		break;
	case INTERNET_STATUS_RECEIVING_RESPONSE:
		sText = _T("RECEIVING_RESPONSE");
		break;
	case INTERNET_STATUS_NAME_RESOLVED:
		sText.Format(_T("IP address %s"), (const char *)lpvStatusInformation);
		break;


	}


	////////////////
	
	
	

	if(IDC_FSB_RECEIVED == nControlToUpdate || IDC_FSB_SENT == nControlToUpdate)
	{
		CWnd *pWnd = GetDlgItem(nControlToUpdate);
		if(pWnd)
		{
			if(dwLength>1024*10)
			{
				dwLength = dwLength/1024;
				sText.Format(_T("%ld KB"), dwLength);
			}
			else
			{
				sText.Format(_T("%ld"), dwLength);
			}
			
			pWnd->SetWindowText(sText);
		}
	}
	else if(IDC_FSB_STATUS == nControlToUpdate)
	{
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_FSB_STATUS);
		if(pEdit)
		{
			static CString sPrev;
			
			//
			
			DWORD dwError;
			DWORD dwBufferLength = 1024;
			LPTSTR szText = new TCHAR[dwBufferLength];
			szText[0] = 0;
			
			VERIFY(InternetGetLastResponseInfo( &dwError, szText, &dwBufferLength));
			
			if(sPrev != szText)
			{
				m_sHis += szText;
				pEdit->SetWindowText(m_sHis);
				pEdit->LineScroll( pEdit->GetLineCount() - pEdit->GetFirstVisibleLine() - 3);
				sPrev = szText;
			}
			delete [] szText;			
		}		
	}
}

