// CopyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoolExplorer.h"
#include "CopyDlg.h"
#include "SimpleEdit.h"
#include "ProgressDlg.h"
#include "QueueDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyDlg dialog
class CProgressThread : public CWinThread
{
	DECLARE_DYNCREATE(CProgressThread)
protected:
	CProgressThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
	CNewProgressDlg *m_pWnd;
	//char a[1024*10];
	void ResumeThread();

// Implementation
protected:
	virtual ~CProgressThread();

	// Generated message map functions
	//{{AFX_MSG(CProgressThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CProgressThread

IMPLEMENT_DYNCREATE(CProgressThread, CWinThread)


CProgressThread::CProgressThread()
{
	m_pWnd = NULL;
}


CProgressThread::~CProgressThread()
{
}

void CProgressThread::ResumeThread()
{
	while(CWinThread::ResumeThread());
}

BOOL CProgressThread::InitInstance()
{
	ASSERT(m_pWnd);

	m_pWnd->MainFun();
	return FALSE;
}

int CProgressThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CProgressThread, CWinThread)
	//{{AFX_MSG_MAP(CProgressThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////


CNewProgressDlg::CNewProgressDlg(UINT nID, CWnd *pParent, LPCTSTR sPath, LPCTSTR sPathTo, CStringList *plFiles, CWnd* pSecWnd)
	: CDialog(nID, pParent)
{
	m_bOperationStarted = FALSE;
	m_pThread = NULL;
	m_pInfo = NULL;
	
	m_nCopied = 0;
	m_nWsp = 1;
	m_nSize = 0;
	
	//pointers to windows
	m_pProgressOne = NULL;
	m_pProgress = NULL;

	m_pWndCopied = NULL;
	m_pWndTime = NULL;
	m_pWndTimeElapsed = NULL;
	m_pWndSpeed = NULL;
	m_pWndStatus = NULL;
	m_pWndFileName = NULL;


	m_aRefresh.Add(pParent);
	m_aRefresh.Add(pSecWnd);
	ASSERT(pParent && pSecWnd);
	m_sPath = sPath;
	m_lstrFiles.AddTail(plFiles);
	if(m_lstrFiles.IsEmpty())
	{
		AfxMessageBox(_T_LNGSTR(87));
	}

	m_sPathTo = sPathTo;

	if(_tcslen(sPath) == 0 || (_tcslen(sPathTo) == 0 && nID != IDD_PROGRESS_NEW))
	{
		//AfxMessageBox(_T_LNGSTR(99));
		//delete this;
		//return;
	}
	m_bQueue = FALSE;
	m_pInfo = &m_Info;
	
}

CNewProgressDlg::~CNewProgressDlg()
{
	FreeNotUsedWnd();
}

CopyInfo * CNewProgressDlg::GetCopyInfo(CopyInfo &Info)
{
	Info.Clear();
	if(m_pInfo)
	{
		m_pInfo->Lock();
		Info = *m_pInfo;
		m_pInfo->Unlock();
	}
	return &Info;
}

CopyInfo * CNewProgressDlg::SetCopyInfo(CopyInfo &Info)
{
	//not implemented
	return &Info;
}

BOOL CNewProgressDlg::MainFun()
{
	//if(m_bQueue)
	//	m_pThread->SuspendThread();
	BOOL bRet = ThreadFun();
	if(m_bQueue)
		CQueueDlg::GetInstance()->RemoveFromQueue(this);
	Lock();
	m_pThread = NULL;
	Unlock();

	for(int i = 0; i < m_aRefresh.GetSize(); i++)
	{
		if(::IsWindow(m_aRefresh[i]->GetSafeHwnd()))
		{
			m_aRefresh[i]->PostMessage(WM_REFRESH);
		}
	}
	KillTimer(1);
	KillTimer(2);
	KillTimer(PROGRESS_DIRNAME);
	KillTimer(PROGRESS_POS);
	KillTimer(PROGRESS_AND_NAME);

	Sleep(600);//wait to finish timer functions
	PostMessage(WM_MYCLOSE);
	///EndDialog(IDOK);
	return bRet;
}

BOOL CNewProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	m_bOperationStarted = TRUE;
	{
		AllocateWnd();
		
		InitWnd();

		FreeNotUsedWnd();
	}
	
	if(m_pWndCopied)		m_pWndCopied->SetWindowText(_T(""));
	if(m_pWndTime)			m_pWndTime->SetWindowText(_T(""));
	if(m_pWndTimeElapsed)	m_pWndTimeElapsed->SetWindowText(_T(""));
	if(m_pWndSpeed)			m_pWndSpeed->SetWindowText(_T(""));
	if(m_pWndStatus)		m_pWndStatus->SetWindowText(_T(""));
	if(m_pWndFileName)		m_pWndFileName->SetWindowText(_T(""));

	Lock();
	m_pThread = (CProgressThread *)AfxBeginThread(RUNTIME_CLASS( CProgressThread ), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED );
	if(m_pThread == NULL)
		TRACE(_T("--- mpthread:%X\n"), m_pThread);
	
	int a = sizeof(*m_pThread);
	int b = sizeof(CProgressThread);
	int c = sizeof(CWinThread);
	m_pThread->m_pWnd = this;
	//memset(m_pThread->a, 0, sizeof(m_pThread->a));
	if(!m_bQueue)
		m_pThread->ResumeThread();
	
	if(!m_bQueue) SetTimer(1, 3000,NULL);
	if(!m_bQueue) SetTimer(2, 1000,NULL);
	m_Time = CTime::GetCurrentTime();
	m_Timer.Start();

	_DLG_ITEM_LANGSTR(26,IDC_COPY_CANCEL);
	Unlock();


	return FALSE;
}

void CNewProgressDlg::Cancel()
{
	Lock();
	if(m_pThread)
		m_pThread->ResumeThread();
	Unlock();
	Sleep(10);
	if(m_pInfo)
	{
		m_pInfo->Lock();
		m_pInfo->AddFlags(ABORT,FALSE);
		m_pInfo->bCancel = TRUE;
		m_pInfo->Unlock();		
	}	
}

void CNewProgressDlg::AllocateWnd()
{
	m_pProgressOne = new CProgressCtrl;
	m_pProgress = new CProgressCtrl;

	m_pWndCopied	= new CWnd;
	m_pWndTime		= new CWnd;
	m_pWndTimeElapsed = new CWnd;
	m_pWndSpeed		= new CWnd;
	m_pWndStatus= new CWnd;
	m_pWndFileName	= new CWnd;
}

void CNewProgressDlg::FreeNotUsedWnd()
{
	if(m_pProgressOne && !::IsWindow(m_pProgressOne->GetSafeHwnd()))
	{
		delete m_pProgressOne;
		m_pProgressOne = NULL;
	}

	if(m_pProgress && !::IsWindow(m_pProgress->GetSafeHwnd()))
	{
		delete m_pProgress;
		m_pProgress = NULL;
	}

	if(m_pWndCopied && !::IsWindow(m_pWndCopied->GetSafeHwnd()))
	{
		delete m_pWndCopied;
		m_pWndCopied = NULL;
	}

	if(m_pWndTime && !::IsWindow(m_pWndTime->GetSafeHwnd()))
	{
		delete m_pWndTime;
		m_pWndTime = NULL;
	}
	if(m_pWndTimeElapsed && !::IsWindow(m_pWndTimeElapsed->GetSafeHwnd()))
	{
		delete m_pWndTimeElapsed;
		m_pWndTimeElapsed = NULL;
	}
	if(m_pWndSpeed && !::IsWindow(m_pWndSpeed->GetSafeHwnd()))
	{
		delete m_pWndSpeed;
		m_pWndSpeed = NULL;
	}
	if(m_pWndStatus && !::IsWindow(m_pWndStatus->GetSafeHwnd()))
	{
		delete m_pWndStatus;
		m_pWndStatus = NULL;
	}
	if(m_pWndFileName && !::IsWindow(m_pWndFileName->GetSafeHwnd()))
	{
		delete m_pWndFileName;
		m_pWndFileName = NULL;
	}
}

void CNewProgressDlg::OnTimer(UINT_PTR nIDEvent) 
{
	if(nIDEvent == 1 && m_pInfo)
	{
		CString sCopied;
		CString sTime;
		CString sSpeed;
		CopyInfo info;
		info = *m_pInfo;
		
		if(!info.IsCancelled() && info.nAllSize > 0)
		{
			info.nAllCopied += info.nFileCopied;
			int nPercent = (int)(info.nAllCopied * 10000/ info.nAllSize);
			CString sTmp;
			sTmp.Format(_T("%s %d%%"), info.sStatus, nPercent/100);
			SetWindowText(sTmp);
			CTime curtime = CTime::GetCurrentTime();
			if(nPercent>0)
			{
				CTimeSpan span =  curtime - info.time;
				LONGLONG sec = span.GetTotalSeconds();
				if(sec>0)
				{
					LONGLONG nTosec = sec*10000/nPercent;
					//CTimeSpan sRem(0,0,0,nTosec-sec);
					CTimeSpan sRem(nTosec-sec);
					
					sTime.Format(_T_LNGSTR(101), sRem.Format(_T("%H:%M:%S")));
					
					ULONGLONG nBS = info.nAllCopied/sec;
					sSpeed.Format(_T_LNGSTR(102), FormatSize(nBS, _T(" "), eAutoBytes));
					//
					m_Timer.Stop();
					double nSec = m_Timer.GetTime();
					if(nSec>0.9)
					{
						ULONGLONG nBSCur = info.nAllCopied - m_nCopied;
						nBSCur = (ULONGLONG)(nBSCur/nSec);
						
						CString sTmp;
						if(nBSCur>=0)
						{
							sTmp.Format(_T_LNGSTR(103), FormatSize(nBSCur, _T(" "), eAutoBytes));
							sSpeed += sTmp;
						}
						else
						{							
							CoolTrace(0, TRUE, _T("CNewProgressDlg::OnTimer nBSCur: %ld file: %s"), nBSCur, info.sNameTo);
						}						
					}
				}
				//			

			}
			TRACE(_T("PERCENT: %d\n"), nPercent);
			
			sCopied.Format(_T("%s/%s"),FormatSize(info.nAllCopied, _T(" "), eAutoBytes), FormatSize(info.nAllSize,_T(" "), eAutoBytes));

			if(m_pWndCopied)
				m_pWndCopied->SetWindowText(sCopied);
			if(m_pWndTime)
				m_pWndTime->SetWindowText(sTime);
			if(m_pWndSpeed)
				m_pWndSpeed->SetWindowText(sSpeed);

			m_Timer.Start();
			m_nCopied = info.nAllCopied;
		}		
		
	}
	else if(nIDEvent == 2 && m_pWndTimeElapsed)
	{
		m_pWndTimeElapsed->SetWindowText((CTime::GetCurrentTime()-m_Time).Format(_T("%H:%M:%S")));
	}
	else if(nIDEvent == PROGRESS_AND_NAME || nIDEvent == PROGRESS_DIRNAME || nIDEvent == PROGRESS_POS)
	{
		OnRefresh(nIDEvent, (LPARAM)m_pInfo);
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CNewProgressDlg::PreTranslateMessage(MSG* pMsg) 
{

	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			Cancel();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CNewProgressDlg::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
 	CDialog::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CNewProgressDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	return CDialog::OnCommand(wParam, lParam);
}

void CNewProgressDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if(nID == SC_CLOSE)
	{		
		Cancel();
	}
	CDialog::OnSysCommand(nID, lParam);
}

BOOL CNewProgressDlg::PauseCopy()
{	
	BOOL bPause = FALSE;
	if(m_pInfo)
	{
		m_pInfo->Lock();
		bPause = m_pInfo->bPause;
		m_pInfo->Unlock();
		if(!bPause)
			OnCopyPause();
	}
	
	return TRUE;
}

BOOL CNewProgressDlg::ResumeCopy()
{
	BOOL bPause = FALSE;
	if(m_pInfo)
	{
		m_pInfo->Lock();
		bPause = m_pInfo->bPause;
		m_pInfo->Unlock();
		if(bPause)
			OnCopyPause();		
	}
	Lock();
	if(m_pThread)
		m_pThread->ResumeThread();
	Unlock();
	
	return TRUE;
}



BEGIN_MESSAGE_MAP(CNewProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CCopyDlg)
	//ON_BN_CLICKED(IDC_COPY_CANCEL, OnCopyCancel)
	ON_WM_TIMER()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PROGRESS, OnRefresh)
	ON_MESSAGE(WM_MYCLOSE, OnMyClose)
END_MESSAGE_MAP()


LRESULT CNewProgressDlg::OnMyClose(WPARAM type, LPARAM value)
{
	DestroyWindow();
	return 0L;
}

LRESULT CNewProgressDlg::OnRefresh(WPARAM type, LPARAM lparam)
{
	if(m_pInfo==NULL || !IsWindow(GetSafeHwnd()))
		return 0;
	int value = (int)lparam;
	CopyInfo *	pInfo = (CopyInfo *) value;
	CString sText;
	int nNum = 0;

	switch(type)
	{
	case PROGRESS_RANGE:
		ASSERT(m_pProgress);
		m_pProgress->SetRange32(0, value);
		//TRACE(_T("PROGRESS_RANGE %d\n"), value);
		break;
	case PROGRESS_RANGE_ONE:
		m_pProgressOne->SetRange32(0, value);
		break;	
	case PROGRESS_FROM:
		m_pProgress->SetPos(value);
		//TRACE(_T("PROGRESS_FROM %d\n"), value);
		break;
	case PROGRESS_AND_NAME:
		pInfo->Lock();
		sText = pInfo->sNameFrom;
		nNum = pInfo->nCount;
		pInfo->Unlock();
		m_pWndFileName->SetWindowText(sText);
		m_pProgress->SetPos(nNum);
		if(m_nSize>1)
		{
			sText.Format(_T("Delete file: %d %%"), nNum*100/m_nSize);
			SetWindowText(sText);
		}
		break;
	case PROGRESS_STAT:
		if(m_pWndStatus)
		{
			pInfo->Lock();
			sText = pInfo->sStatus;
			pInfo->Unlock();
			m_pWndStatus->SetWindowText(sText);
		}
		break;
	case NAME_FROM:
		pInfo->Lock();
		sText = pInfo->sNameFrom;
		pInfo->Unlock();
		m_pWndFileName->SetWindowText(sText);
		break;
	case PROGRESS_NEW:
		if(m_pWndFileName || m_pProgress || m_pProgressOne)
		{
			BOOL bDisplay = FALSE;
			pInfo->Lock();
			if(m_sFilePrev != pInfo->sNameFrom)
			{
				m_sFilePrev = pInfo->sNameFrom;
				bDisplay = TRUE;
			}
			if(m_nWsp>0)
				nNum = (int)((pInfo->nAllCopied + pInfo->nFileCopied)/m_nWsp);
			else
				nNum = (int)(pInfo->nAllCopied + pInfo->nFileCopied);
			pInfo->Unlock();

			if(m_pWndFileName && bDisplay)
			{
				m_pWndFileName->SetWindowText(m_sFilePrev);				
			}

			if(m_pProgress)
				m_pProgress->SetPos(nNum);
			if(m_pProgressOne)
				m_pProgressOne->SetPos(0);			
		}
		break;
	case PROGRESS_POS:
		if(m_pProgressOne || m_pProgress || m_pWndFileName)
		{
			BOOL bDisplay = FALSE;
			pInfo->Lock();
			int pos = 0;
			if(m_nWsp>1)
				pos = (int)((pInfo->nAllCopied + pInfo->nFileCopied) / m_nWsp);
			else
				pos = (int)(pInfo->nAllCopied + pInfo->nFileCopied);
			ULONGLONG nFileSize = pInfo->nFileSize;
			ULONGLONG nFileCopied = pInfo->nFileCopied;
			if(m_sFilePrev != pInfo->sNameFrom)
			{
				m_sFilePrev = pInfo->sNameFrom;
				bDisplay = TRUE;
			}
			pInfo->Unlock();

			
			if(m_pProgress)
				m_pProgress->SetPos(pos);
			if(m_pProgressOne)
			{
				if(m_nSize != nFileSize)
				{
					if(nFileSize>0 && nFileCopied>0)
						pos = (int)(nFileCopied*10000/nFileSize);
					else
						pos = 0;
				}
				m_pProgressOne->SetPos(pos);
			}
			if(m_pWndFileName && bDisplay)
			{
				m_pWndFileName->SetWindowText(m_sFilePrev);				
			}
		}
		break;
	case PROGRESS_DIRNAME:
		if(m_pWndFileName)
		{
			BOOL bDisplay = FALSE;
			pInfo->Lock();
			if(m_sFilePrev != pInfo->sDirName)
			{
				m_sFilePrev = pInfo->sDirName;
				bDisplay = TRUE;
			}
			pInfo->Unlock();

			if(bDisplay)
			{
				m_pWndFileName->SetWindowText(m_sFilePrev);
			}
		}
		break;

	default: ASSERT(0); break;
	}
//	delete sFileName;
	return 0;
}

void CNewProgressDlg::OnCopyPause()
{
	BOOL bPause = FALSE;
	if(m_pInfo)
	{
		m_pInfo->Lock();
		bPause = m_pInfo->bPause = !m_pInfo->bPause;
		m_pInfo->Unlock();
	}
	
	_DLG_ITEM_LANGSTR(bPause ? 177 : 176,IDC_COPY_PAUSE);
	Lock();
	if(m_pThread)
	{
		if(bPause)
			m_pThread->SuspendThread();
		else
			m_pThread->ResumeThread();
	}
	Unlock();
}

////////////
////////////
///////////////
//LPCTSTR sPath, CStringList *plFiles, CWnd* pParent /*=NULL*/, CWnd *pRefresh)
//LPCTSTR sPath, LPCTSTR sPathTo, CStringList *plFiles, CWnd* pParent, CWnd* pSecWnd
//LPCTSTR sPath, LPCTSTR sPathTo, CStringList *plFiles, CWnd* pParent, CWnd* pSecWnd

CCopyDlg::CCopyDlg(LPCTSTR sPath, LPCTSTR sPathTo, CStringList *plFiles, CWnd* pParent /*=NULL*/, CWnd* pSecWnd, LPCTSTR sMes, BOOL bMove, int nFiles, int nDirs)
	: CNewProgressDlg(CCopyDlg::IDD, pParent, sPath, sPathTo, plFiles, pSecWnd)
{
	//{{AFX_DATA_INIT(CCopyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sMes = sMes;

	m_bMove = bMove;
	m_nFiles = nFiles;
	m_nDirs = nDirs;
	m_bSmartChangeName = FALSE;
//	if(m_sPathTo[m_sPathTo.GetLength()-1] != '\\')
//		m_sPathTo += '\\';
	Create(CCopyDlg::IDD);
	if( ::IsWindow( m_hWnd ) && !m_bQueue)
	{
		CenterWindow(AfxGetMainWnd());
		ShowWindow(SW_SHOW);
	}
	//PostMessage(WM_CLOSE);
}
CCopyDlg::~CCopyDlg()
{
//	delete this;
}


void CCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CNewProgressDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyDlg)
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CCopyDlg, CNewProgressDlg)
	//{{AFX_MSG_MAP(CCopyDlg)
	ON_BN_CLICKED(IDC_COPY_CANCEL, OnCopyCancel)
	ON_BN_CLICKED(IDC_COPY_QUEUE, OnCopyQueue)
	ON_BN_CLICKED(IDC_COPY_PAUSE, OnCopyPause)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_PROGRESS, OnRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyDlg message handlers

BOOL CCopyDlg::OnInitDialog() 
{	
	CString sTmp;
	if(m_bMove)
		sTmp.Format(_T_LNGSTR(93), m_sMes, m_sPath);
	else
		sTmp.Format(_T_LNGSTR(94), m_sMes, m_sPath);

	if(m_nFiles == 1 && m_nDirs == 0)
		m_sRename = m_lstrFiles.GetHead();
	if(_tcschr(m_sRename, '\\'))
	{
		CString sDir, sFile;
		GetDirFile(sDir, sFile, m_sRename, '\\');
		m_sRename = sFile;
	}
	//else
	//	m_sRename = _T("*.*");

	CSimpleEdit seDlg(sTmp, m_sPathTo, AfxGetMainWnd(), TRUE, FALSE, m_bMove ? IDC_LINE2 : IDC_LINE3 , m_bMove, m_sRename, TRUE, !(m_nFiles == 1 && m_nDirs == 0));
	if(seDlg.DoModal() == IDOK)
	{
		m_bQueue = seDlg.m_bQueue;
		m_sPathTo = seDlg.GetText();
		m_sMask = seDlg.m_sMask;
		m_bNotCopyEmpty = seDlg.CanNotCopyEmptyDir();
		m_sRename = seDlg.m_sRename;
		
		if(seDlg.m_bExtDirName)
		{
			m_InfoSmart.Init(m_sPath, seDlg.m_sDestFileName, m_sPathTo);
			m_bSmartChangeName = TRUE;
		}

		if(seDlg.CanWithBuffer())
		{
			CopyThreadInfo *info = new CopyThreadInfo;
			info->sFrom = m_sPath;
			info->sTo = m_sPathTo;
			info->lNames.AddTail(&m_lstrFiles);
			info->sMask = m_sMask;
			CProgressDlg dlg(100, _T("Kopiowanie"), info, CProgressDlg::copy);
			dlg.DoModal();
			PostMessage(WM_MYCLOSE);
			return FALSE;
		}
	
		if(m_sPath.Right(1) == '\\' && m_sPath.GetLength() > 3)
			m_sPath = m_sPath.Left(m_sPath.GetLength() - 1);

	}
	else
	{
		PostMessage(WM_MYCLOSE);
		return FALSE;		
	}


	if(m_sMask.IsEmpty())
		m_sMask = _T("*");

	if(m_bMove && !m_bSmartChangeName && m_sRename.IsEmpty())
	{
		if(TryFastMove())
		{
			for(int i = 0; i < m_aRefresh.GetSize(); i++)
			{
				if(::IsWindow(m_aRefresh[i]->GetSafeHwnd()))
					m_aRefresh[i]->PostMessage(WM_REFRESH);
			}
			PostMessage(WM_MYCLOSE);
			return FALSE;
		}
	}
	

	CNewProgressDlg::OnInitDialog();	
	if(m_bQueue)
	{
		OnCopyQueue();
	}
	_DLG_ITEM_LANGSTR(176,IDC_COPY_PAUSE);
	_DLG_ITEM_LANGSTR(178,IDC_COPY_QUEUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCopyDlg::ThreadFun()
{
	//CString *pStr = new CString();

	if(m_sMask.IsEmpty())
		m_sMask = _T("*");

	//CopyInfo info;// = m_Info;
	m_pInfo->Lock();
	m_pInfo->Clear();
	m_pInfo->pWnd = this;
	m_pInfo->bMove = m_bMove;
	m_pInfo->sStatus = _T_LNGSTR(98);
	m_pInfo->sDestMod = m_sRename;
	m_pInfo->bQueue = m_bQueue;
	if(m_InfoSmart.IsNeeded())
		m_pInfo->m_pinfoSmart = &m_InfoSmart;

	m_pInfo->Unlock();
	//m_pInfo = &info;
	PostMessage(WM_PROGRESS, PROGRESS_STAT, (LPARAM)m_pInfo);
	
	m_pInfo->bNotCopyEmpty = m_bNotCopyEmpty;

	if(!m_bQueue) SetTimer(PROGRESS_DIRNAME, 50, NULL);
	m_nSize = m_DirTree.Init(m_sPath, m_sMask, m_lstrFiles, m_pInfo);
	KillTimer(PROGRESS_DIRNAME);
	
	m_pInfo->Lock();
	m_pInfo->nAllSize = m_nSize;
	if(m_bMove)
		m_pInfo->sStatus = _T_LNGSTR(95);
	else
		m_pInfo->sStatus = _T_LNGSTR(96);
	BOOL bCancel = m_pInfo->IsCancelled(FALSE);;
	m_pInfo->time = CTime::GetCurrentTime();
	m_pInfo->Unlock();

	if(bCancel)
	{
		AfxMessageBox(_T_LNGSTR(97));
		m_pInfo = NULL;
		return FALSE;
	}

	if(m_nSize > MAXSHORT)
		m_nWsp = (UINT)(m_nSize / MAXSHORT + 2);
	else
		m_nWsp = 1;

	PostMessage(WM_PROGRESS, PROGRESS_RANGE, (LPARAM)(m_nSize / m_nWsp));
	
	if(m_DirTree.Count()==1)
		PostMessage(WM_PROGRESS, PROGRESS_RANGE_ONE, (LPARAM)(m_nSize / m_nWsp));		
	else
		PostMessage(WM_PROGRESS, PROGRESS_RANGE_ONE, 10000);		
		
	PostMessage(WM_PROGRESS, PROGRESS_STAT, (LPARAM)m_pInfo);
	
	if(!m_bQueue) SetTimer(PROGRESS_POS, 50, NULL);
	m_DirTree.Copy(_T(""), m_sPathTo, m_pInfo);
	KillTimer(PROGRESS_POS);

	if(m_pInfo->bCancel)
	{
		AfxMessageBox(_T_LNGSTR(97));
		m_pInfo = NULL;
		return FALSE;
	}

	m_pInfo = NULL;
	return TRUE;
}

BOOL CCopyDlg::DestroyWindow() 
{
	BOOL bRes = CNewProgressDlg::DestroyWindow();
	return bRes;
}

void CCopyDlg::PostNcDestroy() 
{
	CNewProgressDlg::PostNcDestroy();
//	delete this;
}

void CCopyDlg::Cancel()
{
	CNewProgressDlg::Cancel();
}

void CCopyDlg::OnCopyCancel()
{
	Cancel();
}

void CCopyDlg::OnCopyQueue()
{
	CQueueDlg::GetInstance()->AddToQueue(this);
	ShowWindow(SW_HIDE);

	KillTimer(1);
	KillTimer(2);
	KillTimer(PROGRESS_POS);
	KillTimer(PROGRESS_DIRNAME);
	KillTimer(PROGRESS_AND_NAME);
}

BOOL CCopyDlg::TryFastMove()
{
	if(m_bMove && MatchAll(m_sMask))
	{
		POSITION pos = m_lstrFiles.GetHeadPosition();
		POSITION pos_old = NULL;
		CString s;
		while(pos)
		{
			pos_old = pos;
			s = m_lstrFiles.GetNext(pos);
			BOOL bRes = MoveFileEx(AddPathNames(m_sPath, s), AddPathNames(m_sPathTo,s) , MOVEFILE_WRITE_THROUGH);
			
			if(bRes)
				m_lstrFiles.RemoveAt(pos_old);
			//else
			//	ErrorMessage(AddPathNames(m_sPath, s) + " \n" + m_sPathTo);			
		}
		
		if(m_lstrFiles.IsEmpty())
			return TRUE;
	}
	return FALSE;
}


BOOL CCopyDlg::InitWnd()
{
	CDataExchange dx(this, FALSE);
	DDX_Control(&dx, IDC_COPY_PROGRESS_ONE, *m_pProgressOne);
	DDX_Control(&dx, IDC_COPY_PROGRESS, *m_pProgress);
	DDX_Control(&dx, IDC_COPY_BYTES_COPIED, *m_pWndCopied);
	DDX_Control(&dx, IDC_COPY_TIME, *m_pWndTime);
	DDX_Control(&dx, IDC_COPY_TIME_ELAPSED, *m_pWndTimeElapsed);
	DDX_Control(&dx, IDC_COPY_SPEED, *m_pWndSpeed);
	DDX_Control(&dx, IDC_COPY_STATUS, *m_pWndStatus);
	DDX_Control(&dx, IDC_COPY_FILENAME, *m_pWndFileName);

	return TRUE;
}

