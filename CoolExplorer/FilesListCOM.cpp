// FilesListCOM.cpp: implementation of the CFilesListCOM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FilesListCOM.h"
#include "Shell.h"
#include "FilesListManager.h"
#include "ShellIcons.h"
#include "shlobj.h"
#include "DropSource.h"
#include "ItemsStorageShell.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFilesListCOM::CFilesListCOM(CFilesListManager *pManager) : CFilesListBase(pManager), m_pidlList(NULL),m_pShellFolder(NULL)
{
	m_pShell = new CShell;
	m_bSilence = FALSE;
	m_bIsFileSystem = FALSE;
	m_bIsNetwork = FALSE;
	m_mode = COM;
}

CFilesListCOM::~CFilesListCOM()
{
	delete m_pShell;
}

BOOL CFilesListCOM::EnumFiles(LPCTSTR sPathParam, int nJob)
{	
//coinitailize();
	CString sPath = sPathParam;
	
	BOOL bRet = FALSE;
	
	LPITEMIDLIST pNewIDList = NULL;
	IShellFolder *pNewFolder = NULL;
	LISTData *pList = new LISTData;

	CSingleLock lock(&GetSynObject());

	
	if(!_tcscmp(sPathParam, _T_LNGSTR(9)))
	{
		HRESULT hr = SHGetDesktopFolder(&pNewFolder);
		if(hr == S_OK  )
		{
			bRet = SheelEnumFiles(pNewFolder, pList, nJob);
		}
		else
		{
			ASSERT(0);
			pNewFolder = NULL;
			AfxMessageBox(_T("ERROR: cant get desktop folder\n"));
		}

	}
	else if(!_tcscmp(sPathParam+1, _T(":\\..")))
	{
		sPath = sPath.Left(3);
		//LPITEMIDLIST pTempList = NULL;
		IShellFolder *pTempFolder = m_pShell->GetShellFolder(sPath, &pNewIDList);
		if(pTempFolder)
		{
			if(GetParentFolder(pNewFolder, pNewIDList, nJob))
				bRet = SheelEnumFiles(pNewFolder, pList, nJob);
			m_pShell->Release(pTempFolder);
		}
	}
	else
	{		
		lock.Lock();
		if(m_mapDisp2Parse.count(sPath))
			sPath = m_mapDisp2Parse[sPath];
		lock.Unlock();
		
		pNewIDList = NULL;
		pNewFolder = m_pShell->GetShellFolder(sPath, &pNewIDList);
		
		if(pNewFolder)
		{
			bRet = SheelEnumFiles(pNewFolder, pList, nJob);
		}
	}
	
	if(m_pManager->CanContinue(nJob) && bRet)
	{
		CString sPath2, sPath3;
		TCHAR sBuf[MAX_PATH*2] = {0};
		SHGetPathFromIDList(pNewIDList, sBuf);
		CString sPath = sBuf;
		if(sPath.IsEmpty())
		{
			sPath2 = m_pShell->GetDirForAddressBar(NULL, pNewIDList);
			sPath3 = m_pShell->GetPathFromIDList(NULL, pNewIDList, SHGDN_NORMAL | SHGDN_FORPARSING);
			
		}
		lock.Lock();
		m_bIsFileSystem = !sPath.IsEmpty();		
		if(sPath.IsEmpty())
		{
			m_mapDisp2Parse[sPath2] = sPath3;
			m_sPath = sPath2;
		}
		else
		{
			m_sPath = sPath;
			if(m_sPath.Right(1) != dir_separator)
				m_sPath += dir_separator;
		}
		
		m_pShell->Release(m_pShellFolder);
		m_pShellFolder = pNewFolder;
		pNewFolder = NULL;
		
		m_pShell->Free(m_pidlList);
		m_pidlList = pNewIDList;
		pNewIDList = NULL;
		
		SetNewList(pList);
		pList = NULL;
		
		if(m_pidlList) // NULL - desktop
			AddRetDir(sPath);
		lock.Unlock();
	}
	
	m_pShell->Release(pNewFolder);		
	m_pShell->Free(pNewIDList);	

//	CoUninitialize();

	delete pList;
	return bRet;
}

BOOL CFilesListCOM::Select(const ListData *pld, BOOL &bBrowseItem, int nJob)
{
//	CoInitialize(NULL);
	//m_pShellFolder = m_pShell->GetShellFolder(GetDir(), &m_pidlList);
	BOOL bRet = FALSE;
	CSingleLock lock(&GetSynObject());

	lock.Lock();
	const CString sGetDir = GetDir();
	const CString sGetName = pld->getName();
	BOOL bDir = pld->isDir();

	m_pShellFolder->AddRef();
	IShellFolder *pFolderCopy = m_pShellFolder;
	LPITEMIDLIST pIDListCopy = m_pShell->MakeCopy(m_pidlList);
	LPITEMIDLIST pIDItemCopy = m_pShell->MakeCopy(pld->getList());
	lock.Unlock();
	IShellFolder *pNewFolder = NULL;
	LPITEMIDLIST pNewIDList = NULL;
	LISTData *pList = new LISTData;

	if(sGetName == RET_NAME)
	{
		pNewIDList = m_pShell->MakeCopy(pIDListCopy);
		if(GetParentFolder(pNewFolder, pNewIDList, nJob))
			bRet = SheelEnumFiles(pNewFolder, pList, nJob);
	}
	else if(bDir)
	{
		pNewFolder = pFolderCopy;
		pNewFolder->AddRef();
		pNewIDList = m_pShell->MakeCopy(pIDListCopy);

		if(GetSubFolder(pNewFolder, pNewIDList, pIDItemCopy, nJob))
			bRet = SheelEnumFiles(pNewFolder, pList, nJob);
	}

	if(m_pManager->CanContinue(nJob))
	{
		if(bRet)
		{
			CString sPath2, sPath3;
			TCHAR sBuf[MAX_PATH*2] = {0};
			SHGetPathFromIDList(pNewIDList, sBuf);
			CString sPath = sBuf;
			if(sPath.IsEmpty())
			{
				sPath2 = m_pShell->GetDirForAddressBar(NULL, pNewIDList);
				sPath3 = m_pShell->GetPathFromIDList(NULL, pNewIDList, SHGDN_NORMAL | SHGDN_FORPARSING);

			}
			lock.Lock();
			m_bIsFileSystem = !sPath.IsEmpty();		
			if(sPath.IsEmpty())
			{
				m_mapDisp2Parse[sPath2] = sPath3;
				m_sPath = sPath2;
			}
			else
			{
				m_sPath = sPath;
				if(m_sPath.Right(1) != dir_separator)
					m_sPath += dir_separator;
			}
			
			m_pShell->Release(m_pShellFolder);
			m_pShellFolder = pNewFolder;
			pNewFolder = NULL;
			
			m_pShell->Free(m_pidlList);
			m_pidlList = pNewIDList;
			pNewIDList = NULL;
			
			SetNewList(pList);
			pList = NULL;
			
			if(m_pidlList) // NULL - desktop
				AddRetDir(sPath);
			lock.Unlock();
		}
		else
		{
			bRet = ExecuteItem(sGetDir, pIDListCopy, pIDItemCopy);
		}
	}	

	m_pShell->Release(pNewFolder);
	m_pShell->Release(pFolderCopy);
	
	m_pShell->Free(pNewIDList);	
	m_pShell->Free(pIDListCopy);
	m_pShell->Free(pIDItemCopy);

	delete pList;
	//CoUninitialize();

	return bRet;
}

BOOL CFilesListCOM::GetSubFolder(IShellFolder *&pNewFolder, LPITEMIDLIST &pNewIDList, const LPITEMIDLIST pListSub, int nJob)
{
	BOOL bRet = FALSE;
	ASSERT(pNewFolder != NULL && /*pNewIDList != NULL &&*/ pListSub != NULL);

	//LPITEMIDLIST pidlNewFolder = NULL;
	IShellFolder *pTempFolder;

	HRESULT hr = pNewFolder->BindToObject(pListSub, m_pShell->m_pBindContext, IID_IShellFolder, (LPVOID *) &pTempFolder);
	if(hr == S_OK)
	{		
		pNewIDList = m_pShell->Append(pNewIDList, pListSub);
		m_pShell->Release(pNewFolder);
		pNewFolder = pTempFolder;
		bRet = TRUE;
	}
	else
	{
		TRACE(_T("Error binding to subfolder"));
	}

	return bRet;
			
}

BOOL CFilesListCOM::GetParentFolder(IShellFolder *&pFolder, LPITEMIDLIST &listID, int nJob, LPITEMIDLIST *pChildId)
{
	BOOL bRet = FALSE;
	ASSERT(pFolder == NULL);
	
	IShellFolder * pDesktop = NULL;
	if(listID != NULL)
	{		
		HRESULT hr = SHGetDesktopFolder(&pDesktop);
		if(hr == S_OK  )
		{
			m_pShell->GetParentID(listID, pChildId);
			if(listID)
			{
				hr = pDesktop->BindToObject(listID,m_pShell->m_pBindContext,IID_IShellFolder,(void **) &pFolder);
				if(hr != S_OK  )
				{
					pFolder = NULL;
					ASSERT(0);
					TRACE(_T("Error bind to parent\n"));
				}
				bRet = hr == S_OK;
				m_pShell->Release(pDesktop);
			}
			else
			{
				pFolder = pDesktop;
				bRet = TRUE;
			}
			
		}
		else
		{
			ASSERT(0);
			TRACE(_T("ERROR: GetParentFolder cant get desktop folder\n"));
		}
	}
	else
	{
		ASSERT(0);
		TRACE(_T("listID Empty\n"));
	}

	return bRet;
	
}

BOOL CFilesListCOM::SheelEnumFiles(IShellFolder *pFolder, LISTData *plist, int nJob)
{
	BOOL bRet = FALSE;
	LPENUMIDLIST ppenum = NULL;
	LPITEMIDLIST pidlForEnum = NULL;
	ULONG celtFetched = 0;
	ListData EnumItemsData;
	EnumItemsData.Clear();

//	IncrementKey();
	
	// listowanie zawartosci katalogu
	HRESULT hr = pFolder->EnumObjects(m_bSilence ? NULL : GetParent(),SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN, &ppenum);
	if(hr == S_OK  )
	{
		while( hr = ppenum->Next(1,&pidlForEnum, &celtFetched) == S_OK && (celtFetched) == 1)
		{
			EnumItemsData.Clear();
			EnumItemsData.setList(pidlForEnum);
			GetFileInfo(pFolder, &EnumItemsData);
			if(CanAdd(&EnumItemsData))
				plist->AddTail(EnumItemsData);
			else
				m_pShell->Free(pidlForEnum);
		}
		EnumItemsData.Clear();
		pidlForEnum = NULL;
		bRet = TRUE;
	}
	
	m_pShell->Release(ppenum);
	
	return bRet;
}

BOOL CFilesListCOM::GetNetworkFileInfo(IShellFolder *pFolder, ListData *fi, LPITEMIDLIST pidlList)
{
	BOOL bRet = FALSE;
	NETRESOURCE *ntr = (NETRESOURCE *)new BYTE[4000];
	memset(ntr, 0, 4000);
	HRESULT hr = SHGetDataFromIDList(pFolder,(LPITEMIDLIST)pidlList, SHGDFIL_NETRESOURCE,(void *) ntr,2000);
	if(hr== S_OK)
	{
		bRet = TRUE;
		fi->setName(ntr->lpRemoteName);
		//if(fi->sName.ReverseFind(dir_separator) != -1)
		//	fi->sName = fi->sName.Right(fi->sName.GetLength()-fi->sName.ReverseFind(dir_separator)-1);
		fi->setDescription(ntr->lpComment);
	}
	delete [] ntr;
	return bRet;
}

BOOL CFilesListCOM::GetSystemFileInfo(IShellFolder *pFolder, ListData * fi, LPITEMIDLIST pidlList)
{
	BOOL bRet = FALSE;
	WIN32_FIND_DATA  pv;
	HRESULT hr = SHGetDataFromIDList(pFolder,(LPITEMIDLIST)pidlList, SHGDFIL_FINDDATA ,(void *) &pv,sizeof(WIN32_FIND_DATA ));
	if(hr == S_OK)
	{
		bRet = TRUE;
		/*if(_tcslen(pv.cFileName))
			fi->sName = pv.cFileName;
		fi->lSize = pv.nFileSizeLow;
		fi->tmCreat = pv.ftLastWriteTime;
		fi->cAttrib = pv.dwFileAttributes;*/
		fi->setFileInfo(pv);
	}
	/*else
	{
		ErrorMessage(_T("GetSystemFileInfo "), FALSE);
	}*/
	return bRet;
}

BOOL CFilesListCOM::GetFileInfo(IShellFolder *pFolder, ListData * pListData)
{
	LPITEMIDLIST pidlItems = pListData->getList();
//	fileinfo *pInfo = &pListData->info;

	CString sName = m_pShell->GetPathFromIDList(pFolder, pidlItems, SHGDN_INFOLDER /*|  SHGDN_FORPARSING*/);
	int nLen = sName.GetLength();

	if(nLen >= 4)
	{
		CString sT = sName.Right(4);
		// np. (Z:)
		sT.SetAt(1, ' ');
		if(sT == _T("( :)"))
		{			
			sName = sName.Mid(nLen - 3, 2) + _T("\\ (") + sName.Left(nLen-5) + _T(")");
		}
	}	

	pListData->setName(sName);
	if(m_bIsNetwork)
	{
		if(FALSE == GetNetworkFileInfo(pFolder, pListData, pidlItems))
		{
			if(GetSystemFileInfo(pFolder, pListData, pidlItems))
				m_bIsNetwork = FALSE;
		}
		if(m_bIsNetwork)
		{
			ULONG flags = SFGAO_FOLDER ;
			pFolder->GetAttributesOf(1, (const ITEMIDLIST **)&pidlItems, &flags);
			if(flags & SFGAO_FOLDER)
			{
				pListData->setAttrib(pListData->getAttrib() | FILE_ATTRIBUTE_DIRECTORY);
				pListData->setSize(-1);

			}
			pListData->setName(sName);
			
			return FALSE;
		}
	}
	else
	{
		if(FALSE == GetSystemFileInfo(pFolder, pListData, pidlItems))
		{
			if(GetNetworkFileInfo(pFolder, pListData, pidlItems))
			{
				m_bIsNetwork = TRUE;
				pListData->setName(sName);
			}

			{
				ULONG flags = SFGAO_FOLDER ;
				HRESULT hr = pFolder->GetAttributesOf(1, (const ITEMIDLIST **)&pidlItems, &flags);
				if(hr == S_OK && flags & SFGAO_FOLDER)
				{
					pListData->setAttrib(pListData->getAttrib() | FILE_ATTRIBUTE_DIRECTORY);
					pListData->setSize(-1);
				}
				return FALSE;
			}
		}
	}

	
		
	return TRUE;
}



BOOL CFilesListCOM::ExecuteItem(LPCTSTR sDir, LPITEMIDLIST idDir, LPITEMIDLIST idSelItem)
{
	BOOL bRet = TRUE;
	TCHAR sCurDir[MAX_PATH];
	GetCurrentDirectory(sizeof(sCurDir), sCurDir);
	SetCurrentDirectory(sDir);
	SHELLEXECUTEINFO shi;
	memset(&shi, 0, sizeof(shi));
	shi.cbSize = sizeof (SHELLEXECUTEINFO); 
	shi.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_IDLIST | SEE_MASK_INVOKEIDLIST | SEE_MASK_FLAG_DDEWAIT ; 
	shi.lpDirectory = sDir;
	shi.hwnd = NULL; 
	shi.lpVerb = NULL; 
	shi.lpFile = NULL; 
	shi.lpParameters = NULL; 
	shi.lpDirectory = NULL; 
	shi.nShow = SW_SHOWNORMAL; 
	shi.lpIDList = m_pShell->Append(idDir,idSelItem);
	
	if(FALSE == ShellExecuteEx( &shi))
	{
		ErrorMessage();
		bRet = FALSE;
	}
	m_pShell->Free(shi.lpIDList);		
	
	SetCurrentDirectory(sCurDir);
	return bRet;
}


BOOL CFilesListCOM::CreateDirectory(LPCTSTR sDir)
{
	BOOL bRet = FALSE;
	
	if(AreFilesOperationsAvailable())
		bRet = ::CreateDirectory(GetDir() + sDir, NULL);

	return bRet;
}


/*BOOL CFilesListCOM::DisplayExplorerMenu(CItemsStorage *pData, CPoint pt)
{
	return CShell::DisplayExplorerMenu((CItemsStorageShell*)pData, pt);
}*/

CItemsStorage *CFilesListCOM::SaveItems(LISTDataPointers &listItems)
{
	CItemsStorageShell *pData = new CItemsStorageShell(GetParent(), this);

	ITEMIDARRAY &lFiles = pData->getPidl();
	if(listItems.GetCount()>0)
	{
		m_pShellFolder->AddRef();
		pData->setFolder(m_pShellFolder);
		POSITION pos = listItems.GetHeadPosition();
		const ListData *pd = NULL;
		while(pos)
		{
			pd = listItems.GetNext(pos);
			lFiles.Add(m_pShell->MakeCopy(pd->getList()));
		}
		pData->setDir(GetDir());
	}
	else
	{
		IShellFolder *pNewFolder = NULL;
		LPITEMIDLIST pNewIDList = m_pShell->MakeCopy(m_pidlList);
		LPITEMIDLIST pChildId = NULL;
		if(GetParentFolder(pNewFolder, pNewIDList, JOBNUM_NONE, &pChildId))
		{
			lFiles.Add(pChildId);
			pData->setFolder(pNewFolder);
			pData->setDir(_T("Parent"));
		}
		else
		{
			delete pData;
			pData = NULL;
		}
		m_pShell->Free(pNewIDList);
	}

	return pData;
}
/*
BOOL CFilesListCOM::DisplayExplorerMenu(CItemsStorage *pData, CPoint clickpoint)
{
	BOOL bRet = FALSE;

	ITEMIDARRAY lFiles;
	if(listPointers.GetCount()>0)
	{
		POSITION pos = listPointers.GetHeadPosition();
		const ListData *pd = NULL;
		while(pos)
		{
			pd = listPointers.GetNext(pos);
			lFiles.Add(pd->getList());
		}
		
		CShell::DisplayExplorerMenu (hwnd, m_pShellFolder, lFiles.GetData(), lFiles.GetSize(), clickpoint);
	}
	else
	{
		IShellFolder *pNewFolder = NULL;
		LPITEMIDLIST pNewIDList = m_pShell->MakeCopy(m_pidlList);
		LPITEMIDLIST pChildId = NULL;
		if(GetParentFolder(pNewFolder, pNewIDList, JOBNUM_NONE, &pChildId))
		{
			lFiles.Add(pChildId);
			CShell::DisplayExplorerMenu (hwnd, pNewFolder, lFiles.GetData(), lFiles.GetSize(), clickpoint);
		}
		m_pShell->Release(pNewFolder);
		m_pShell->Free(pNewIDList);
		m_pShell->Free(pChildId);
		
	}

	return bRet;
}*/


CString CFilesListCOM::GetFileFullPath(const ListData * pdl) const
{
	//CSingleLock lock(&GetSynObject());
	//lock.Lock();
	if(!m_bIsFileSystem)
	{
		if(pdl)
		{
			ASSERT(pdl->getName() != RET_NAME);
			if(pdl->getName() == RET_NAME)
				return _T("");

			return m_pShell->GetDirForAddressBar(m_pShellFolder, pdl->getList());
		}
		else if(IsDesktop())
		{
			TCHAR szPath[MAX_PATH*2]={0};			
			SHGetSpecialFolderPath(GetParent(), szPath, CSIDL_DESKTOPDIRECTORY, FALSE);
			CString sPath = szPath;
			if(sPath.GetLength() > 1 && sPath.Right(1) != dir_separator)
				sPath += dir_separator;
			return szPath;
		}
		else
			return m_pShell->GetDirForAddressBar(m_pShellFolder, NULL);
	}
	
	return CFilesListBase::GetFileFullPath(pdl);	
}

BOOL CFilesListCOM::CanGoToMain()
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


int CFilesListCOM::ReadIcon(ListData *pld, int nDataVer)
{
	int nIcon = -1;
	if(!_tcsncmp(GetDir(), _T("A:\\"), 3) || !_tcsncmp(GetDir(), _T("a:\\"), 3))
		return nIcon;

	LPMALLOC pMalloc = NULL;
	SHGetMalloc(&pMalloc);
	if(NULL == pMalloc)
	{
		CoolTrace(0, TRUE, _T("Error get malloc int, dir:%s"), (LPCTSTR)GetDir());
		return nIcon;
	}

	CSingleLock lock(&GetSynObject());
	{
		CString sNameEl;
		IShellFolder *pShellFolder = NULL;
		LPITEMIDLIST pidlList = NULL;
		LPITEMIDLIST pidlEl = NULL;
		int nOverlayIcon = -1;
			
		lock.Lock();
		if(nDataVer == m_pManager->GetCurJobNum())
		{
			sNameEl = pld->getName();
			m_pShellFolder->AddRef();
			pShellFolder = m_pShellFolder;
			pidlList = m_pShell->MakeCopy(m_pidlList);
			if(sNameEl != RET_NAME)
			{
				pidlEl = m_pShell->MakeCopy(pld->getList());
			}
		}
		lock.Unlock();
		
		
		//// we have copy of data so we can read additional info without locks			
		if(sNameEl != RET_NAME && nDataVer == m_pManager->GetCurJobNum())
		{			
		
			IShellIcon *pShellIcon = NULL;
			
			pShellFolder->QueryInterface(IID_IShellIcon, (void **) &pShellIcon);
			

			IShellIconOverlay *pShellIconOverlay = NULL;
			
			pShellFolder->QueryInterface(IID_IShellIconOverlay, (void **) &pShellIconOverlay);

			if(pShellIconOverlay)
			{
				pShellIconOverlay->GetOverlayIndex(pidlEl, &nOverlayIcon);
				pShellIconOverlay->Release();
			}

			if(pShellIcon == NULL || pShellIcon->GetIconOf(pidlEl, GIL_FORSHELL ,&nIcon) != NOERROR)
			{
				SHFILEINFO shinfo = {0};
				LPITEMIDLIST pcurfolder = m_pShell->Append(pidlList, pidlEl);
				HIMAGELIST nRet = (HIMAGELIST)MySHGetFileInfo((LPCTSTR) pcurfolder, NULL, &shinfo, 
					sizeof(shinfo), 
					SHGFI_SYSICONINDEX  | SHGFI_SMALLICON|SHGFI_PIDL|SHGFI_OVERLAYINDEX|SHGFI_ICON );
				if(nRet)
				{
					nIcon = shinfo.iIcon & 0xFFFFFF;
					nOverlayIcon = HIBYTE(HIWORD(shinfo.iIcon));
					
					if(nOverlayIcon == 0)
						nOverlayIcon = -1;
					DestroyIcon(shinfo.hIcon);
				}
				if(pcurfolder)
					pMalloc->Free(pcurfolder);
				pcurfolder = NULL;
			}
			
			if(pShellIcon)
				pShellIcon->Release();
			
			lock.Lock();
			// list is not actual - next enum files was done
			if(nDataVer == m_pManager->GetCurJobNum())
			{
				pld->setExactIcon(nIcon);
				pld->setOverlayIcon(nOverlayIcon);
				lock.Unlock();
			}
			lock.Unlock();
		}
		if(pShellFolder)
		{
			pShellFolder->Release();
			pShellFolder = NULL;
		}
		
		if(pidlList)
			pMalloc->Free(pidlList);
	}

	if(pMalloc)
	{
		pMalloc->Release();
		pMalloc = NULL;
	}
	return nIcon;
}



#ifdef old_code_a
BOOL CFilesListCOM::ReadAddInfo()
{
	BOOL bRet = TRUE;
	LPMALLOC pMalloc = NULL;
	SHGetMalloc(&pMalloc);
	if(NULL == pMalloc)
	{
		CoolTrace(0, TRUE, _T("Error get malloc int, dir:%s"), (LPCTSTR)GetDir());
		return FALSE;
	}
	CSingleLock lock(&GetSynObject());
	{		
		lock.Lock();
		const int nKey = GetKeyValue();
		IShellFolder *pShellFolder = NULL;
		LPITEMIDLIST pidlList = NULL;
		LPITEMIDLIST pidlEl = NULL;
		CString sNameEl;
		POSITION pos = GetHeadPosition();
		
		CString sPath = GetDir();
		m_pShellFolder->AddRef();
		pShellFolder = m_pShellFolder;
		pidlList = m_pShell->MakeCopy(m_pidlList);

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
			if(sNameEl != RET_NAME)
			{
				pidlEl = m_pShell->MakeCopy(pd->getList());
			}				
			lock.Unlock();
			
			
			//// we have copy of data so we can read additional info without locks
			DWORD dwShAttrib = 0;
			int nOverlayIcon = -1;
			
			if(sNameEl != RET_NAME)
			{
				if(sNameEl.Left(5) != _T("A:\\ (") && sNameEl.Find(_T("CD Drive")) == -1)
					dwShAttrib = m_pShell->GetAtributesOf(pShellFolder, pidlEl);
				//else
				//	ld->info.dwShAttrib = ;
				IShellIcon *pShellIcon = NULL;
				
				pShellFolder->QueryInterface(IID_IShellIcon, (void **) &pShellIcon);
				
				if(pShellIcon == NULL || pShellIcon->GetIconOf(pidlEl, GIL_FORSHELL ,&nIcon) != NOERROR)
				{
					SHFILEINFO shinfo;
					LPITEMIDLIST pcurfolder = m_pShell->Append(pidlList, pidlEl);
					MySHGetFileInfo((LPCTSTR) pcurfolder, NULL, &shinfo, 
						sizeof(shinfo), 
						SHGFI_SYSICONINDEX  | SHGFI_SMALLICON|SHGFI_PIDL/*|SHGFI_ATTRIBUTES*/ );
					nIcon = shinfo.iIcon;
					if(pcurfolder)
						pMalloc->Free(pcurfolder);
					pcurfolder = NULL;
					//dwShAttrib = shinfo.dwAttributes;
				}
				
				if(pShellIcon)
					pShellIcon->Release();					
				
				nOverlayIcon = CShellIcons::GetOverlayIndex(dwShAttrib);
				
			}
			
			lock.Lock();
			// list is not actual - next enum files was done
			if(nKey != GetKeyValue())
			{
				if(pidlEl)
				{
					pMalloc->Free(pidlEl);
					pidlEl = NULL;
				}
				bRet = FALSE;
				break;
			}
			pd->setShAttrib(dwShAttrib);
			pd->setExactIcon(nIcon);
			pd->setOverlayIcon(nOverlayIcon);
			lock.Unlock();
			////
			if(pidlEl)
			{
				pMalloc->Free(pidlEl);
				pidlEl = NULL;
			}
		}
		
	
		if(pShellFolder)
		{
			pShellFolder->Release();
			pShellFolder = NULL;
		}
			
		if(pidlList)
			pMalloc->Free(pidlList);
	
		if(lock.IsLocked())
			lock.Unlock();
		if(bRet && GetParent() && IsWindow(GetParent()))
			::PostMessage(GetParent(), WM_REFRESHICON, 0, 0);
	}

	if(pMalloc)
	{
		pMalloc->Release();
		pMalloc = NULL;
	}
	return bRet;
}

//pListData - NULL odswiezenie
/*BOOL CFilesListCOM::ShellEnumFiles(ListData * pListData, BOOL bBindDesktop)
{
	BOOL bRet = FALSE;
	HRESULT hr;
	IShellFolder *pNewFolder = NULL;
	IShellFolder *pTempFolder = NULL;
	LPENUMIDLIST ppenum = NULL;
	LPITEMIDLIST pidlNewFolder = NULL;
	LPITEMIDLIST pidlTemp = NULL;
	LPITEMIDLIST pidlForEnum = NULL;

	ULONG celtFetched;
	ListData EnumItemsData;
	EnumItemsData.Clear();
	IShellFolder *pOldShellFolder = m_pShellFolder;
	LPITEMIDLIST pOldList = m_pidlList;

//	m_sPrevDir = GetParentName();

	try
	{
		if(pListData)
		{	
			// przejscie do desktopu
			if(bBindDesktop)
			{
				//AfxMessageBox("bind desktop");
				hr = SHGetDesktopFolder(&pNewFolder);
				if(hr != S_OK)
					throw 0;
				m_pidlList = NULL;
				m_pShellFolder = pNewFolder;
				pNewFolder = NULL;
			} // koniec bindowania do desktopu

			// przejscie do podkatalogu
			if(pListData->getList())
			{
				//AfxMessageBox("bind subdirectory");
				LPITEMIDLIST pidlNewFolder = pListData->getList();			
				hr = m_pShellFolder->BindToObject(pidlNewFolder, m_pShell->m_pBindContext, IID_IShellFolder, (LPVOID *) &pNewFolder);
				if(hr != S_OK)
					throw 0;

				m_pShellFolder = pNewFolder;
				pNewFolder = NULL;
			
				pidlTemp = m_pShell->Append(m_pidlList, pidlNewFolder);
				m_pidlList = pidlTemp;
				pidlTemp = NULL;
			}//przejscie do podkatalogu zakonczone sukcesem
			else if(!bBindDesktop)
			{
			// przechodzenie o katalog wyzej
				//AfxMessageBox("bind parent directory");
				if(m_pidlList == NULL)
					throw 0;
				hr = SHGetDesktopFolder(&pTempFolder);
				if(hr != S_OK  )
					throw 0;

				m_pidlList = m_pShell->MakeCopy(m_pidlList);

				m_pShell->GetParentID(m_pidlList);
				if(m_pidlList)
				{
					hr = pTempFolder->BindToObject(m_pidlList,m_pShell->m_pBindContext,IID_IShellFolder,(void **) &pNewFolder);
					if(hr != S_OK  ) throw 0;

					m_pShellFolder = pNewFolder;
					pNewFolder = NULL;
				}
				else
				{
					m_pShellFolder = pTempFolder;
					pTempFolder = NULL;
				}
			}// wyjscie do katalogu wyzej zakonczone sukcesem
			else
			{
				//AfxMessageBox("Internal module error\nNast¹pi³ nieoczekiwany b³¹d w programie\nKod b³êdu 0x123\n");
			}
		}//koniec bindowania do nowego katalogu


		// listowanie zawartosci katalogu
		hr = m_pShellFolder->EnumObjects(m_bSilence ? NULL : AfxGetMainWnd()->GetSafeHwnd(),SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN, &ppenum);
		if(hr != S_OK  )
		{
			throw 0;
		}
		FreeList();

		while( hr = ppenum->Next(1,&pidlForEnum, &celtFetched) == S_OK && (celtFetched) == 1)
		{
			EnumItemsData.Clear();
			EnumItemsData.setList(pidlForEnum);
			GetFileInfo(&EnumItemsData);
			if(CanAdd(&EnumItemsData))
				AddTail(EnumItemsData);
		}
		pidlForEnum = NULL;
	
		if(m_pidlList)
		{
			//int icon = 0;
			TCHAR sBuf[MAX_PATH];
			SHGetPathFromIDList(m_pidlList, sBuf);
			m_sPath = sBuf;
			m_bIsFileSystem = !m_sPath.IsEmpty();
			AddRetDir(m_sPath);
		}



		if(pListData)
		{
			m_pShell->Free(pOldList);
			m_pShell->Release(pOldShellFolder);
		}

		bRet = TRUE;
	}
	catch(int a)
	{
		CoolTrace(0, TRUE, _T("CFilesListCOM::ShellEnumFiles(ListData * pListData, BOOL bBindDesktop) catch:%d"), a);

		m_pShellFolder = pOldShellFolder;
		m_pidlList = pOldList;
		bRet = FALSE;
	}

	m_pShell->Free(pidlForEnum);
	m_pShell->Free(pidlNewFolder);
	m_pShell->Release(ppenum);
	m_pShell->Free(pidlTemp);
	m_pShell->Release(pNewFolder);
	m_pShell->Release(pTempFolder);

	if(m_sPath.IsEmpty())
		m_sPath = GetDirForAdressBar();

	return bRet;
}*/
/*
BOOL CFilesListCOM::ShellEnumFiles(LPCTSTR sPath)
{
	ListData ld;
	WCHAR pwPath[MAX_PATH];

#ifdef UNICODE
	_tcscpy(pwPath, sPath);
#else
	USES_CONVERSION;
	wcscpy(pwPath, A2W(sPath));
#endif
	
	ULONG ulCount, ulAttr;
	LPITEMIDLIST pidlMain = NULL;
	BOOL bBindDesktop = FALSE;


	if((_tcscmp(_T("m_desktop_path"), sPath) == 0) 
		||(_tcscmp(_T_LNGSTR(9), sPath) == 0))
	{
		ld.setList(NULL);
		LPITEMIDLIST pidl = NULL;
		TCHAR szPath[MAX_PATH] = {0};


		HRESULT res = SHGetSpecialFolderLocation( NULL,CSIDL_DESKTOP,&pidl);
		//return ShellEnumFiles(szPath);

		if ( res == S_OK )
		{
			SHGetSpecialFolderPath(NULL,szPath,CSIDL_DESKTOP,FALSE);

			if (!SHGetPathFromIDList( pidl,szPath))
			{
				return FALSE;

			}
			else
			{
				return ShellEnumFiles(&ld, TRUE);
			}
		}
		else 
			return ShellEnumFiles(&ld, TRUE);
	}
	if(_tcscmp(_T_LNGSTR(126), sPath) == 0)
	{
		LPITEMIDLIST pidl = NULL;
		TCHAR szPath[MAX_PATH] = {0};


		HRESULT res = SHGetSpecialFolderLocation( NULL,CSIDL_NETHOOD,&pidl);
		//return ShellEnumFiles(szPath);

		if ( res == S_OK )
		{
			if (!SHGetPathFromIDList( pidl,szPath))
			{
				return FALSE;

			}
			else
			{
				return ShellEnumFiles(szPath);
			}
		}
	}

	//if(CanUsePidl() == FALSE)
	{
		if(m_pidlList == NULL || m_pShellFolder == NULL)
		{
			m_pShell->Free(m_pidlList);
			m_pShell->Release(m_pShellFolder);
			//m_pShellFolder = m_pShell->GetShellFolder(sPath, &m_pidlList);
			m_pShellFolder = m_pShell->GetShellFolder(GetDir(), &m_pidlList);
		}
		else
		{
			//prawdopodobnie wyjscie z ftp
			return ShellEnumFiles(NULL,FALSE);
		}
	}


	if(_tcscmp(RET_NAME, sPath) == 0)
	{
		ld.setList(NULL);
		return ShellEnumFiles(&ld, FALSE);
	}

	if(m_pShellFolder == NULL
		|| S_OK != m_pShellFolder->ParseDisplayName( m_bSilence ? NULL : AfxGetMainWnd()->GetSafeHwnd(),
		m_pShell->m_pBindContext, pwPath, &ulCount, &pidlMain, &ulAttr))
	{
		IShellFolder *pShellFolder = NULL;
		if(S_OK != SHGetDesktopFolder(&pShellFolder))
			return FALSE;

		HRESULT hr = pShellFolder->ParseDisplayName ( m_bSilence ? NULL : AfxGetMainWnd()->GetSafeHwnd(),
			m_pShell->m_pBindContext, pwPath, &ulCount, &pidlMain, &ulAttr);
		if(hr != S_OK)
		{
			ErrorMessage((CString)_T("Can't open directory:\n") + sPath, !m_bSilenceMode);
			m_pShell->Release(pShellFolder);
			return FALSE;
		}
		m_pShell->Release(pShellFolder);		
//		m_pShellFolder = pShellFolder;
//		m_pShell->Free(m_pidlList);
		bBindDesktop = TRUE;
	}
	ld.setList(pidlMain);
	BOOL bRet = ShellEnumFiles(&ld, bBindDesktop);
	m_pShell->Free(pidlMain);
	return bRet;
}*/

#endif