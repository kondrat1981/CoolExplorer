// FilesTree.cpp: implementation of the CDirTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoolExplorer.h"
#include "FilesTree.h"
#include "QuestionDlg.h"


///////////////
#define FTPENDFILEREAD -1
#define FTPENDFILEWRITE -2
#define FTPERROR -3
struct FtpData
{
	DWORD dwDataLen;
	char buf[2048];
};

typedef  CList<FtpData, const FtpData&> FTPLIST;
class CFtpDataList : protected FTPLIST, public SafetyStruct
{
public:
	CFtpDataList::CFtpDataList()
	{
		m_bEndReadThread = FALSE;
		m_bEndWriteThread = FALSE;
	}
protected:
	BOOL m_bEndReadThread;
	BOOL m_bEndWriteThread;
public:
	POSITION AddTail(const FtpData& info)
	{
		Lock();
		POSITION pos = FTPLIST::AddTail(info);
		Unlock();
		return pos;
	}
	POSITION AddHead(const FtpData& info)
	{
		Lock();
		POSITION pos = FTPLIST::AddHead(info);
		Unlock();
		return pos;
	}
	FtpData GetHead( )
	{
		Lock();
		FtpData data = FTPLIST::GetHead();
		Unlock();
		return data;
	}
	FtpData RemoveHead( )
	{
		Lock();
		FtpData data = FTPLIST::RemoveHead();
		Unlock();
		return data;
	}
	BOOL IsEmpty()
	{
		Lock();
		BOOL bRet = FTPLIST::IsEmpty();
		Unlock();
		return bRet;
	}
	BOOL IsError()
	{
		Lock();
		BOOL bRet = FALSE;
		if(!FTPLIST::IsEmpty())
			bRet = FTPLIST::GetHead().dwDataLen == FTPERROR;
		Unlock();
		return bRet;
	}
	BOOL IsEnd()
	{
		Lock();
		BOOL bRet = m_bEndReadThread && m_bEndWriteThread;
		Unlock();
		return bRet;
	}
	void SetEndReadThread()
	{
		Lock();
		m_bEndReadThread = TRUE;
		Unlock();
	}

	void SetEndWriteThread()
	{
		Lock();
		m_bEndWriteThread = TRUE;
		Unlock();
	}
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirTree::CDirTree()
{
	m_bOtherFilesInDir = FALSE;
}

CDirTree::~CDirTree()
{
	Clean();
}

void CDirTree::Clean()
{
	while(!m_lDir.IsEmpty())
		delete m_lDir.RemoveHead();

	m_lFiles.RemoveAll();
}


ULONGLONG CDirTree::Init(LPCTSTR sPath, LPCTSTR sMask, const CStringList &lstrFiles, CopyInfo *pInfo)
{
	ULONGLONG nSize = 0;
//	ASSERT(sPath && _tcslen(sPath));
	ASSERT(m_lFiles.GetCount() == 0 && m_lDir.GetCount() == 0); // first use clean function
//	this->sName = sPath;
	this->m_recInfo.sName = sPath;
	ASSERT(m_recInfo.sName.Right(1) != _T("\\") || m_recInfo.sName.GetLength() <= 3);

	if(pInfo)
	{
		pInfo->Lock();
		pInfo->sDirName = sPath;
		pInfo->Unlock();
	}

	POSITION pos = lstrFiles.GetHeadPosition();
	while(pos && pInfo && !pInfo->IsCancelled())
	{
		CString sName = lstrFiles.GetNext(pos);
		CString sFullPath = AddPathNames(sPath, sName); 
		{
			CString sDir, sFile;
			if(_tcschr(sName, '\\')) //multilevel path
			{				
				GetDirFile(sDir, sFile, sFullPath, '\\');
			}
			
			WIN32_FIND_DATA fd;
			memset(&fd, 0, sizeof(fd));
			HANDLE hFind = FindFirstFile(sFullPath, &fd);
			if(hFind == INVALID_HANDLE_VALUE)
			{
				ASSERT(0);
				return 0;
			}
			else
			{	
				if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(_tcscmp(fd.cFileName, _T(".")) && _tcscmp(fd.cFileName, _T("..")))
					{
						CDirTree *pSubDir = new CDirTree;
						pSubDir->m_recInfo = fd;
						
						ULONGLONG nSizeOld = nSize;
						nSize += pSubDir->InitRecurse(AddPathNames(sPath, sDir), fd.cFileName, sMask, pInfo);
						pSubDir->m_recInfo.sPath = sDir;
						if(pInfo && pInfo->bNotCopyEmpty && nSizeOld == nSize && pSubDir->Count() == 0)
						{
							TRACE((CString)_T("Don't insert to list empty dir:") + fd.cFileName + _T("\n"));
							delete pSubDir;
						}
						else
						{
							m_lDir.AddTail(pSubDir);
						}
					}
				}
				else if(Compare(sMask, fd.cFileName))
				{				
					CDirFile dir_file;
					dir_file = fd;
					dir_file.sPath = sDir;
					m_lFiles.AddTail(dir_file);		
					
					
					nSize += dir_file.m_size.QuadPart;
				}
				else
				{
					m_bOtherFilesInDir = TRUE;
				}
				FindClose(hFind);
			}
		}
	}	
	
	return nSize;
}


ULONGLONG CDirTree::InitRecurse(LPCTSTR sPath, LPCTSTR sDir,LPCTSTR sMask, CopyInfo *pInfo)
{
	ULONGLONG nSize = 0;
	ASSERT(sPath && _tcslen(sPath));
	ASSERT(m_lFiles.GetCount() == 0 && m_lDir.GetCount() == 0); // first use clean function
//	this->sName = sPath;
	this->m_recInfo.sName = sDir;
	ASSERT(m_recInfo.sName.Right(1) != _T("\\") || m_recInfo.sName.GetLength() <= 3);

	CString sCurPath = AddPathNames(sPath, sDir);
	
	if(pInfo && pInfo->pWnd )
	{
		pInfo->Lock();
		pInfo->sDirName = sCurPath;
		pInfo->Unlock();
	}

	
	{
		WIN32_FIND_DATA fd;
		memset(&fd, 0, sizeof(fd));
		HANDLE hFind = FindFirstFile(AddPathNames(sCurPath, _T("*")), &fd);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			ASSERT(0);
		}
		else
		{
			do
			{
				if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(_tcscmp(fd.cFileName, _T(".")) && _tcscmp(fd.cFileName, _T("..")))
					{
						CDirTree *pSubDir = new CDirTree;
						pSubDir->m_recInfo = fd;
//						pSubDir->sName = fd.cFileName;
						ULONGLONG nSizeOld = nSize;
						nSize += pSubDir->InitRecurse(sCurPath, fd.cFileName, sMask, pInfo);
						if(pInfo && pInfo->bNotCopyEmpty && nSizeOld == nSize && pSubDir->Count() == 0)
						{
							nSize = nSizeOld;
							TRACE((CString)_T("Don't insert to list empty dir:") + fd.cFileName + _T("\n"));
							delete pSubDir;
						}
						else
						{
							m_lDir.AddTail(pSubDir);
						}

					}
				}
				else if(Compare(sMask, fd.cFileName))
				{
					CDirFile dir_file;
					dir_file = fd;
					m_lFiles.AddTail(dir_file);
					nSize += dir_file.m_size.QuadPart;
				}
/*				else
				{
					CString str;
					str.Format("File: %s not matches mask", fd.cFileName);
					AfxMessageBox(str);
				}
*/			}while(FindNextFile(hFind, &fd) && !pInfo->IsCancelled());
			FindClose(hFind);
		}
	}
	return nSize;
}

ULONGLONG CDirTree::FtpInit(CFtpConnection *pFtpConection, LPCTSTR sPath, LPCTSTR sMask, const CStringList *lstrFiles, CopyInfo *pInfo)
{
	POSITION pos = NULL;
	ULONGLONG nSize = 0;
	ASSERT(sPath && _tcslen(sPath));
	m_recInfo.sName = sPath;
	CString sCurPath, s1;
	pFtpConection->GetCurrentDirectory(sCurPath);
	VERIFY(pFtpConection->SetCurrentDirectory(sPath));
	pFtpConection->GetCurrentDirectory(s1);
	
	if(pInfo)
	{
		pInfo->Lock();
		pInfo->sDirName = s1;
		pInfo->Unlock();
		pInfo->pWnd->PostMessage(WM_PROGRESS, PROGRESS_DIRNAME, (LPARAM)pInfo);
	}

	if(lstrFiles)
	{
		CStringList StringList;
		StringList.AddTail((CStringList *)lstrFiles);

		WIN32_FIND_DATA fd;
		memset(&fd, 0, sizeof(fd));
		HANDLE hFind = FtpFindFirstFile(*pFtpConection, NULL, &fd, 0 ,NULL);
		//ErrorMessage();
		do
		{
			if(hFind == INVALID_HANDLE_VALUE)
			{
				ASSERT(0);
			}
			else
			{
				if(pos = StringList.Find(fd.cFileName))
				{
					StringList.RemoveAt(pos);
					if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						if(_tcscmp(fd.cFileName, _T(".")) && _tcscmp(fd.cFileName, _T("..")))
						{
							CDirTree *pSubDir = new CDirTree;
							pSubDir->m_recInfo = fd;
							//nSize += pSubDir->FtpInit(pFtpConection, fd.cFileName, sMask);
							m_lDir.AddTail(pSubDir);
						}					
					}
					else if(Compare(sMask, fd.cFileName))
					{
						CDirFile dir_file;
						dir_file = fd;
						m_lFiles.AddTail(dir_file);
						nSize += fd.nFileSizeLow;
					}
				}
			}
		}while(!pInfo->IsCancelled() && !StringList.IsEmpty() && InternetFindNextFile(hFind, &fd));

	VERIFY(InternetCloseHandle(hFind));

	pos = m_lDir.GetHeadPosition();
	while(pos && !pInfo->IsCancelled())
	{
		CDirTree *pSubDir = m_lDir.GetNext(pos);
		nSize += pSubDir->FtpInit(pFtpConection, pSubDir->m_recInfo.sName, sMask, NULL, pInfo);
	}
				
	}
	else
	{
		WIN32_FIND_DATA fd;
		memset(&fd, 0, sizeof(fd));
		HANDLE hFind = FtpFindFirstFile(*pFtpConection, NULL, &fd, 0, NULL);
		//ErrorMessage();
		if(hFind == INVALID_HANDLE_VALUE)
		{
			ASSERT(0);
		}
		else if((int)hFind == ERROR_FTP_TRANSFER_IN_PROGRESS)
		{
			TRACE(_T("Ftp connection in progresss\n"));
		}
		else
		{
			do
			{
				if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(_tcscmp(fd.cFileName, _T(".")) && _tcscmp(fd.cFileName, _T("..")))
					{
						CDirTree *pSubDir = new CDirTree;
						pSubDir->m_recInfo = fd;
						m_lDir.AddTail(pSubDir);
						
						//nSize += pSubDir->FtpInit(pFtpConection, fd.cFileName, sMask);						
					}
				}
				else if(Compare(sMask, fd.cFileName))
				{
					CDirFile dir_file;
					dir_file = fd;
					m_lFiles.AddTail(dir_file);
					nSize += fd.nFileSizeLow;
				}
/*				else
				{
					CString str;
					str.Format("File: %s not matches mask", fd.cFileName);
					AfxMessageBox(str);
				}
*/			}while(!pInfo->IsCancelled() && InternetFindNextFile(hFind, &fd));
			InternetCloseHandle(hFind);


			pos = m_lDir.GetHeadPosition();
			while(!pInfo->IsCancelled() && pos)
			{
				CDirTree *pSubDir = m_lDir.GetNext(pos);
				nSize += pSubDir->FtpInit(pFtpConection, pSubDir->m_recInfo.sName, sMask, NULL, pInfo);
			}
	
		}
	}
	VERIFY(pFtpConection->SetCurrentDirectory(sCurPath));
	return nSize;

}


CDirEnumerator::CDirEnumerator(LPCTSTR sPath, LPCTSTR sMask, const CStringList *lstrFiles, CFtpConnection *pFtpConection) 
{

	if(pFtpConection)
	{
		m_pFtpConection = pFtpConection;
		m_nAllSize = m_DirTree.FtpInit(pFtpConection, sPath, sMask, lstrFiles, NULL);

	}
	else
	{
		CString str = sPath;
		if(str.Right(1) == _T("\\"))
			str = str.Left(str.GetLength() - 1);

		m_nAllSize = m_DirTree.Init(str, sMask, *lstrFiles, NULL);
	}
}

CDirEnumerator::~CDirEnumerator()
{
}

void CDirEnumerator::Enum(CDirTree *pDir, LPCTSTR sDesc)
{
	static BOOL bMain = FALSE;
	CString sDestimation = sDesc;
	if(bMain)
		bMain = FALSE;
	else if(pDir)
		sDestimation += pDir->m_recInfo.sName + _T("\\");

	if(pDir)
	{
		POSITION pos = pDir->m_lFiles.GetHeadPosition();
		while(pos)
		{
			CDirFile *dir_file = &pDir->m_lFiles.GetNext(pos);
			if(m_pFileFun)
				(* m_pFileFun)(dir_file, sDestimation);
		}

		pos = pDir->m_lDir.GetHeadPosition();
		while(pos)
		{
			CDirTree *pDirTree = pDir->m_lDir.GetNext(pos);
			if(m_pDirFun)
				(*m_pDirFun)(pDirTree->m_recInfo.sName, sDestimation);
			Enum(pDirTree, sDestimation);
		}
	}
	else
	{
		bMain = TRUE;
		Enum(&m_DirTree, sDesc);
	}
}

void CDirEnumerator::Enum(PFILEFUN pFileFun, PDIRFUN pDirFun, LPCTSTR sDesc)
{
	m_pFileFun = pFileFun;
	m_pDirFun = pDirFun;
	Enum(NULL, sDesc);
}

INT_PTR CDirTree::Count()
{
	INT_PTR count = 0;
	count += m_lFiles.GetCount();
	POSITION pos = m_lDir.GetHeadPosition();
	while(pos)
	{
		count += m_lDir.GetNext(pos)->Count();
	}
	return count;
}
///
DWORD CALLBACK CopyProgressCallback(
  LARGE_INTEGER TotalFileSize,          // file size
  LARGE_INTEGER TotalBytesTransferred,  // bytes transferred
  LARGE_INTEGER StreamSize,             // bytes in stream
  LARGE_INTEGER StreamBytesTransferred, // bytes transferred for stream
  DWORD dwStreamNumber,                 // current stream
  DWORD dwCallbackReason,               // callback reason
  HANDLE hSourceFile,                   // handle to source file
  HANDLE hDestinationFile,              // handle to destination file
  LPVOID lpData                         // from CopyFileEx
)
{
	
	/*CopyInfo *pInfo = (CopyInfo *)lpData;
	if(pInfo->nCount%10==0)
	{
		//pInfo->
		//CString str;
		//str.Format("prograss:%I64d,%I64d \n", TotalFileSize, TotalBytesTransferred);
		//TRACE(str);
		//memcpy(&pInfo->nFileCopied,&TotalBytesTransferred, sizeof(pInfo->nFileCopied));
		pInfo->nFileCopied = TotalBytesTransferred.QuadPart;
		pInfo->pWnd->PostMessage(WM_PROGRESS, PROGRESS_POS, (LPARAM)pInfo);
	}
	pInfo->nCount++;
	if(pInfo->IsCancelled())
		return PROGRESS_CANCEL;
	else
		return PROGRESS_CONTINUE;*/

	CopyInfo *pInfo = (CopyInfo *)lpData;
	
	DWORD dwRet = 0;

	pInfo->Lock();
//	if(pInfo->bPause)
//		dwRet = PROGRESS_STOP;
//	else
	{
		//pInfo->nCount++;
		pInfo->nFileCopied = TotalBytesTransferred.QuadPart;
		if(pInfo->IsCancelled(FALSE))
			dwRet = PROGRESS_CANCEL;
		else
			dwRet = PROGRESS_CONTINUE;
	}
	pInfo->Unlock();

	return dwRet;
}


//.

void CDirTree::Copy(LPCTSTR sPathFrom, LPCTSTR strPathTo, CopyInfo *pInfo)
{
	BOOL bAllDeleted = TRUE;
	BOOL bRes = FALSE;
	CString sPathTo = strPathTo;//AddPathNames(strPathTo, m_recInfo.sPath) ;
	CString sRename;
	BOOL bDel = FALSE;
	if(_tcslen(sPathFrom))
		/*sPathTo = AddPathNames(sPathTo, m_recInfo.sName)*/;
	else
	{
		pInfo->Lock();
		sRename = pInfo->sDestMod;
		pInfo->sDestMod = _T("");
		pInfo->Unlock();
	}
	

	pInfo->Lock();
	BOOL bMove = pInfo->bMove;
	CString sPrefix = pInfo->sPrefix;
	CString sPostfix = pInfo->sPostfix;
	pInfo->Unlock();

	CString sMyName;
	if(m_recInfo.sPath.IsEmpty())
	{
		sMyName = AddPathNames(sPathFrom, m_recInfo.sName);
	}
	else
	{
		sMyName = AddPathNames(sPathFrom, m_recInfo.sPath);
		sMyName = AddPathNames(sMyName, m_recInfo.sName);
		//RecurseCreateDir(sPathTo,NULL); // multilevel
	}
	
	POSITION pos = m_lDir.GetHeadPosition();
	CDirTree *pDir = NULL;
	CString sTemp;
	BOOL bSkip = FALSE;

	while(pos && !pInfo->IsCancelled())
	{
		pDir = m_lDir.GetNext(pos);
		if(!pInfo->m_pinfoSmart)
		{
			if(sRename.IsEmpty())
				sTemp = AddPathNames(sPathTo, pDir->m_recInfo.sName);
			else
			{
				sTemp = AddPathNames(sPathTo, sRename);
				sRename = _T("");
			}
			
			
			
			if(MyCreateDirectory(sTemp, NULL) == FALSE)
			{
				AfxMessageBox((CString)_T("ErrorCreating directory:\n") + sTemp); 
				return;			
			}
		}
		pDir->Copy(sMyName, sTemp, pInfo);
		if(!pInfo->m_pinfoSmart && MySetFileStatus(sTemp, &pDir->m_recInfo) == FALSE)
			ASSERT(0);
	}

	CString sFileFrom, sFileTo;
	pos = m_lFiles.GetHeadPosition();
	while(pos && !pInfo->IsCancelled())
	{
		CDirFile &dir_file = m_lFiles.GetNext(pos);
		if(dir_file.sPath.IsEmpty())
		{
			sFileFrom = AddPathNames(sMyName, dir_file.sName);
		}
		else
		{
			sFileFrom = AddPathNames(sMyName, dir_file.sPath);
			sFileFrom = AddPathNames(sFileFrom, dir_file.sName);
		}

		if(pInfo->m_pinfoSmart)
		{
			pInfo->m_pinfoSmart->CreateName(dir_file.sName, sFileTo, sPathTo, sMyName, &((WIN32_FIND_DATA)dir_file));
			sFileTo = AddPathNames(sPathTo, sFileTo);
			RecurseCreateDir(sPathTo,NULL);
		}
		else if(sRename.IsEmpty())
			sFileTo = AddPathNames(sPathTo, dir_file.sName, '\\', sPrefix, sPostfix);
		else
		{
			sFileTo = AddPathNames(sPathTo, sRename);
			sRename = _T("");
		}

		HANDLE hFile = CreateFile(sFileTo, 0, 0, 0, OPEN_EXISTING,0,NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			//stawanie na glowie tylko po to zeby uzytkownik mogl zdecydowac czy nadpisac plik
			CloseHandle(hFile);
			CString str;
			str.Format(_T("File %s exist in destimation dir\nOverwrite this file?"), sFileTo);
		
			DWORD dwFlags = pInfo->GetFlags();
			if((dwFlags & SKIP_ALL) || (dwFlags & SKIP_ALL_EXISTING))
				goto info;

			BOOL bTheSameFiles = FALSE;
			if(AreTheSameFiles(sFileFrom, sFileTo))
			{
				bTheSameFiles = TRUE;
				CString sMes;
				//te same pliki
				sMes.Format(_T("Cannot copy %s to %s\n Probably they are the same.\nTry to copy/move this file?"), sFileFrom, sFileTo);
				switch(AfxMessageBox(sMes,MB_YESNOCANCEL | MB_DEFBUTTON3))
				{
				case IDYES:
					break;
				case IDCANCEL:
					pInfo->AddFlags(ABORT);
					return;
				default:
					goto info;
					break;
				}
				
			}
			else
			{				
				if(!(dwFlags & YES_TO_ALL))
				{
					CQuestionDlg dlg(str, CQuestionDlg::OverWrite);
					switch(dlg.DoModal())
					{
					case SKIP:
						goto info;
					case SKIP_ALL:
						pInfo->AddFlags(SKIP_ALL);
						goto info;
					case YES:
						break;
					case YES_TO_ALL:
						pInfo->AddFlags(YES_TO_ALL);
						break;
					case SKIP_ALL_THIS_TYPE:
						pInfo->AddFlags(SKIP_ALL_EXISTING);
						goto info;
						break;
					case ABORT:
						pInfo->AddFlags(ABORT);
						return;
					}
				}
				
				if(FALSE == DeleteFile(sFileTo))
				{
					//ErrorMessage();
					//usuniecie atrybutu read only, sytem itd...
					//i ponowna proba usuniecia
					
					// wazne : mozna wstawic pytanie o usuniecie pliku read only, sytemowego lub ukrytego
					if(FALSE == SetFileAttributes(sFileTo, FILE_ATTRIBUTE_NORMAL)
						|| FALSE == DeleteFile(sFileTo))
					{
						CString str;
						str.Format(_T("CannotDeleteFile %s\n"), sFileTo);
						ErrorMessage(str);
						goto info;
					}
				}
			}
			
			
		}// koniec, plik w katalogu docelowym napewno nie istnieje

		///

		pInfo->Lock();
		pInfo->sNameFrom = sFileFrom;
		pInfo->sNameTo = sFileTo;
		pInfo->nFileSize = dir_file.m_size.QuadPart;
		//pInfo->nCount = 1;
		pInfo->nFileCopied = 0;		
		pInfo->Unlock();

		//pInfo->pWnd->PostMessage(WM_PROGRESS, PROGRESS_NEW, (LPARAM)pInfo);

		///
		while(!pInfo->IsCancelled())
		{
			if(bMove)
			{
				bRes = MoveFileWithProgress(sFileFrom, sFileTo, CopyProgressCallback, pInfo, MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH );
			}
			else
			{
				bRes = CopyFileEx(sFileFrom, sFileTo, CopyProgressCallback, pInfo, &pInfo->bCancel, 0);
			}
			
			/*BOOL bRepeat = FALSE;
			/*do
			{
				bRepeat = FALSE;
				if(bMove)
				{
					bRes = MoveFileWithProgress(sFileFrom, sFileTo, CopyProgressCallback, pInfo, MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH );
				}
				else
				{
					bRes = CopyFileEx(sFileFrom, sFileTo, CopyProgressCallback, pInfo, &pInfo->bCancel, COPY_FILE_RESTARTABLE);
				}
				
				while(TRUE)
				{
					if(pInfo->IsCancelled())
					{
						bRepeat = FALSE;
						break;
					}
					pInfo->Lock();
					BOOL bPause = pInfo->bPause;
					pInfo->Unlock();
					if(bPause)
					{
						bRepeat = TRUE;
						Sleep(500);
					}
					else
					{
						break;
					}
				}
				
				
				//pInfo->Unlock();
			}while(bRepeat);*/

			int nError = 0;
			if(!bRes)
				nError = GetLastError();

			if(bRes || pInfo->CheckFlags(SKIP_ALL | SKIP_ALL_SHARED))
				break;

			if(pInfo->IsCancelled())
				return;
			bSkip = FALSE;
			//ErrorMessage(sFileFrom, FALSE);
			CString str;
			str.Format(_T("File %s.\n %s"), sFileFrom, GetErrorText(nError));
			CQuestionDlg dlg(str, CQuestionDlg::AccessDenied);
			switch(dlg.DoModal())
			{
				case SKIP:
					bSkip = TRUE;
					break;						
				case SKIP_ALL:
					pInfo->AddFlags(SKIP_ALL);
					break;
				case SKIP_ALL_THIS_TYPE:
					pInfo->AddFlags(SKIP_ALL_SHARED);
					break;
				case YES:
					break;
				case ABORT:
					pInfo->AddFlags(ABORT);
						//ASSERT(bRetry == FALSE);
					return;
				default: 
					ASSERT(0);
					break;
			}
			if(bSkip || pInfo->CheckFlags(SKIP_ALL_SHARED | SKIP_ALL))
				break;
		}
		if(!bRes)
			bAllDeleted = FALSE;
info:
		pInfo->Lock();
		pInfo->nAllCopied += dir_file.m_size.QuadPart;
		pInfo->nFileCopied = 0;
		pInfo->Unlock();
	}

	if(bMove && bAllDeleted)
	{
		if(!pInfo->IsCancelled()
			&& !m_bOtherFilesInDir // nie probuje kasowac katalogu gdy jest w nim wiecej plikow
			&& (_tcslen(sPathFrom) || _tcslen(m_recInfo.sPath))	// nie kasuje katalogu root
			)
		{
			BOOL bRetry;
			do
			{
				if(MyRemoveDirectory(sMyName, NULL) || GetLastError() == ERROR_DIR_NOT_EMPTY /*145 - Directory is not empty*/)
				{
					//ASSERT(GetLastError() != ERROR_DIR_NOT_EMPTY);
					break;
				}
				ErrorMessage(sMyName, FALSE);
				//TCHAR sCurPath[MAX_PATH];
				//GetCurrentDirectory(MAX_PATH, sCurPath);
				//AfxMessageBox(sCurPath);
				
				if(pInfo->CheckFlags(SKIP_ALL) )
					bRetry = FALSE;
				else
				{
					CQuestionDlg dlg((CString) _T("Can't delete directory:\n") + sMyName, CQuestionDlg::CantDelete);
					INT_PTR iRes = dlg.DoModal();
					switch(iRes)
					{
					case YES:
						bRetry = TRUE;
						break;
					case SKIP:
						bRetry = FALSE;
						break;
					case SKIP_ALL:
						bRetry = FALSE;
						pInfo->AddFlags(SKIP_ALL);
						break;
					case ABORT:
						pInfo->AddFlags(ABORT);
						//AfxThrowUserException();
						break;
					default: ASSERT(0);
					}
				}
			}while(bRetry && pInfo->IsCancelled());
		}
	}
}


void CDirTree::Delete(LPCTSTR sPath, CopyInfo *pDelInfo)
{
	BOOL bDel = FALSE;
	CString sDelName;
	char cDirSep = pDelInfo->pFtpCon ? '/' : '\\';
	CString sMyName;
	if(m_recInfo.sPath.IsEmpty())
	{
		sMyName = AddPathNames(sPath, m_recInfo.sName);
	}
	else
	{
		sMyName = AddPathNames(sPath, m_recInfo.sPath);
		sMyName = AddPathNames(sMyName, m_recInfo.sName);	
	}
	
	while(!m_lDir.IsEmpty() && !pDelInfo->IsCancelled())
	{
		CDirTree *pDir = m_lDir.RemoveHead();
		CString sDelDir = pDir->m_recInfo.sName;
		pDir->Delete(sMyName, pDelInfo);
		delete pDir;
	}
	while(!m_lFiles.IsEmpty() && !pDelInfo->IsCancelled())
	{
		CDirFile dir_file = m_lFiles.RemoveHead();
		if(dir_file.sPath.IsEmpty())
		{
			sDelName = AddPathNames(sMyName, dir_file.sName, cDirSep);
		}
		else
		{
			sDelName = AddPathNames(sMyName, dir_file.sPath, cDirSep);
			sDelName = AddPathNames(sDelName, dir_file.sName, cDirSep);
		}

		bDel = FALSE;
		if(pDelInfo->pFtpCon == NULL)
		{
			if(MyDeleteFile(sDelName))
				bDel = TRUE;
			else
				SetFileAttributes(sDelName, FILE_ATTRIBUTE_NORMAL);
		}
		if(!bDel)
		{
			BOOL bRetry = FALSE;
			do
			{
				if(pDelInfo->pFtpCon == NULL)
					bDel = MyDeleteFile(sDelName);
				else
					bDel = pDelInfo->pFtpCon->Remove(sDelName);

				if(bDel)
				{
					bDel = TRUE;
					break;
				}
				if(pDelInfo->CheckFlags(SKIP_ALL))
					bRetry = FALSE;
				else
				{
					CQuestionDlg dlg((CString) "Can't delete file:\n" + sDelName, CQuestionDlg::CantDelete);
					INT_PTR iRes = dlg.DoModal();
					switch(iRes)
					{
					case YES:
						bRetry = TRUE;
						break;
					case SKIP:
						bRetry = FALSE;
						break;
					case SKIP_ALL:
						bRetry = FALSE;
						pDelInfo->AddFlags(SKIP_ALL);
						break;
					case ABORT:
						pDelInfo->AddFlags(ABORT);
						bRetry = FALSE;
						//AfxThrowUserException();
						break;
					default: ASSERT(0);
					}
				}
			}while(bRetry && !pDelInfo->IsCancelled());
		}
		pDelInfo->Lock();
		pDelInfo->sNameFrom = sDelName;
		pDelInfo->nCount++;
		//int nCount = pDelInfo->nCount;
		//pDelInfo->bDeleted = bDel;
		pDelInfo->Unlock();
		//pDelInfo->pWnd->PostMessage(WM_PROGRESS, PROGRESS_AND_NAME, (LPARAM)pDelInfo);
		//pDelInfo->pWnd->PostMessage(WM_PROGRESS, PROGRESS_FROM, nCount);
	}
	if(!pDelInfo->IsCancelled()
		&& !m_bOtherFilesInDir // nie probuje kasowac katalogu gdy jest w nim wiecej plikow
		&& (_tcslen(sPath) || _tcslen(m_recInfo.sPath))	// nie kasuje katalogu root
		)
	{
		BOOL bRetry = FALSE;
		do
		{
			if(MyRemoveDirectory(sMyName, pDelInfo->pFtpCon) || GetLastError() == ERROR_DIR_NOT_EMPTY /*145 - Directory is not empty*/)
			{
				break;
			}
			ErrorMessage(sMyName, FALSE);
			//TCHAR sCurPath[MAX_PATH];
			//GetCurrentDirectory(MAX_PATH, sCurPath);
			//AfxMessageBox(sCurPath);

			if(pDelInfo->CheckFlags(SKIP_ALL) || !pDelInfo->bMesInErr )
				bRetry = FALSE;
			else
			{
				CQuestionDlg dlg((CString) _T("Can't delete directory:\n") + sMyName, CQuestionDlg::CantDelete);
				INT_PTR iRes = dlg.DoModal();
				switch(iRes)
				{
				case YES:
					bRetry = TRUE;
					break;
				case SKIP:
					bRetry = FALSE;
					break;
				case SKIP_ALL:
					bRetry = FALSE;
					pDelInfo->AddFlags(SKIP_ALL);
					break;
				case ABORT:
					pDelInfo->AddFlags(ABORT);
					bRetry = FALSE;
					break;
				default: ASSERT(0);
				}
			}
		}while(bRetry && !pDelInfo->IsCancelled());
	}
}

BOOL CDirTree::SetStatus(ChangeStatusInfo *pInfo, LPCTSTR sPath)
{
	//CString sMyName = AddPathNames(sPath, m_recInfo.sName, '\\');
	POSITION pos = m_lDir.GetHeadPosition();
	CString sFullName;
	while(pos)
	{
		CDirTree *pDir = m_lDir.GetNext(pos);
		//CString sChangeDir = pDir->m_recInfo.sName;
		sFullName = AddPathNames(sPath, pDir->m_recInfo.sName);
		pDir->SetStatus(pInfo, sFullName);
		MySetFileStatus(sFullName, &pDir->m_recInfo, pInfo);
	}
	pos = m_lFiles.GetHeadPosition();
	while(pos)
	{
		CDirFile &dir_file = m_lFiles.GetNext(pos);
		sFullName = AddPathNames(sPath, dir_file.sName);
		MySetFileStatus(sFullName, &dir_file, pInfo);
	}	
	return TRUE;
}



void CDirTree::Copy(CFtpConnection * pFtpCon, LPCTSTR sPathFrom, LPCTSTR strPathTo, FtpProgressInfo * pFtpInfo)
{
	CString sPathTo = strPathTo;
	BOOL bDel = FALSE;
	if(_tcslen(sPathFrom))
		sPathTo = AddPathNames(sPathTo, m_recInfo.sName, '/');

	CString sMyName = AddPathNames(sPathFrom, m_recInfo.sName);
	
	POSITION pos = m_lDir.GetHeadPosition();
	CDirTree *pDir = NULL;
	CString sTemp;
	CopyInfo *pInfo = pFtpInfo->pCopyInfo;

	while(pos && !pInfo->IsCancelled())
	{
		pDir = m_lDir.GetNext(pos);
		sTemp = AddPathNames(sPathTo, pDir->m_recInfo.sName, '/');
		if(pFtpCon->CreateDirectory(sTemp) == FALSE)
		{
			WIN32_FIND_DATA fd;
			HANDLE hFind = FtpFindFirstFile(pFtpCon, sTemp, &fd, 0 ,NULL);
			if(hFind == INVALID_HANDLE_VALUE)
			{
				ASSERT(0);
				AfxMessageBox((CString)"ErrorCreating directory:\n" + sTemp); 
				return;
			}
			InternetCloseHandle(hFind);
		}
		//if(MySetFileStatus(sPathTo, &pDir->m_recInfo) == FALSE)
		//	ASSERT(0);
		pDir->Copy(pFtpCon, sMyName, sPathTo, pFtpInfo);
	}

	CString sFileFrom, sFileTo;
	pos = m_lFiles.GetHeadPosition();
	while(pos && !pInfo->IsCancelled())
	{
		CDirFile &dir_file = m_lFiles.GetNext(pos);
		sFileFrom = AddPathNames(sMyName, dir_file.sName);
		sFileTo = AddPathNames(sPathTo, dir_file.sName, '/');

		pInfo->Lock();
		pInfo->sNameFrom = sFileFrom;
		pInfo->sNameTo = sFileTo;
		pInfo->nFileSize = dir_file.m_size.QuadPart;
		//pInfo->nCount = 0;
		pInfo->nFileCopied = 0;
		pInfo->Unlock();

		//pInfo->pWnd->PostMessage(WM_PROGRESS, PROGRESS_NEW, (LPARAM)pInfo);

		if(FALSE == pFtpCon->PutFile( sFileFrom, sFileTo, FTP_TRANSFER_TYPE_BINARY, (DWORD)pFtpInfo))
			ErrorMessage(_T("Error put file on server"));

		pInfo->Lock();
		pInfo->nAllCopied += dir_file.m_size.QuadPart;		
		pInfo->Unlock();
	}
}

void CDirTree::CopyFtp(CFtpConnection * pFtpCon, LPCTSTR sPathFrom, LPCTSTR strPathTo, FtpProgressInfo * pFtpInfo)
{
	CString sPathTo = strPathTo;
	BOOL bDel = FALSE;
	CopyInfo *pInfo = pFtpInfo->pCopyInfo;
	
	pInfo->Lock();
	const char cToDirSep = pInfo->pFtpConDest ? '/' : '\\';
	CFtpConnection *pFtpTo = pInfo->pFtpConDest;
	pInfo->Unlock();

	if(_tcslen(sPathFrom))
		sPathTo = AddPathNames(sPathTo, m_recInfo.sName, cToDirSep);

	CString sMyName = AddPathNames(sPathFrom, m_recInfo.sName, '/');
	
	POSITION pos = m_lDir.GetHeadPosition();
	CDirTree *pDir = NULL;
	CString sTemp;

	while(pos && !pInfo->IsCancelled() )
	{
		pDir = m_lDir.GetNext(pos);
		sTemp = AddPathNames(sPathTo, pDir->m_recInfo.sName, cToDirSep);
		if(pFtpTo)
		{
			if(pFtpTo->CreateDirectory(sTemp) == FALSE)
			{
				WIN32_FIND_DATA fd;
				HANDLE hFind = FtpFindFirstFile(pFtpCon, sTemp, &fd, 0 ,NULL);
				if(hFind == INVALID_HANDLE_VALUE)
				{
					ASSERT(0);
					AfxMessageBox((CString)"ErrorCreating directory:\n" + sTemp); 
					return;
				}
				InternetCloseHandle(hFind);
			}
		}
		else
		{
			if(MyCreateDirectory(sTemp, NULL) == FALSE)
			{
				AfxMessageBox((CString)"ErrorCreating directory:\n" + sTemp); 
				return;
			}
		}

		if(!pFtpTo && MySetFileStatus(sTemp, &pDir->m_recInfo) == FALSE)
			ASSERT(0);
		pDir->CopyFtp(pFtpCon, sMyName, sPathTo, pFtpInfo);
	}

	CString sFileFrom, sFileTo;
	pos = m_lFiles.GetHeadPosition();
	while(pos && !pInfo->IsCancelled())
	{

		//(*pFun)(pInfo);


		CDirFile &dir_file = m_lFiles.GetNext(pos);
		sFileFrom = AddPathNames(sMyName, dir_file.sName, '/');
		sFileTo = AddPathNames(sPathTo, dir_file.sName, cToDirSep);

		pInfo->Lock();
		pInfo->sNameFrom = sFileFrom;
		pInfo->sNameTo = sFileTo;
		pInfo->nFileSize = dir_file.m_size.QuadPart;
		//pInfo->nCount = 0;
		pInfo->nFileCopied = 0;
		pInfo->Unlock();
		
		//pInfo->pWnd->PostMessage(WM_PROGRESS, PROGRESS_NEW, (LPARAM)pInfo);

		if(pFtpTo)
		{
			if(FALSE == MyCopyFtp(pFtpInfo))
				ErrorMessage((CString)_T("Error copy from ftp to ftp file from server") + sFileFrom + _T("\n") + sFileTo);

		}
		else if(FALSE == pFtpCon->GetFile( sFileFrom, sFileTo, /*bFailIfExists*/ FALSE, FILE_ATTRIBUTE_NORMAL,FTP_TRANSFER_TYPE_BINARY,(DWORD)pFtpInfo))
			ErrorMessage((CString)_T("Error get file from server") + sFileFrom + _T("\n") + sFileTo);

		pInfo->Lock();
		pInfo->nAllCopied += dir_file.m_size.QuadPart;
		pInfo->Unlock();
	}
/*
		HANDLE hFile = CreateFile(sFileTo, 0, 0, 0, OPEN_EXISTING,0,NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			//stawanie na glowie tylko po to zeby urzytkownik mogl zdecydowac czy nadpisac plik
			CloseHandle(hFile);
			CString str;
			str.Format(_T("File %s exist in destimation dir\nOverwrite this file?"), sFileTo);
		
			if(pInfo->dwFlags & SKIP_ALL)
				goto info;

			if(!(pInfo->dwFlags & YES_TO_ALL))
			{
				CQuestionDlg dlg(str, CQuestionDlg::OverWrite);
				switch(dlg.DoModal())
				{
					case SKIP:
						goto info;
				case SKIP_ALL:
						pInfo->dwFlags |= SKIP_ALL;
						goto info;
				case YES:
					break;
				case YES_TO_ALL:
					pInfo->dwFlags |= YES_TO_ALL;
					break;
				case ABORT:
					pInfo->dwFlags |= ABORT;
					return;
				}
			}
			if(FALSE == DeleteFile(sFileTo))
			{
				//ErrorMessage();
				//usuniecie atrybutu read only, sytem itd...
				//i ponowna proba usuniecia

				// wazne : mozna wstawic pytanie o usuniecie pliku read only, sytemowego lub ukrytego
				if(FALSE == SetFileAttributes(sFileTo, FILE_ATTRIBUTE_NORMAL)
				|| FALSE == DeleteFile(sFileTo))
				{
					CString str;
					str.Format(_T("CannotDeleteFile %s\n"), sFileTo);
					ErrorMessage(str);
					goto info;
				}
			}		
		}// koniec, plik w katalogu docelowym napewno nie istnieje

		while(CopyFile(sFileFrom, sFileTo, TRUE) == FALSE
			&&
			  !(pInfo->dwFlags & SKIP_ALL)
			&&
			  !(pInfo->dwFlags & SKIP_ALL_SHARED))
		{

			ErrorMessage(sFileFrom);
			CString str;
			str.Format(_T("File %s.\n Access Denied."), sFileFrom);
			CQuestionDlg dlg(str, CQuestionDlg::AccessDenied);
			switch(dlg.DoModal())
			{
				case SKIP:
					break;						
				case SKIP_ALL:
					pInfo->dwFlags |= SKIP_ALL;
					break;
				case SKIP_ALL_THIS_TYPE:
					pInfo->dwFlags |= SKIP_ALL_SHARED;
					break;
				case YES:
					break;
				case ABORT:
					pInfo->dwFlags |= ABORT;
						//ASSERT(bRetry == FALSE);
					return;
				default: 
					ASSERT(0);
					break;
			}
			if(pInfo->dwFlags & SKIP_ALL_SHARED || pInfo->dwFlags & SKIP_ALL)
				break;
		}
info:
		pInfo->sNameFrom = sFileFrom;
		pInfo->sNameTo = sFileTo;
		pInfo->nFileSize = dir_file.m_size;
		(*pFun)(pInfo);
	}
*/
//	(*pFun)(pInfo);
}

BOOL CDirTree::GetFiles(CStringList &listFiles, CStringList &listDirs)
{
	return GetFiles(_T(""), listFiles, listDirs);
}

BOOL CDirTree::GetFiles(LPCTSTR sDir, CStringList &listFiles, CStringList &listDirs)
{
	char cDirSep = '\\';
	CString sMyName = AddPathNames(sDir, m_recInfo.sName, cDirSep);
	if(_tcslen(sDir))
		listDirs.AddTail(sMyName);
	POSITION pos = m_lDir.GetHeadPosition();
	while(pos)
	{
		CDirTree *pDir = m_lDir.GetNext(pos);
		pDir->GetFiles(sMyName, listFiles, listDirs);		
	}

	pos = m_lFiles.GetHeadPosition();
	while(pos)
	{
		CString sDelName = AddPathNames(sMyName, m_lFiles.GetNext(pos).sName, cDirSep);
		listFiles.AddTail(sDelName);
	}

	return TRUE;
}

UINT CDirTree::ReadFtpThread(LPVOID pData)
{
	TRACE(_T("CDirTree::ReadFtpThread begin\n"));
	UINT nRet = 0;
	FtpCopyInfo *pInfo =(FtpCopyInfo *)pData;

	pInfo->Lock();
	CFtpDataList *pList = pInfo->pList;
	CInternetFile *pFile = pInfo->pFileFrom;
	CopyInfo *pCopyInfo = pInfo->pFtpCopyInfo->pCopyInfo;
	pInfo->Unlock();

	FtpData data;

	try
	{
		while(!pCopyInfo->IsCancelled() && (data.dwDataLen = pFile->Read(data.buf, sizeof(data.buf))))
		{
			pList->AddTail(data);
			if(pList->IsError())
			{
				nRet = -1;
				break;
			}
		}
	}
	catch(CInternetException *pExc)
	{
		FtpData data;
		data.dwDataLen = FTPERROR;
		pList->AddHead(data);
		pExc->GetErrorMessage((LPTSTR)data.buf, sizeof(data.buf));
		TRACE(_T("%s\n"), (LPCTSTR)data.buf);
		pExc->Delete();
		nRet = -2;
	}

	if(nRet == 0)
	{
		memset(data.buf, 0, sizeof(data.buf));
		data.dwDataLen = FTPENDFILEREAD;
		pList->AddTail(data);
	}
	pList->SetEndReadThread();
	TRACE(_T("CDirTree::ReadFtpThread end %d\n"), nRet);
	return nRet;
}

UINT CDirTree::WriteFtpThread(LPVOID pData)
{
	TRACE(_T("CDirTree::WriteThread begin\n"));
	UINT nRet = 0;
	FtpCopyInfo *pInfo =(FtpCopyInfo *)pData;

	pInfo->Lock();
	CopyInfo *pCopyInfo = pInfo->pFtpCopyInfo->pCopyInfo;
	CFtpDataList *pList = pInfo->pList;
	CInternetFile *pFile = pInfo->pFileTo;
	
	pInfo->Unlock();

	FtpData data;
	while(!pCopyInfo->IsCancelled())
	{
		if(!pList->IsEmpty())
		{
			//if(pList->IsError())
			//	return -1;
			data = pList->RemoveHead();
			if(data.dwDataLen == FTPENDFILEREAD)
			{
				data.dwDataLen = FTPENDFILEWRITE;
				pList->AddTail(data);
				nRet = 1;
				break;
			}else if(data.dwDataLen == FTPERROR)
			{
				pList->AddHead(data);
				nRet = -1;
				break;
			}
			else
			{
				try{
					pFile->Write(data.buf, data.dwDataLen);
				}
				catch(CInternetException *pExc)
				{
					FtpData data;
					data.dwDataLen = FTPERROR;
					pList->AddHead(data);
					pExc->GetErrorMessage((LPTSTR)data.buf, sizeof(data.buf));
					TRACE(_T("%s\n"), (LPCTSTR)data.buf);
					pExc->Delete();
					nRet = -2;
				}
			}
		}
		else
		{
			Sleep(200);
		}
	}

	pList->SetEndWriteThread();
	TRACE(_T("CDirTree::WriteThread end %d\n"), nRet);
	return nRet;
}


BOOL CDirTree::MyCopyFtp(FtpProgressInfo *pFtpInfo)
{
	TRACE(_T("CDirTree::MyCopyFtp begin\n"));
	BOOL bRet = FALSE;
	CInternetFile* pFileFrom = NULL;
	CInternetFile* pFileTo = NULL;

	CopyInfo *pInfo = pFtpInfo->pCopyInfo;
	pInfo->Lock();
	CString sFileFrom = pInfo->sNameFrom;
	CString sFileTo = pInfo->sNameTo;
	CFtpConnection *pConFrom = pInfo->pFtpCon;
	CFtpConnection *pConTo = pInfo->pFtpConDest;

	pInfo->Unlock();

	try{		
		pFileFrom = pConFrom->OpenFile( sFileFrom, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY/*, (DWORD)pFtpInfo*/);//, DWORD dwAccess = GENERIC_READ, DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY, DWORD dwContext = 1 );
		if(pFileFrom)
		{
			pFileTo = pConTo->OpenFile( sFileTo, GENERIC_WRITE, FTP_TRANSFER_TYPE_BINARY, (DWORD)pFtpInfo);
			if(pFileTo)
			{
				FtpCopyInfo info;
				CFtpDataList list;
				info.Clear();
				info.pFtpCopyInfo = pFtpInfo;
				info.pFileFrom = pFileFrom;
				info.pFileTo = pFileTo;
				info.pList = &list;

				AfxBeginThread(ReadFtpThread, &info);
				AfxBeginThread(WriteFtpThread, &info);

				FtpData data;
				while(TRUE)
				{
					Sleep(200);
					if(!list.IsEmpty())
					{
						if(list.IsEnd())
						{
							data = list.GetHead();
							if(data.dwDataLen == FTPENDFILEWRITE || pInfo->IsCancelled())
							{
								bRet = TRUE;
								ASSERT(list.IsEnd());							
							}
							break;
						}
					}
				}				
			}
		}
	}
	catch(CInternetException *pExc)
	{
		FtpData data;
		//data.dwDataLen = FTPERROR;
		//pList->AddHead(data);
		pExc->GetErrorMessage((LPTSTR)data.buf, sizeof(data.buf));
		TRACE(_T("%s\n"), (LPCTSTR)data.buf);
		pExc->Delete();
	}

	if(pFileFrom)
	{
		pFileFrom->Close();
		delete pFileFrom;
	}

	if(pFileTo)
	{
		pFileTo->Close();
		delete pFileTo;
	}

	TRACE(_T("CDirTree::MyCopyFtp end %d\n"), bRet);

	return bRet;
}
