// FilesListNormal.cpp: implementation of the CFilesListNormal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FilesListNormal.h"
#include "ShellIcons.h"
#include "FilesListManager.h"
#include "Shell.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilesListNormal::CFilesListNormal(CFilesListManager *pManager) : CFilesListBase(pManager)
{
	dir_separator = '\\';
	m_mode = Normal;
}

CFilesListNormal::~CFilesListNormal()
{

}

BOOL CFilesListNormal::EnumFiles(LPCTSTR sPath, int nJob)
{
	BOOL bRet = FALSE;
	//IncrementKey();
	//CSingleLock lock(&GetSynObject());
	//lock.Lock();
	//m_nKey++;

	if(_tcslen(sPath)==0)
		return FALSE;

	CString sNewDir;
	ListData ld;
	WIN32_FIND_DATA fd={0};
	HANDLE hFind = INVALID_HANDLE_VALUE;
	
	
	{
		// prawdopodnie sciezka jest bezwzgledna
		sNewDir = sPath;
		
		if(!m_pManager->CanContinue(nJob))
			return FALSE;

		hFind = FindFirstFile(AddPathNames(sNewDir,_T("*.*")), &fd);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			if(!m_pManager->CanContinue(nJob))
				return FALSE;
			if(!_tcscmp(sPath+1, _T(":\\")))//root directory eg : "c:\"
			{
				//CreateFile sprawdzi czy istnieje root katalog, jesli tak oznacza to, ze jest on pusty i dlatego FindFirstFile nie zadzialalo 
				HANDLE hDir = CreateFile(sNewDir, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
				if(hDir != INVALID_HANDLE_VALUE)
				{
					CloseHandle(hDir);
					if(!m_pManager->CanContinue(nJob))
						return FALSE;
					LockList();
					m_sPath = sNewDir;
					FreeList();
					AddRetDir(m_sPath);
					UnlockList();
					return TRUE;
				}
				else
				{
					return FALSE;
				}

			}

			ErrorMessage(sNewDir, TRUE);
			//				AfxMessageBox((CString)"Path:"+sAddPath+"is incorect\nEnter another path or close this program\nCan you a little think befor you write rubbish dir path");
			
			return FALSE;
		}
	}
	
	
	LISTData *pList = new LISTData;
	
	if(sNewDir[sNewDir.GetLength()-1] != dir_separator)
		sNewDir += dir_separator;

	if(hFind == INVALID_HANDLE_VALUE)
	{
		ASSERT(0);
	}
	else
	{
		// skoro katalog istnieje
		
		//FreeList();
		

		do
		{
			if(_tcscmp(fd.cFileName, _T(".")) && _tcscmp(fd.cFileName, _T("..")))
			{
				//## tu trzeba rozdzielic name i extension
				ld.Clear();
				//ld.info.sName = fd.cFileName;
				ld.setFileInfo(fd);
				if(!m_pManager->CanContinue(nJob))
					break;
				if(CanAdd(&ld))
					pList->AddTail(ld);
			}
		}while(m_pManager->CanContinue(nJob) && FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
	//lock.Unlock();

	LockList();
	if(m_pManager->CanContinue(nJob))
	{
		m_sPath = sNewDir;
		SetNewList(pList);
		pList = NULL;
		AddRetDir(sNewDir);
//		StartReadAddInfoThread();
	}
	else
	{
		delete pList;
		pList = NULL;
	}
	UnlockList();

	return TRUE;
}

BOOL CFilesListNormal::Select(const ListData *pld, BOOL &bBrowseItem, int nJob)
{
	ASSERT(pld);
	BOOL bRet = FALSE;
	CString sPath;
	bBrowseItem = TRUE;
	LockList();
	//{
	const CString sName = pld->getName();
	const CString sGetDir = GetDir();
	
	if(sName == RET_NAME)//go to parent dir
		sPath = m_sPath.Left(m_sPath.Left(m_sPath.GetLength() - 1).ReverseFind(dir_separator)+1);
	else if(pld->isDir())
		sPath = AddPathNames(sGetDir,sName);
	else
		bBrowseItem = FALSE;
	//}
	UnlockList();
	
	
	if(bBrowseItem)
	{
		bRet = EnumFiles(sPath, nJob);
	}
	else // run program
	{
		bBrowseItem = FALSE;
		TCHAR sCurDir[MAX_PATH];
		GetCurrentDirectory(sizeof(sCurDir), sCurDir);
		SetCurrentDirectory(sGetDir);
		
		SHELLEXECUTEINFO shi={0};
		
		shi.cbSize = sizeof (SHELLEXECUTEINFO); 
		shi.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_FLAG_DDEWAIT ;//(umozliwia wywolywanie z innnego niz glowny watku);
		shi.hwnd = GetParent();//AfxGetMainWnd()->GetSafeHwnd(); 
		shi.lpVerb = NULL; 
		shi.lpFile = sName; 
		shi.lpParameters = NULL;
		shi.lpDirectory = sGetDir; 
		shi.nShow = SW_SHOWNORMAL; 
		
		if(!ShellExecuteEx( &shi))
		{
			//				if(_tsystem(sCommand) )
			AfxMessageBox(_T("Run Command Error"));
			//				else
			//					bRet = TRUE;
		}
		else
			bRet = TRUE;
		SetCurrentDirectory(sCurDir);
	}

	return bRet;
}







BOOL CFilesListNormal::CreateDirectory(LPCTSTR sDir)
{
	BOOL bRet = FALSE;
	
	bRet = ::CreateDirectory(GetDir() + sDir, NULL);

	return bRet;
}

BOOL CFilesListNormal::CanGoToMain()
{
	BOOL bEnable = FALSE;
	CString sPath = GetDir();
	if(_tcslen(sPath) > 3)
	{		
		if(sPath[1] == ':')
			bEnable = TRUE;
	}
	return bEnable;
}

/*BOOL CFilesListNormal::DisplayExplorerMenu(CItemsStorage *pData, CPoint pt)
{
	return CShell::DisplayExplorerMenu((CItemsStorageShell *)pData, pt);
}
*/

/*BOOL CFilesListNormal::ReadAddInfo()
{
	BOOL bRet = TRUE;
	CSingleLock lock(&GetSynObject());
	{
		int nUnknown = CShellIcons::GetUnknownIcon();
		lock.Lock();
		const int nKey = GetKeyValue();
		IShellFolder *pShellFolder = NULL;
		LPITEMIDLIST pidlList = NULL;
		LPITEMIDLIST pidlEl = NULL;
		CString sNameEl;
		POSITION pos = GetHeadPosition();		
		CString sPath = GetDir();
		lock.Unlock();
		
		ListData *pd;
		
		while(pos)
		{
			lock.Lock();
			// list is not actual - next enum files was done
			if(nKey != GetKeyValue())
			{
				bRet = FALSE;
				break;
			}
			pd = GetNext(pos);
			sNameEl = pd->getName();
			int nIcon = pd->getIcon();
			lock.Unlock();
			
			
			//// we have copy of data so we can read additional info without locks
			DWORD dwShAttrib = 0;
			int nOverlayIcon = -1;
			
			if(sNameEl != RET_NAME)
			{
				
				{
					SHFILEINFO sfi;
					memset(&sfi, 0, sizeof(sfi));
					DWORD dwFlags = SHGFI_ATTRIBUTES | SHGFI_ATTR_SPECIFIED | SHGFI_SYSICONINDEX  | SHGFI_SMALLICON;
					sfi.dwAttributes =  SFGAO_FOLDER | SFGAO_DISPLAYATTRMASK;
					size_t size = sizeof( SHFILEINFO );
					DWORD nRet = MySHGetFileInfo( sPath + sNameEl, NULL, &sfi, size, dwFlags );
					if(nRet)
					{
						dwShAttrib = sfi.dwAttributes;
						nIcon = sfi.iIcon;//GetSHIcon(sPath + sNameEl);
					}
					else
					{
						//dwShAttrib = 0;
						//nIcon = GetUnknownIcon();
					}
				}
				
				nOverlayIcon = CShellIcons::GetOverlayIndex(dwShAttrib);
				
			}
			
			lock.Lock();
			// list is not actual - next enum files was done
			if(nKey != GetKeyValue())
			{
				bRet = FALSE;
				break;
			}
			pd->setShAttrib(dwShAttrib);
			pd->setExactIcon(nIcon == nUnknown ? pd->getIcon() : nIcon);
			pd->setOverlayIcon(nOverlayIcon);
			lock.Unlock();
			////
		}
		
		
		if(lock.IsLocked())
			lock.Unlock();
		if(bRet && GetParent() && IsWindow(GetParent()))
			::PostMessage(GetParent(), WM_REFRESHICON, 0, 0);
	}
	return bRet;
}
*/
