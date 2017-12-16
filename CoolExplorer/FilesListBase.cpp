// FilesListBase.cpp: implementation of the CFilesListBase class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "FilesListBase.h"
#include "FilesListManager.h"
#include "MyTimer.h"
#include "Shell.h"
#include "ItemsStorageShell.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilesListBase::CFilesListBase(CFilesListManager *pManager) : m_pManager(pManager)
{
	dir_separator='\\';
	m_mode = NoneMode;

	CSettings cfg;
	m_bSilenceMode = FALSE;
}

CFilesListBase::~CFilesListBase()
{
//	LockList();
//	m_bTerminateThread = TRUE;
//	m_nKey++;
//	UnlockList();
//	EndReadAddInfoFunction();
//	if(m_pThread)
//	{
//		m_pThread->ResumeThread();
//		DWORD dw = 0;
//		do
//		{
//			m_pThread->ResumeThread();
//			Sleep(100);
//			GetExitCodeThread(m_pThread->m_hThread, &dw);
//		}
//		while(dw == STILL_ACTIVE);
//		delete m_pThread;
//		m_pThread = NULL;
//	}
}

BOOL CFilesListBase::CanAdd(const ListData *ld)
{
	if(ld->isFile())
	{
		if(!m_pManager->GetShowFilter().IsEmpty())
			if(!Compare(m_pManager->GetShowFilter(), ld->getName()))
				return FALSE;
			
			if(!m_pManager->GetHideFilter().IsEmpty())
				if(Compare(m_pManager->GetHideFilter(), ld->getName()))
					return FALSE;
	}
	
	return TRUE;
}


/*BOOL CFilesListBase::IsDir(const ListData * pld) const
{
	ULONG flags = SFGAO_FOLDER ;
	if(pld->getName() == RET_NAME)
		return TRUE;
	if(pld->getAttrib() & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;
	return FALSE;
}*/

void CFilesListBase::AddRetDir(LPCTSTR sCurDir)
{
	size_t len = _tcslen(sCurDir);
	//if(/*len > 2 &&*/ !m_bMinInf)
	{
		if(!((len==3 || len ==2) && sCurDir[1]==':') || CSettings::m_settings.bEnableDotsInRoot)
		{
			ListData ld(RET_NAME, RETURN_ICONINDEX);
			ld.setExactIcon(RETURN_ICONINDEX);
			AddHead(ld);
		}
	}

}

void CFilesListBase::FreeList()
{
	LockList();	
	m_lFiles.RemoveAll();
	UnlockList();
}


BOOL CFilesListBase::IsBrowseableByNet(LPCTSTR sPath)
{
	BOOL bRet = FALSE;
	if(!_tcsncmp(sPath, _T("\\\\"), 2))
	{
		if(_tcslen(sPath) == 2)
			bRet = TRUE;
		else
		{
			sPath += 2;
			sPath = _tcschr(sPath, '\\');
			if(sPath == NULL || _tcslen(sPath) <= 1)
				bRet = TRUE;			
		}
	}
	return bRet;
}

BOOL CFilesListBase::CanUseFindFirstFile(LPCTSTR sPath)
{
	WIN32_FIND_DATA fd={0};
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(AddPathNames(sPath,_T("*.*")), &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return TRUE;
	}
	
	HANDLE hDir = CreateFile(sPath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if(hDir != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hDir);
		return TRUE;
	}
	return FALSE;
}


BOOL CFilesListBase::IsBrowseableByNormal(LPCTSTR sParPath)
{
	LPCTSTR sPath = sParPath;
	if(sPath[0] == '/' || IsBrowseableByVirtual(sPath))
		return FALSE;
	if(!_tcsncmp(sPath,_T("\\\\"), 2))
	{
		sPath += 2;
		sPath = _tcschr(sPath, '\\');
		if(!sPath || *(sPath+1)=='\0')
		{
			return FALSE;
		}
		return CanUseFindFirstFile(sParPath);
	}
	else if(!_tcsncmp(sPath+1, _T(":\\"), 2))
	{
		return CanUseFindFirstFile(sParPath);
	}
	return FALSE;
}




BOOL CFilesListBase::IsBrowseableByFtp(LPCTSTR sPath)
{
	if(!_tcsncmp(sPath,_T("/"), 1))
	{		
		return TRUE;
	}
	return FALSE;
}

BOOL CFilesListBase::IsBrowseableByCOM(LPCTSTR sPath)
{
	if(!_tcsncmp(sPath,_T("/"), 1) || !_tcscmp(sPath, _T("\\\\")) || IsBrowseableByVirtual(sPath))
		return FALSE;
	return TRUE;
}

BOOL CFilesListBase::IsBrowseableByVirtual(LPCTSTR sPath)
{
	if(!_tcsncmp(sPath, VIRT_NAME, _tcslen(VIRT_NAME)))
		return TRUE;
	return FALSE;
}


BOOL CFilesListBase::DoCommand(LPCTSTR sCommand, BOOL bFromSelect)
{
	if(/*AreFilesOperationsAvailable() && */!_tcsncmp(sCommand, _T("cd "),3))
	{
		sCommand += 3;
		//while(*sCommand == ' ')
		//	sCommand++;
		ListData *pld = SearchInCurrentDir(sCommand);
		BOOL bRet = TRUE;
		BOOL bBrowsableItem = TRUE;
		if(pld)
			bRet = m_pManager->Select(pld, m_pManager->GetCurJobNum());
		else
			bRet = m_pManager->SetDir(sCommand);
		return TRUE;
	}
	TCHAR sCurDir[MAX_PATH];
	GetCurrentDirectory(sizeof(sCurDir), sCurDir);
	SetCurrentDirectory(GetDir());

	BOOL bRet = FALSE;

	CString sProgram = sCommand;
	CString sParameters;
	int pos;
	if(( pos = sProgram.Find(' ')) != -1)
	{
		sProgram = sProgram.Left(pos);
		sParameters = sCommand + pos + 1;
	}

	CString str = GetDir();
	SHELLEXECUTEINFO shi={0};
	//memset(&shi,0,sizeof(shi));
		shi.cbSize = sizeof (SHELLEXECUTEINFO); 
		shi.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_FLAG_DDEWAIT; 
		shi.hwnd = GetParent(); 
		shi.lpVerb = NULL; 
		shi.lpFile = sProgram; 
		shi.lpParameters = sParameters; 
		shi.lpDirectory = str; 
		shi.nShow = SW_SHOWNORMAL; 
    
	if(!ShellExecuteEx( &shi))
	{
		shi.lpParameters = NULL;
		shi.lpFile = sCommand;
		if(!ShellExecuteEx( &shi) && !bFromSelect)
		{
			//CString sCom = "command.com /K ";
			//sCom += sCommand;
			if(_tsystem(sCommand) )
				AfxMessageBox(_T("Run Command Error"));
			else
				bRet = TRUE;
		}
	}
	else
		bRet = TRUE;


	//execl(sCommand, " ",NULL);//, SW_SHOWNORMAL);
	SetCurrentDirectory(sCurDir);
	return bRet;
}


INT_PTR CFilesListBase::GetAllCount() const
{
	return m_lFiles.GetCount();
}

POSITION CFilesListBase::GetHeadPosition() const
{
	return m_lFiles.GetHeadPosition();
}

ListData * CFilesListBase::GetNext(POSITION & pos)
{
	return &m_lFiles.GetNext(pos);
}

CString CFilesListBase::GetParentName(CString *psParentParentName) const
{
	if(psParentParentName)
		*psParentParentName = _T("");
	CString sParent = GetFileFullPath(NULL);
	if(sParent.IsEmpty() == FALSE)
	{
		if(sParent.GetLength() <= 3)
		{
			return sParent;
		}
		else
		{
			while(sParent.Right(1) == dir_separator)
				sParent = sParent.Left(sParent.GetLength() - 1);
			int pos = sParent.ReverseFind(dir_separator);
			if(pos != -1)
			{
				if(psParentParentName)
				{
					*psParentParentName = sParent.Mid(0, pos);
					if(psParentParentName->GetLength() == 2 && psParentParentName->GetAt(1) == ':')
						*psParentParentName += dir_separator;
				}
				sParent = sParent.Right(sParent.GetLength() - pos - 1);
				
				return sParent;
			}
		}
	}
//	else if(m_pidlList)
//		return this->GetDirForAdressBar();
	return _T("");
}


BOOL CFilesListBase::CreateDirectory(LPCTSTR sDir)
{
	BOOL bRet = FALSE;
	
	return bRet;
}


ListData *CFilesListBase::SearchInCurrentDir(LPCTSTR sHelpCom)
{
	ListData *pData = NULL;
	CSingleLock lock(&m_lFiles.GetSynObject());
	lock.Lock();
	POSITION pos = GetHeadPosition();
	while(pos)
	{
		pData = &m_lFiles.GetNext(pos);
		if(pData->getName() == sHelpCom)
		{

			return pData;
		}
	}

	return NULL;
}

CString CFilesListBase::GetFileFullPath(const ListData * pdl) const
{
	if(pdl)
	{
		ASSERT(pdl->getName() != RET_NAME);
		if(pdl->getName() == RET_NAME)
			return _T("");
		return GetDir() + pdl->getName();
	}
	
	return GetDir();	
}

CString CFilesListBase::GetFileFullPath(const CString &sFile) const
{
	return AddPathNames(GetFileFullPath(NULL), sFile);
}

HWND CFilesListBase::GetParent() const
{
	return m_pManager->GetParent();
}

LONGLONG CFilesListBase::CountDirSize(LPCTSTR sPath, int nJob) const
{
	ASSERT(_tcslen(sPath)>0);
	if(_tcslen(sPath)==0)
		return 0;

	LARGE_INTEGER tmp={0};
	ULONGLONG lSize = 0;

	//BOOL bRet = FALSE;
	WIN32_FIND_DATA fd;
	memset(&fd, 0, sizeof(fd));
	HANDLE hFind = FindFirstFile(AddPathNames(sPath, _T("*.*")), &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
//		bRet = TRUE;
		do
		{
			ASSERT(hFind != INVALID_HANDLE_VALUE);
			
			{
				if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(_tcscmp(fd.cFileName, _T(".")) && _tcscmp(fd.cFileName, _T("..")))
					{
						lSize += CountDirSize(AddPathNames(sPath,fd.cFileName), nJob);
					}
				}
				else
				{
					tmp.LowPart = fd.nFileSizeLow;
					tmp.HighPart = fd.nFileSizeHigh;
					lSize += tmp.QuadPart;
				}
			}
		}
		while(m_pManager->CanContinue(nJob) && FindNextFile(hFind, &fd));
		
		FindClose(hFind);
		
	}

	return lSize;
}


BOOL CFilesListBase::CountDirsSize(ListData *pd, int nJob)
{
	if(AreFilesOperationsAvailable() && GetMode() != Ftp)
	{
		//CWaitCursor waitcr;
		if(pd == NULL)
		{
			POSITION pos = GetHeadPosition();
			
			while(pos && m_pManager->CanContinue(nJob))
			{
				LockList();
				//if(nJob!=m_pManager->GetCurJobNum())
				//	break;
				pd = GetNext(pos);
				BOOL bDir = pd->isDir();
				CString sName = AddPathNames(GetDir(), pd->getName());
				UnlockList();

				if(bDir)
				{
					LONGLONG nSize = CountDirSize(sName, nJob);
					LockList();
					pd->setSize(nSize);
					UnlockList();
				}
			}
		}
		else
		{
			LockList();
			BOOL bDir = pd->isDir();
			CString sName = AddPathNames(GetDir(), pd->getName());
			UnlockList();
			
			if(bDir)
			{
				LONGLONG nSize = CountDirSize(sName, nJob);
				LockList();
				pd->setSize(nSize);
				UnlockList();
			}
		}
		return TRUE;
	}
	return TRUE;
}

int CFilesListBase::ReadIcon(ListData *pld, int nDataVer)
{
	int nIcon = -1;
	LockList();
	{
		if(nDataVer == m_pManager->GetCurJobNum())
		{
			nIcon = pld->getIcon();
			pld->setExactIcon(nIcon);
		}
	}
	UnlockList();
	return nIcon;
}

int CFilesListBase::ReadIconUsingFileName(ListData *pld, int nDataVer)
{
	int nIcon = -1;

	if(!_tcsncmp(GetFileFullPath(NULL), _T("A:\\"), 3) || !_tcsncmp(GetFileFullPath(NULL), _T("a:\\"), 3))
		return nIcon;
	
	CSingleLock lock(&GetSynObject());
	{		
		lock.Lock();
		if(nDataVer != m_pManager->GetCurJobNum())
			return nIcon;
		CString sFileFullPath = GetFileFullPath(pld);
		CString sNameEl = pld->getName();
		int nExactIcon = pld->getExactIcon();
		lock.Unlock();
		
		//// we have copy of data so we can read additional info without locks
		int nOverlayIcon = -1;
			
		if(sNameEl != RET_NAME && nExactIcon == -1)
		{			
			SHFILEINFO sfi = {0};
			DWORD dwFlags = SHGFI_ICON | SHGFI_SMALLICON|SHGFI_OVERLAYINDEX | SHGFI_SYSICONINDEX; //SHGFI_ATTRIBUTES | SHGFI_ATTR_SPECIFIED |
			
			HIMAGELIST nRet = (HIMAGELIST)MySHGetFileInfo( sFileFullPath, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(sfi), dwFlags );
			if(nRet)
			{
				nIcon = sfi.iIcon & 0xFFFFFF;//8 highest bits it is overlay index
				nOverlayIcon = HIBYTE(HIWORD(sfi.iIcon));
				
				if(nOverlayIcon == 0)
					nOverlayIcon = -1;
				DestroyIcon(sfi.hIcon);
			}
			
			lock.Lock();
			// list is not actual - next enum files was done
			if(nDataVer == m_pManager->GetCurJobNum())
			{
				pld->setExactIcon(/*nIcon == nUnknown ? pld->getIcon() : */nIcon);
				pld->setOverlayIcon(nOverlayIcon);
			}
			lock.Unlock();
			////
		}
	}
	return nIcon;
}



CItemsStorage *CFilesListBase::SaveItemsUsingFilesNames(LISTDataPointers &listItems, BOOL bFullPaths/* = FALSE */)
{
	CItemsStorageShell *pData = new CItemsStorageShell(GetParent(), this);

	CStringList lFiles;
	CString sDir;

	if(listItems.GetCount()>0)
	{
		if(!bFullPaths)
			sDir = GetFileFullPath(NULL);
		POSITION pos = listItems.GetHeadPosition();
		const ListData *pd = NULL;
		while(pos)
		{
			pd = listItems.GetNext(pos);
			if(sDir.IsEmpty())
				lFiles.AddTail(GetFileFullPath(pd));
			else
				lFiles.AddTail(pd->getName());
		}
	}
	else
	{
		
		CString sCurDir = GetParentName(&sDir);
		if(!sCurDir.IsEmpty())
		{			
			lFiles.AddHead(sCurDir);
		}
	}

	BOOL bRet = CShell().GetPIDLsForFilesNames(pData, sDir, lFiles);
	if(!bRet)
	{
		delete pData;
		pData = NULL;
	}
	else
		pData->setDir(sDir);

	return pData;
}

#ifdef old_code_aa

/*
BOOL CFilesListBase::IsLink(ListData * pld)
{
	ULONG flags = SFGAO_LINK  ;
	LPITEMIDLIST pidl = pld->pIdList;
	if(pld->info.sName == RET_NAME)
		return FALSE;
	if(pidl == NULL)
	{
		if(m_mode == normal) ASSERT(0);
		return FALSE;
	}
	m_pShellFolder->GetAttributesOf(1, (const ITEMIDLIST **)&pidl, &flags);
	return (flags & SFGAO_LINK    ? TRUE : FALSE);
}
*/


/*BOOL CFilesListBase::ReadIconsForFileTypes()
{
	POSITION pos = GetHeadPosition();
	ListData *pd = NULL;
	while(pos)
	{
		pd = GetNext(pos);
		pd->nIcon = GetIconIndex(pd);
	}
	return TRUE;
}
*/

int CFilesListBase::GetReturnIcon()
{
	// to nie byl dobry pomysl
	// m_pidlListdla dla ftp tez jest NULL (chyba)
	/*if(m_pidlList == NULL)
	{
		return -1;
	}
	*/
	return RETURN_ICONINDEX;
}

/*void CFilesListBase::StartReadAddInfoThread()
{	
	CMyTimer tm;
	tm.Start();
	EndReadAddInfoFunction();
	
	CString sTime;
	//////
	//ReadIconsForFileTypes();
	if(GetMode() != Ftp && GetDir().Find(_T("A:\\")) == -1)
	{
		CSingleLock lock(&m_lFiles.GetSynObject());
	
		lock.Lock();
		m_nKey++;
		if(NULL == m_pThread)
		{
			m_pThread = AfxBeginThread(ReadAddInfoThread, this, THREAD_PRIORITY_IDLE, 0, CREATE_SUSPENDED);
			m_pThread->m_bAutoDelete = FALSE;
		}
		
		if(0 == m_pThread->ResumeThread())// nie byl uspiony
		{
			CoolTrace(3, TRUE, _T("Nie byl uspiony %s"), m_sPath);
//			ASSERT(0);
		}
		lock.Unlock();

	}
	tm.Stop();
	sTime += _T("\nStarting ReadAddInfoThread: ");
	sTime += tm.GetStringTime() + '\n';
	TRACE(sTime);
	
		//ReadAddInfoThread(this);
}

UINT CFilesListBase::ReadAddInfoThread(LPVOID pData)
{
	CFilesListBase *pList = (CFilesListBase *)pData;
	BOOL bTerminateThread = FALSE;
	while(!bTerminateThread)
	{
		pList->LockList();
		pList->m_bInReadAddInfo = TRUE;
		bTerminateThread = pList->m_bTerminateThread;
		pList->UnlockList();
		if(bTerminateThread)
			break;

		pList->ReadAddInfo();
		
		pList->LockList();
		pList->m_bInReadAddInfo = FALSE;
		bTerminateThread = pList->m_bTerminateThread;
		CoolTrace(3, TRUE, _T("After ReadAddInfo, before  SuspendThread %s"), pList->m_sPath);
		pList->UnlockList();
		if(bTerminateThread)
			break;
		//Sleep(10000);
		pList->m_pThread->SuspendThread();		
	}
	return 0;
}


BOOL CFilesListBase::EndReadAddInfoFunction(void)
{
	CSingleLock lock(&m_lFiles.GetSynObject());
	lock.Lock();
	m_nKey++;
	lock.Unlock();
	BOOL bThreadIsRun = FALSE;
	do
	{
		lock.Lock();
		bThreadIsRun = m_bInReadAddInfo;
		lock.Unlock();
		if(bThreadIsRun)
			Sleep(20);		
	}while(bThreadIsRun);
	return TRUE;
}
*/

#endif