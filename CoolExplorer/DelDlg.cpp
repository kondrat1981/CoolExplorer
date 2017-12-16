// DelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoolExplorer.h"
#include "DelDlg.h"
#include "SimpleEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDelDlg dialog


CDelDlg::CDelDlg(CFtpConnection *pFtpConnection, LPCTSTR sPath, CStringList *plFiles, CWnd* pParent /*=NULL*/, CWnd *pRefresh)
	: CNewProgressDlg(CDelDlg::IDD, pParent, sPath, _T(""),plFiles, pRefresh)
{
	//{{AFX_DATA_INIT(CDelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_pDelInfo = NULL;
	m_pFtpConnection = pFtpConnection;

}


void CDelDlg::Create()
{
	CNewProgressDlg::Create(CDelDlg::IDD);
	//ShowWindow(SW_SHOW);
	if( ::IsWindow( m_hWnd ) )
	{
		CenterWindow(AfxGetMainWnd());
		ShowWindow(SW_SHOW);
	}	

}


void CDelDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewProgressDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDelDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDelDlg, CNewProgressDlg)
	//{{AFX_MSG_MAP(CDelDlg)
	ON_BN_CLICKED(IDC_DELCANCEL, OnDelCancel)	
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PROGRESS, OnRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDelDlg message handlers


BOOL CDelDlg::ThreadFun()
{
	//CString *pStr = new CString(_T("Listing files"));

	//PostMessage(WM_PROGRESS, NAME_FROM, (LPARAM)_T("Listing files"));

	if(m_sMask.IsEmpty())
		m_sMask = "*";

	CopyInfo info;
	
	info.Clear();	

	info.nCount = 0;
	info.pWnd = this;
	info.SetFlags(0,FALSE);
	info.bMesInErr = (m_sMask == _T("*") || m_sMask == _T("*.*"));
	info.pFtpCon = m_pFtpConnection;
	info.sStatus = _T_LNGSTR(98);

	
	info.pWnd = this;
//	info.pFun = ListingProgress;

	m_pInfo = &info;
	PostMessage(WM_PROGRESS, PROGRESS_STAT, (LPARAM)&info);
	if(m_pFtpConnection)
	{
		m_pFtpConnection->SetCurrentDirectory(m_sPath);
		m_DirTree.FtpInit(m_pFtpConnection, m_sPath, m_sMask, &m_lstrFiles, &info);
		CString sMes;
		CStringList lF, lD;
		m_DirTree.GetFiles(lF, lD);
		POSITION pos = lF.GetHeadPosition();
		sMes += _T("Delete?:\nFiles:\n");
		while(pos)
		{
			sMes+=lF.GetNext(pos);
		}
		pos = lD.GetHeadPosition();
		sMes += _T("\nDirs:\n");
		while(pos)
		{
			sMes+=lD.GetNext(pos);
		}

		if(AfxMessageBox(sMes, MB_OKCANCEL| MB_ICONQUESTION)!=IDOK)
		{
			m_pInfo = NULL;
			return FALSE;
		}
	}
	else
	{
		SetTimer(PROGRESS_DIRNAME, 50, NULL);
		m_DirTree.Init(m_sPath, m_sMask, m_lstrFiles, &info);
		KillTimer(PROGRESS_DIRNAME);
	}

	info.Lock();

	BOOL bCancelled = info.IsCancelled(FALSE);
	info.sStatus = _T_LNGSTR(117);
	info.Unlock();

	m_nSize = m_DirTree.Count();

	//AfxMessageBox(_T("test"));
	
	if(!info.IsCancelled())
	{
		//((CProgressCtrl *)GetDlgItem(IDC_PROGRESS))->SetRange(0, m_DirTree.Count());
		PostMessage(WM_PROGRESS, PROGRESS_RANGE, (LPARAM)m_nSize);
		
		//	m_DelInfo.bCancel = FALSE;
		
		TCHAR sCurDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, sCurDir);
		try
		{
			SetTimer(PROGRESS_AND_NAME,50,NULL);
			PostMessage(WM_PROGRESS, PROGRESS_STAT, (LPARAM)&info);			
			m_DirTree.Delete(_T(""), &info);
			KillTimer(PROGRESS_AND_NAME);

		}
		catch(CUserException *pEx)
		{
			pEx->Delete();
		}
		SetCurrentDirectory(sCurDir);		
	}

	if(info.IsCancelled())
	{
		m_pInfo = NULL;
		AfxMessageBox(_T_LNGSTR(97));
	}


	m_pInfo = NULL;
	m_pFtpConnection = NULL;

	return TRUE;
	//PostMessage(WM_CLOSE);
}
/*
LRESULT CDelDlg::OnRefresh(WPARAM type, LPARAM value)
{
	if(m_bInProgress && m_pDelInfo && m_pInfo)
	{
		CString sText;
		switch(type)
		{
		case PROGRESS_RANGE:
			m_Progress.SetRange(0, (short)value);		
			break;
		case PROGRESS_FROM:
			m_Progress.SetPos(value);
			break;
		case PROGRESS_DIRNAME:
			m_pInfo->Lock();
			sText = m_pInfo->sDirName;
			m_pInfo->Unlock();
			GetDlgItem(IDC_PROGRESS_STATIC)->SetWindowText(sText); // trudno zrozumiec ale tak ma byc
			break;
		case NAME_FROM:
			m_pDelInfo->Lock();
			sText = m_pDelInfo->sName;
			m_pDelInfo->Unlock();

			GetDlgItem(IDC_PROGRESS_STATIC)->SetWindowText(sText); // trudno zrozumiec ale tak ma byc
			break;
		case PROGRESS_WINNAME:
			SetWindowText((LPCTSTR)value);
			break;
		default: ASSERT(0); break;
		}
	}
	return 0;
}
*/

void CDelDlg::PostNcDestroy() 
{
	CNewProgressDlg::PostNcDestroy();
//	delete this;
}

void CDelDlg::OnCancel() 
{
	CNewProgressDlg::OnCancel();
}

void CDelDlg::Cancel()
{
	CNewProgressDlg::Cancel();
}

void CDelDlg::OnDelCancel() 
{
	Cancel();
}

BOOL CDelDlg::InitWnd()
{
	CDataExchange dx(this, FALSE);
	DDX_Control(&dx, IDC_PROGRESS, *m_pProgress);
	//DDX_Control(&dx, IDC_FTPCOPY_BYTES_COPIED, *m_pWndCopied);
	//DDX_Control(&dx, IDC_FTPCOPY_TIME, *m_pWndTime);
	DDX_Control(&dx, IDC_PROGRESS_TIME_ELAPSED, *m_pWndTimeElapsed);
	//DDX_Control(&dx, IDC_FTPCOPY_SPEED, *m_pWndSpeed);
	DDX_Control(&dx, IDC_PROGRESS_STATUS, *m_pWndStatus);
	DDX_Control(&dx, IDC_PROGRESS_FILENAME, *m_pWndFileName);
	return TRUE;
}

BOOL CDelDlg::OnInitDialog() 
{
	CString str;
	if(m_lstrFiles.GetCount() == 1)
		str.Format(_T_LNGSTR(111), AddPathNames(m_sPath,m_lstrFiles.GetHead()));
	else
		str.Format(_T_LNGSTR(112), m_lstrFiles.GetCount(), m_sPath);
	CSimpleEdit dlg(str, _T(""), this, TRUE, TRUE);
	if(dlg.DoModal() == IDOK)
	{		
		if(m_sPath.Right(1) == '\\')
			m_sPath = m_sPath.Left(m_sPath.GetLength() - 1);
		if(m_sPath.GetLength() == 2 && m_sPath[1] == ':')
			m_sPath += '\\';
		m_sMask = dlg.m_sMask;
	}
	else
	{
		PostMessage(WM_MYCLOSE);
		return FALSE;
	}
	CNewProgressDlg::OnInitDialog();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDelDlg::IsSharedFtpCon(CFtpConnection * pFtpCon)
{
	if(m_pFtpConnection && m_pFtpConnection == pFtpCon)
		return TRUE;
	return FALSE;
}