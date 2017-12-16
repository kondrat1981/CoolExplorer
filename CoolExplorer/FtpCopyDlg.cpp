// FtpCopyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoolExplorer.h"
#include "FtpCopyDlg.h"
#include "SimpleEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFtpCopyDlg dialog


CFtpCopyDlg::CFtpCopyDlg(CFtpConnection *pSourceFtpConection, CFtpConnection *pTargetFtpConection, BOOL bFreeSourceFtp, BOOL bFreeTargetFtp, LPCTSTR sPath, LPCTSTR sPathTo, CStringList *plFiles, CWnd* pParent /*=NULL*/, CWnd* pSecWnd, FtpProgressInfo *pFtpProgressInfo)
	: CNewProgressDlg(CFtpCopyDlg::IDD, pParent, sPath, sPathTo, plFiles, pSecWnd)
{
	//{{AFX_DATA_INIT(CFtpCopyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bFreeSourceFtp = bFreeSourceFtp;
	m_bFreeTargetFtp = bFreeTargetFtp;
	if(!_tcslen(sPath) || !_tcslen(sPathTo))
	{
		AfxMessageBox(_T("Target or source path is empty!!!"));
//		delete this;
		return;
	}

	m_pFtpProgressInfo = pFtpProgressInfo;

	//m_bInProgress = FALSE;
	m_nWsp = 1;

	m_pSourceFtpConection = pSourceFtpConection;
	m_pTargetFtpConection = pTargetFtpConection;

	//if(m_pTargetFtpConection && m_bFreeTargetFtp)
	{
		m_pFtpProgressInfo = new FtpProgressInfo;
		m_pFtpProgressInfo->Clear();
		m_pFtpProgressInfo->pWnd = pFtpProgressInfo->pWnd;			
	}

//	if(m_sPathTo[m_sPathTo.GetLength()-1] != '\\')
//		m_sPathTo += '\\';
	Create(CFtpCopyDlg::IDD);
	CenterWindow(AfxGetMainWnd());
	ShowWindow(SW_SHOW);
	//PostMessage(WM_CLOSE);
}
CFtpCopyDlg::~CFtpCopyDlg()
{
	if(m_pFtpProgressInfo)
		delete m_pFtpProgressInfo;
	m_pFtpProgressInfo = NULL;
//	delete this;
}


void CFtpCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewProgressDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFtpCopyDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFtpCopyDlg, CNewProgressDlg)
	//{{AFX_MSG_MAP(CFtpCopyDlg)
	//ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_FTPCOPY_CANCEL, OnFtpcopyCancel)
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_PROGRESS, OnRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtpCopyDlg message handlers

BOOL CFtpCopyDlg::OnInitDialog() 
{
	CSimpleEdit seDlg(_T("Copy Files To:"), m_sPathTo, AfxGetMainWnd(), TRUE);
	if(seDlg.DoModal() == IDOK)
	{
		m_sPathTo = seDlg.GetText();
		m_sMask = seDlg.m_sMask;		
	}
	else
	{
		PostMessage(WM_MYCLOSE);
		return TRUE;
	}

	CNewProgressDlg::OnInitDialog();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFtpCopyDlg::ThreadFun()
{
	if(m_sMask.IsEmpty())
		m_sMask = "*";

	if((m_sPath.Right(1) == _T("\\") && m_sPath.Right(2) != _T(":\\")) || m_sPath.Right(1) == _T("/"))
	{
		m_sPath = m_sPath.Left(m_sPath.GetLength() - 1);
	}

	CopyInfo info;
	info.Clear();
	info.pWnd = this;
	info.sStatus = _T("Listing files...");
	m_pInfo = &info;

//	m_bInProgress = TRUE;
	PostMessage(WM_PROGRESS, PROGRESS_STAT, (LPARAM)m_pInfo);
	
	if(m_pSourceFtpConection)
		m_nSize = m_DirTree.FtpInit(m_pSourceFtpConection, m_sPath, m_sMask, &m_lstrFiles,&info);
	else
	{		
		SetTimer(PROGRESS_DIRNAME, 50, NULL);
		m_nSize = m_DirTree.Init(m_sPath, m_sMask, m_lstrFiles,&info);
		KillTimer(PROGRESS_DIRNAME);		
	}

	info.Lock();
	info.nAllSize = m_nSize;
	info.sStatus = _T("Copying files...");
	info.time = CTime::GetCurrentTime();
	if(m_pTargetFtpConection)
		m_pInfo->bFtpSent = TRUE;
	BOOL bCancel = info.IsCancelled(FALSE);
	info.pFtpCon = m_pSourceFtpConection;
	info.pFtpConDest = m_pTargetFtpConection;
	info.Unlock();

	if(!bCancel)
	{
		if(m_nSize > MAXSHORT)
			m_nWsp = (UINT)(m_nSize / MAXSHORT + 2);
		else
			m_nWsp = 1;
		
		LPARAM nRange = (LPARAM)(m_nSize / m_nWsp);
		PostMessage(WM_PROGRESS, PROGRESS_RANGE, nRange);
		
		if(m_DirTree.Count()!=1)
			nRange = 10000;
		
		PostMessage(WM_PROGRESS, PROGRESS_RANGE_ONE, nRange);
		
		PostMessage(WM_PROGRESS, PROGRESS_STAT, (LPARAM)&info);
		m_pFtpProgressInfo->pCopyInfo = &info;
		SetTimer(PROGRESS_POS, 50, NULL);
		if(m_pSourceFtpConection)
		{
			m_DirTree.CopyFtp(m_pSourceFtpConection, _T(""), m_sPathTo, m_pFtpProgressInfo);
		}
		else
		{			
			ASSERT(m_pTargetFtpConection);
			m_DirTree.Copy(m_pTargetFtpConection, _T(""), m_sPathTo, m_pFtpProgressInfo);
		}
		SetTimer(PROGRESS_POS, 50, NULL);
	}

	bCancel = info.IsCancelled();
	if(bCancel)
	{
		m_pInfo = NULL;
		AfxMessageBox(_T_LNGSTR(97));
	}
	m_pInfo = NULL;

	m_pFtpProgressInfo->pCopyInfo = NULL;
	if(m_bFreeSourceFtp && m_pSourceFtpConection)
	{
		m_pSourceFtpConection->Close();
		delete m_pSourceFtpConection;
	}
	if(m_bFreeTargetFtp && m_pTargetFtpConection)
	{
		m_pTargetFtpConection->Close();
		delete m_pTargetFtpConection;
	}
//	m_bInProgress = FALSE;
	return !bCancel;
}

/*LRESULT CFtpCopyDlg::OnRefresh(WPARAM type, LPARAM value)
{
	CopyInfo * pInfo = (CopyInfo *)value;
	if(m_bInProgress)
	{
		CString sText;
		switch(type)
		{
		case PROGRESS_RANGE_ONE:
			m_ProgressOne.SetRange(0, (short)value);
			TRACE(_T("PROGRESS_RANGE_ONE %d\n"), value);
			break;		
		case PROGRESS_RANGE:
			m_Progress.SetRange(0, (short)value);
			TRACE(_T("PROGRESS_RANGE %d\n"), value);
			break;
		case PROGRESS_FROM:
			m_Progress.SetPos(value);
			TRACE(_T("PROGRESS_FROM %d\n"), value);
			break;
		
		case PROGRESS_NEW:
			{
				pInfo->Lock();
				sText = pInfo->sNameFrom;
				pInfo->Unlock();
				GetDlgItem(IDC_FTPCOPY_FILENAME)->SetWindowText(sText);
			}
		case PROGRESS_POS:
			{
				pInfo->Lock();
				UINT pos = (UINT)(pInfo->nAllCopied + pInfo->nFileCopied)/ m_nWsp;
				LONGLONG nFileSize = pInfo->nFileSize;
				LONGLONG nFileCopied = pInfo->nFileCopied;
				pInfo->Unlock();


				m_Progress.SetPos(pos);
				TRACE(_T("PROGRESS_NEW SetPos %d"), pos);
				if(m_nSize != nFileSize)
				{
					if(nFileSize>0 && nFileCopied>0)
						pos = nFileCopied*10000/nFileSize;
					else
						pos = 0;
				}
				TRACE(_T("SetPosOne %d\n"), pos);
				m_ProgressOne.SetPos(pos);
			}
			break;
		case PROGRESS_DIRNAME:
			pInfo->Lock();
			sText = pInfo->sDirName;
			pInfo->Unlock();

			GetDlgItem(IDC_FTPCOPY_FILENAME)->SetWindowText(sText);
			break;
		case PROGRESS_WINNAME:
			SetWindowText((LPCTSTR)value);		
		break;
		default: ASSERT(0); break;
		}
	}
//	delete sFileName;
	return 0;
}*/

void CFtpCopyDlg::PostNcDestroy() 
{
	CNewProgressDlg::PostNcDestroy();
//	delete this;
}

void CFtpCopyDlg::OnCancel() 
{
	CNewProgressDlg::OnCancel();	
}

void CFtpCopyDlg::Cancel() 
{
	CNewProgressDlg::Cancel();
	Sleep(500);
	if(m_pInfo)
	{
		if(m_pSourceFtpConection)
			m_pSourceFtpConection->Close();
		if(m_pTargetFtpConection)
			m_pTargetFtpConection->Close();
	}
}


void CFtpCopyDlg::OnFtpcopyCancel() 
{
	Cancel();
}

BOOL CFtpCopyDlg::InitWnd()
{
	CDataExchange dx(this, FALSE);
	DDX_Control(&dx, IDC_FTPCOPY_PROGRESS_ONE, *m_pProgressOne);
	DDX_Control(&dx, IDC_FTPCOPY_PROGRESS, *m_pProgress);
	DDX_Control(&dx, IDC_FTPCOPY_BYTES_COPIED, *m_pWndCopied);
	DDX_Control(&dx, IDC_FTPCOPY_TIME, *m_pWndTime);
	DDX_Control(&dx, IDC_FTPCOPY_TIME_ELAPSED, *m_pWndTimeElapsed);
	DDX_Control(&dx, IDC_FTPCOPY_SPEED, *m_pWndSpeed);
	DDX_Control(&dx, IDC_FTPCOPY_STATUS, *m_pWndStatus);
	DDX_Control(&dx, IDC_FTPCOPY_FILENAME, *m_pWndFileName);
	return TRUE;
}

BOOL CFtpCopyDlg::IsSharedFtpCon(CFtpConnection * pFtpCon)
{
	if(m_pInfo)
	{
		if(	!m_bFreeSourceFtp && m_pSourceFtpConection && m_pSourceFtpConection == pFtpCon
			|| !m_bFreeTargetFtp && m_pTargetFtpConection && m_pTargetFtpConection == pFtpCon)
		{
			return TRUE;
		}
	}

	return FALSE;
}
