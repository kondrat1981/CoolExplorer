// FilesListFtp.cpp: implementation of the CFilesListFtp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListData.h"
#include "FilesListFtp.h"
#include "MyFtpFile.h"
#include "FilesListManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilesListFtp::CFilesListFtp(CFilesListManager *pManager) : CFilesListBase(pManager)
{
	dir_separator = '/';
	m_pInetSession = NULL;
	m_pFtpStatusBar = NULL;
	m_pFtpConnection = NULL;
	m_FtpProgressInfo.Clear();
	m_mode = Ftp;
}

CFilesListFtp::~CFilesListFtp()
{
	CSingleLock lock(&m_mutexFTP);
	lock.Lock();
	ASSERT(lock.IsLocked());

	if (m_pFtpConnection != NULL)
	{
		m_pFtpConnection->Close();
		delete m_pFtpConnection;
	}
	if (m_pInetSession != NULL)
	{
		m_pInetSession->Close();
		delete m_pInetSession;
	}
}

BOOL CFilesListFtp::EnumFiles(LPCTSTR sPath, int nJobNum)
{
	BOOL bRet = FtpEnumFiles(sPath, nJobNum);

	return bRet;
}

BOOL CFilesListFtp::Select(const ListData *pld, BOOL &bBrowseItem, int nJob)
{
	ASSERT(pld);
	BOOL bRet = FALSE;
	CString sNewDir;
	bBrowseItem = TRUE;

	LockList();
	const CString sName = pld->getName();
	const CString sGetDir = GetDir();
	if(sName == RET_NAME)
	{
		CString sOldDir = m_sPath.Right(m_sPath.GetLength() - m_sPath.Left(m_sPath.GetLength() - 1).ReverseFind(dir_separator)-1);
		sOldDir = sOldDir.Left(sOldDir.GetLength()-1);
		sNewDir = m_sPath.Left(m_sPath.Left(m_sPath.GetLength() - 1).ReverseFind(dir_separator)+1);
	}
	else if(pld->isDir())
	{
		sNewDir = AddPathNames(sGetDir, sName, dir_separator);
	}
	else
	{
		bBrowseItem = FALSE;
	}
	UnlockList();

	if(bBrowseItem)
	{
		bRet = FtpEnumFiles(sNewDir, nJob);
	}
	else
	{
		CString sFileToView = sGetDir + sName;
		CMyFtpFile file(m_pFtpConnection);
		CString sTmp;
		if(!file.GetFileToTemp(sFileToView, sTmp,pld->getSize()))
		{
			AfxMessageBox(_T("Error reading ftp file"));
		}
		else
		{
			CString sDir,sFile;
			GetDirFile(sDir, sFile,sTmp);
			
			CheckAndReconnect();
			
			SHELLEXECUTEINFO shi={0};
			
			shi.cbSize = sizeof (SHELLEXECUTEINFO);
			shi.fMask = SEE_MASK_FLAG_NO_UI;
			shi.hwnd = GetParent(); 
			shi.lpVerb = NULL; 
			shi.lpFile = sFile;
			shi.lpParameters = NULL;
			shi.lpDirectory = sDir;
			shi.nShow = SW_SHOWNORMAL;

			if(!ShellExecuteEx( &shi))
			{
				CString str;
				str.Format(_T("CNewFilesList::Select FTP file:%s"), sTmp);
				ErrorMessage(str, FALSE);
			}
			bRet = TRUE;
		}
	}

	return bRet;
}

void CFilesListFtp::FtpDisconect(BOOL bHideStatus)
{
	CSingleLock lock(&m_mutexFTP);
	lock.Lock();
	ASSERT(lock.IsLocked());

	FreeList();
	TRACE(_T("CFilesListFtp::FtpDisconect\n"));
	if (m_pFtpConnection != NULL)
	{
		m_pFtpConnection->Close();
		m_sPath = _T("");
	}
	delete m_pFtpConnection;
	m_pFtpConnection = NULL;
	
	if(bHideStatus && ::IsWindow(m_pFtpStatusBar->GetSafeHwnd()) && ::IsWindow(GetParent()))
	{
		m_pFtpStatusBar->ShowWindow(SW_HIDE);
		m_pFtpStatusBar->GetParentFrame()->RecalcLayout();
	}
}

void CFilesListFtp::SetFtpStatusBar(CWnd *pFtpStatusBar)
{
	m_pFtpStatusBar = pFtpStatusBar;
	m_FtpProgressInfo.pWnd = pFtpStatusBar;
}


BOOL CFilesListFtp::IsStillConnected()
{

	CSingleLock lock(&m_mutexFTP);
	lock.Lock();
	ASSERT(lock.IsLocked());

	if (!m_pFtpConnection || !(HINTERNET)*m_pFtpConnection)
		return FALSE;

	CString strDirName;
	if (m_pFtpConnection->GetCurrentDirectory(strDirName))
	{
//		GetTraceView()->AddTraceLine(0, "[%d] FTP status: Connected.", AfxGetThread()->m_nThreadID);
		return TRUE;
	}

	// close connection
	m_pFtpConnection->Close();
	delete m_pFtpConnection;
	m_pFtpConnection = NULL;

//	GetTraceView()->AddTraceLine(0, "[%d] FTP status: Connection has been closed.", AfxGetThread()->m_nThreadID);
	return FALSE;
}



BOOL CFilesListFtp::CheckAndReconnect()
{
	CSingleLock lock(&m_mutexFTP);
	lock.Lock();
	ASSERT(lock.IsLocked());

	BOOL bRet = FALSE;

	bRet = IsStillConnected();
	if(!bRet)
	{
		if(m_pFtpConnection)
		{
			try
			{
				m_pFtpConnection->Close();
				delete m_pFtpConnection;
				m_pFtpConnection = NULL;
			}
			catch (CInternetException* pEx)
			{
				pEx->Delete();
			}
		}

		m_pFtpConnection = DuplicateFtpConnection();
		if(m_pFtpConnection==NULL)
		{
			AfxMessageBox(_T("Error reconnect to ftp server"));
			bRet = FALSE;
		}
	}

	return bRet;
}


CFtpConnection * CFilesListFtp::DuplicateFtpConnection()
{
	CSingleLock lock(&m_mutexFTP);
	lock.Lock();
	ASSERT(lock.IsLocked());

	CFtpConnection * pNewCon = NULL;
	try
	{			
		pNewCon = m_pInetSession->GetFtpConnection(m_FtpSet.m_sServer, m_FtpSet.m_sUser, m_FtpSet.m_sPassword, m_FtpSet.m_nPort, m_FtpSet.m_bPassive );
		InternetSetStatusCallback(*pNewCon, MyFtpCallBack);		
	}
	catch (CInternetException* pEx)
	{
		// catch errors from WinINet
		TCHAR szErr[2048];
		if (pEx->GetErrorMessage(szErr, sizeof(szErr)))
			CoolTrace(0, TRUE, _T("CFilesListFtp::DuplicateFtpConnection %s"), szErr);
		else
			CoolTrace(0, TRUE, _T("CFilesListFtp::DuplicateFtpConnection exception too long"));
		pEx->Delete();
		
		pNewCon = NULL;		
	}
	return pNewCon;
}

BOOL CFilesListFtp::InitInternetConection(const CFtpConSet * pFtpSet, CWnd* pStatusbar)
{

	CSingleLock lock(&m_mutexFTP);
	lock.Lock();
	ASSERT(lock.IsLocked());

	SetFtpStatusBar(pStatusbar);
	m_FtpSet = *pFtpSet;
	// na razie nie zwalniam m_pShellFolder oraz m_pidlList aby latwo mozna bylo powrocic do stanu poprzedniego
	// po zamknieciu ftp
	
	
	
	CString strObject;
	DWORD dwServiceType;
	if(::IsWindow(m_pFtpStatusBar->GetSafeHwnd()))
	{
		m_pFtpStatusBar->ShowWindow(SW_SHOW);
		m_pFtpStatusBar->GetParentFrame()->RecalcLayout();
	}

	//FreeList();
	//FtpDisconect();

	CString sError;
	sError.Format(_T("Try connect to:%s\nUser:%s\n"), m_FtpSet.m_sServer, m_FtpSet.m_sUser);

	if(!m_pInetSession)
	{
		CString str;
		if (!str.LoadString(AFX_IDS_APP_TITLE))
			str = _T("AppUnknown");
		//INTERNET_FLAG_NO_CACHE_WRITE
		m_pInetSession = new CInternetSession(str, (DWORD)&m_FtpProgressInfo, PRE_CONFIG_INTERNET_ACCESS, NULL, NULL, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_NO_CACHE_WRITE );
	}

	// Alert the user if the internet session could
	// not be started and close app
	if (!m_pInetSession)
	{
		AfxMessageBox(sError + _T("Cannot open inet conection"), MB_OK|MB_ICONERROR);
		return FALSE;
	}

	// check to see if this is a reasonable URL --
	// ie "ftp://servername/dirs" or just "servername/dirs"

	{
		CString strServerName;
		unsigned short nPort = 0;
		if (!AfxParseURL(m_FtpSet.m_sServer, dwServiceType, strServerName, strObject, nPort))
		{
			// try adding the "ftp://" protocol
			CString strFtpURL = _T("ftp://");
			strFtpURL += m_FtpSet.m_sServer;
			
			if (!AfxParseURL(strFtpURL, dwServiceType, strServerName, strObject, m_FtpSet.m_nPort))
			{
				AfxMessageBox(sError+_T("Invalid URL"), MB_OK|MB_ICONERROR);
				return FALSE;
			}
		}
		m_FtpSet.m_sServer = strServerName;
		if(m_FtpSet.m_nPort == 0)
			m_FtpSet.m_nPort = nPort;
	}

	CWaitCursor cursor; // this will automatically display a wait cursor

	// replace the MRU URL in the combo's edit box

	// Now open an annonymous FTP connection to the server
	if ((dwServiceType == INTERNET_SERVICE_FTP) && !m_FtpSet.m_sServer.IsEmpty())
	{
		try
		{
			m_pFtpConnection = m_pInetSession->GetFtpConnection(m_FtpSet.m_sServer, m_FtpSet.m_sUser, m_FtpSet.m_sPassword, m_FtpSet.m_nPort, m_FtpSet.m_bPassive );
			InternetSetStatusCallback(*m_pFtpConnection, MyFtpCallBack);
		}
		catch (CInternetException* pEx)
		{
			// catch errors from WinINet
			TCHAR szErr[2048];
			if (pEx->GetErrorMessage(szErr, sizeof(szErr)))
				AfxMessageBox(sError+szErr, MB_OK | MB_ICONERROR);
			else
				AfxMessageBox(sError+_T("exception too long"), MB_OK);
			pEx->Delete();

			m_pFtpConnection = NULL;
			return FALSE;
		}
		dir_separator = '/';
		CString sDir;
		m_sPath = _T("");
		
		sDir = m_FtpSet.m_sRemoteDir;

		if (!sDir.IsEmpty())
		{
			if (!m_pFtpConnection->SetCurrentDirectory(sDir))
			{
				m_pFtpConnection->GetCurrentDirectory(sDir);
			}
		}
		else
		{
			m_pFtpConnection->GetCurrentDirectory(sDir);
		}


		EnumFiles(sDir, JOBNUM_NONE);
	}
	else
	{
		AfxMessageBox(sError+_T("Invalid URL"), MB_OK|MB_ICONERROR);
		return FALSE;
	}
	TRACE(_T("CFilesListFtp::InitInternetConection OK %s\n"), m_FtpSet.m_sServer);	
	return TRUE;
}


BOOL CFilesListFtp::FtpEnumFiles(LPCTSTR sNewDir, int nJob)
{
	CSingleLock lock(&m_mutexFTP);
	lock.Lock();
	ASSERT(lock.IsLocked());
	CheckAndReconnect();
	BOOL bEmptyDirExist = FALSE;
	BOOL bFindFile = FALSE;

	CString sOldDir;
	ListData ld;
	CFtpFileFind ff(m_pFtpConnection);


	
	if(ff.FindFile(sNewDir) == FALSE)
	{
		bEmptyDirExist = m_pFtpConnection->SetCurrentDirectory(sNewDir);
		
		if(!bEmptyDirExist)
		{	
			ErrorMessage(CString(_T("Path: ")) + sNewDir + _T(" is incorect."));
			return FALSE;			
		}
	}
	else
	{
		bFindFile = TRUE;
	}

	LockList();
	FreeList();

	m_sPath = sNewDir;
	if(m_sPath[m_sPath.GetLength()-1] != dir_separator)
		m_sPath += dir_separator;
	
	AddRetDir(m_sPath);
	UnlockList();

	// skoro katalog istnieje


	if(!bEmptyDirExist)
	{
		BOOL bContinue = ff.FindFile(m_sPath+_T("*.*"));
		if(FALSE == bContinue && FALSE == bFindFile)
		{
			AfxMessageBox((CString)"In directory:" + m_sPath + "possible isn't any file");
			//ErrorMessage();
			ff.Close();
			return FALSE;
		}
		while (m_pManager->CanContinue(nJob) && bContinue)
		{
			bContinue = ff.FindNextFile();
			
			//if(m_pWndToUpdate)
			//	m_pWndToUpdate->PostMessage(WM_PROGRESS, (LPARAM)new CString(ff.GetFileName()));
			
			if(!ff.IsDots())
			{
				ld.Clear();
				ld.setFileInfo(FileInfo(ff.GetFileName(), &ff));
				if(CanAdd(&ld))
					AddTail(ld);				
			}		
		}
		ff.Close();
	}

	
	return TRUE;
}

BOOL CFilesListFtp::CreateDirectory(LPCTSTR sDir)
{
	BOOL bRet = FALSE;
	
	CSingleLock lock(&m_mutexFTP);
	lock.Lock();
	ASSERT(lock.IsLocked());
	
	ASSERT(m_pFtpConnection != NULL);
	if(m_pFtpConnection != NULL)
	{
		bRet = m_pFtpConnection->CreateDirectory(GetDir() + sDir);
	}
	
	return bRet;
}
