// FilesListNet.cpp: implementation of the CFilesListNet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FilesListNet.h"
#include "Lm.h"
#include "CoolExplorer.h"
#include "FilesListManager.h"
#include "ItemsStorageShell.h"
#include "Shell.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilesListNet::CFilesListNet(CFilesListManager *pManager) : CFilesListBase(pManager)
{
	m_mode = Net;
}

CFilesListNet::~CFilesListNet()
{

}

BOOL CFilesListNet::EnumFiles(LPCTSTR sPath, int nJob)
{
	BOOL bRet = FALSE;
	
	{
		if(!_tcscmp(sPath, _T("\\\\")))
			bRet = EnumerateServers(nJob);
		else
			bRet = EnumerateNetResource(nJob, sPath);

	}

	return bRet;
}

BOOL CFilesListNet::Select(const ListData *pld, BOOL &bBrowseItem, int nJob)
{
	BOOL bRet = FALSE;
	LockList();
	CString sPath = GetDir();
	BOOL bDir = pld->isDir();
	CString sName = pld->getName();
	UnlockList();
	if(sName == RET_NAME)
	{
		bBrowseItem = TRUE;
		sPath = sPath.Left(sPath.Left(sPath.GetLength() - 1).ReverseFind(dir_separator)+1);
	}
	else if(bDir)
	{
		bBrowseItem = TRUE;
		sPath = AddPathNames(sPath,sName);
	}
	else
	{
		bBrowseItem = FALSE;
		TCHAR sCurDir[MAX_PATH];
		GetCurrentDirectory(sizeof(sCurDir), sCurDir);
		SetCurrentDirectory(sPath);
		
		
		CString sProgram = AddPathNames(sPath, sName);
		
		CString str = sPath;
		SHELLEXECUTEINFO shi={0};
		
		shi.cbSize = sizeof (SHELLEXECUTEINFO); 
		shi.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_FLAG_DDEWAIT; 
		shi.hwnd = GetParent(); 
		shi.lpVerb = NULL; 
		shi.lpFile = sProgram; 
		shi.lpParameters = NULL;
		shi.lpDirectory = str; 
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


	if(bBrowseItem)
		bRet = EnumFiles(sPath, nJob);

	return bRet;
}



BOOL CFilesListNet::EnumerateServers(int nJob)
{
	LPSERVER_INFO_101 pBuf = NULL;
	LPSERVER_INFO_101 pTmpBuf = NULL;
	DWORD dwLevel = 101;
	DWORD dwPrefMaxLen = -1;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwTotalCount = 0;
	DWORD dwServerType = SV_TYPE_SERVER; // all servers
	DWORD dwResumeHandle = 0;
	NET_API_STATUS nStatus;
	LPCWSTR pszServerName = NULL;
	DWORD i;
	
	sServer tmpServer;
	
	ListData ld;
	
	
	NetServerEnum_Fun ptr_NetServerEnum_Fun;
	
	ptr_NetServerEnum_Fun = ((CCoolExplorerApp*)AfxGetApp())->m_ptr_NetServerEnum_Fun;
	
	try
	{
		nStatus = (*ptr_NetServerEnum_Fun)((TCHAR *)pszServerName,
			dwLevel,
			(LPBYTE *) &pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			dwServerType,
			NULL,
			&dwResumeHandle);
	}
	catch(...)
	{
		int i = 0;
	}
	// if ok
	if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
	{
		if ((pTmpBuf = pBuf) != NULL)
		{
			LockList();
			{
				FreeList();
				//
				// Loop through the entries and 
				//  print the data for all server types.
				//
				for (i = 0; i < dwEntriesRead; i++)
				{
					//            assert(pTmpBuf != NULL);
					
					if (pTmpBuf == NULL)
					{
						break;
					}
					
					ld.Clear();
					//ld.info.sName = fd.cFileName;
					ld.setName((CString)((TCHAR*)pTmpBuf->sv101_name));
					ld.setDescription((CString)((TCHAR*)pTmpBuf->sv101_comment));
					ld.setAttrib(FILE_ATTRIBUTE_DIRECTORY);
					
					
					if(!m_pManager->CanContinue(nJob))
						break;
					if(CanAdd(&ld))
						AddTail(ld);
					//get computername and comment
					
					pTmpBuf++;
					
					dwTotalCount++;
				}
				m_sPath = _T("\\\\");
			}
			UnlockList();
			
		}
		if (pBuf != NULL)
		{
			NetApiBufferFree_Fun ptr_NetApiBufferFree_Fun = ((CCoolExplorerApp*)AfxGetApp())->m_ptr_NetApiBufferFree_Fun;
			
			(*ptr_NetApiBufferFree_Fun)(pBuf);
		}
	}
	//else if(m_pManager->CanContinue(nJob))
	//{
	//ErrorMessage(sServer, TRUE);
	//}
	
	
	return TRUE;
}

BOOL CFilesListNet::EnumerateNetResource(int nJob, LPCTSTR sServer)
{
	BOOL bRet = FALSE;
	sNetSearchThreads temp;
	int iThreadsRun = 0;
	CString sPath = _T("");
	CString sServerName = _T("");
	LPSHARE_INFO_1 bufptr = NULL;
	DWORD entriesread = 0;
	DWORD totalentries = 0;
	DWORD resume_handle = 0;
	NET_API_STATUS status = NERR_Success;
	LPSHARE_INFO_1 p = NULL;
	_result_list item;
	BOOL bSearch = TRUE;
	SERVERLIST::iterator i;
	_ff_mask mask;
	BOOL bEndThread = FALSE;
	ListData ld;

	
	sServerName = sServer;
	
	
	NetShareEnum_Fun ptr_NetShareEnum_Fun = ((CCoolExplorerApp*)AfxGetApp())->m_ptr_NetShareEnum_Fun;
	//enumerate resources
	
#ifndef _UNICODE	
	USES_CONVERSION;
	LPWSTR WideCharStr;  // wide-character buffer
	WideCharStr = A2W(sServerName);
	char * p1 = strdup(sServerName);
	
	status = (*ptr_NetShareEnum_Fun)((char *)WideCharStr,1,(LPBYTE *)&bufptr,MAX_PREFERRED_LENGTH,&entriesread,
		&totalentries,&resume_handle);
	free(p1);
	
#else
	
	
	status = (*ptr_NetShareEnum_Fun)((LPCWSTR)sServerName,1,(LPBYTE *)&bufptr,MAX_PREFERRED_LENGTH,&entriesread,
		&totalentries,&resume_handle);
#endif					
	//enumeration succed
	if (status == NERR_Success ||status == ERROR_MORE_DATA)
	{
		LockList();
		{
			FreeList();
			bRet = TRUE;
			p = bufptr;
			CString sTemp = _T("");
			for (int i = 1; i <= (int)entriesread; i++)
			{
				//create path to search for files
				// ->  //HOST/SHAREDITEM/*
				ld.Clear();
				//sTemp  = W2A((TCHAR*)(void*)p->shi1_netname);
				
				ld.setName(p->shi1_netname);
				ld.setDescription(p->shi1_remark);
				
				if(p->shi1_type == STYPE_DISKTREE)
					ld.setAttrib(FILE_ATTRIBUTE_DIRECTORY);
				if(!m_pManager->CanContinue(nJob))
					break;
				if(CanAdd(&ld))
					AddTail(ld);
				
				
				
				
				p++;					
			}
			
			m_sPath = sServer;
			AddRetDir(m_sPath);
		}
		UnlockList();
		
		if ( bufptr != NULL)
		{
			NetApiBufferFree_Fun ptr_NetApiBufferFree_Fun = ((CCoolExplorerApp*)AfxGetApp())->m_ptr_NetApiBufferFree_Fun;
			(*ptr_NetApiBufferFree_Fun)( bufptr );
		}
		
	}
	else if(m_pManager->CanContinue(nJob))
	{
		CString sMessage = sServer;
		sMessage += _T("\n");
		sMessage += GetErrorText(status);
		AfxMessageBox(sMessage);
	}
	
	

	return bRet;
}

CItemsStorage *CFilesListNet::SaveItems(LISTDataPointers &listItems)
{
	CItemsStorageShell *pData = NULL;
	if(listItems.GetCount() == 1)
	{
		CString sDir;
		CStringList lFiles;
		lFiles.AddTail(GetFileFullPath(listItems.GetHead()));
		pData = new CItemsStorageShell(GetParent(), this);
		BOOL bRet = CShell().GetPIDLsForFilesNames(pData, sDir, lFiles);
		if(!bRet)
		{
			delete pData;
			pData = NULL;
		}
		else
			pData->setDir(sDir);
	}
	return pData;
}