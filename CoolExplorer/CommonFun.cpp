#include "stdafx.h"
#include "UnicodeFile.h"

CString gsFilesPath;
CString gsStartPath;

BOOL AreTheSameFiles( LPCTSTR sFile1, LPCTSTR sFile2)
{	
	if(!_tcscmp(sFile1,sFile2))
		return TRUE;
	BOOL bRet = TRUE;
	HANDLE hFile1 = CreateFile(sFile1, 0, 0, 0, OPEN_EXISTING,0,NULL);
	HANDLE hFile2 = CreateFile(sFile2, 0, 0, 0, OPEN_EXISTING,0,NULL);
	if(hFile1 == INVALID_HANDLE_VALUE || hFile2 == INVALID_HANDLE_VALUE)
		bRet = FALSE;
	
	CloseHandle(hFile1);
	CloseHandle(hFile2);
	if(!bRet)
		return FALSE;

	CString sFileBack = CString(sFile2) + _T("cool.bak");
	DWORD dFileAttr = -1;
	bRet = MoveFile(sFile2, sFileBack);
	if(!bRet)
	{
		dFileAttr = GetFileAttributes(sFile2);
		SetFileAttributes(sFile2, FILE_ATTRIBUTE_NORMAL);
		if(!MoveFile(sFile2, sFileBack))
		{
			DWORD err = GetLastError();
			SetFileAttributes(sFile2, dFileAttr);
			if(err == ERROR_ACCESS_DENIED)
				return FALSE;
			else
			{
				AfxMessageBox(CString(_T_LNGSTR(10)) + sFile2);
				return TRUE;
			}
		}
	}
						///////////////
	hFile1 = CreateFile(sFile1, 0, 0, 0, OPEN_EXISTING,0,NULL);
	if(hFile1 == INVALID_HANDLE_VALUE) //the same file
		bRet = TRUE;
	else
	{
		CloseHandle(hFile1);
		bRet = FALSE;
	}
	if(!MoveFile(sFileBack, sFile2))
		AfxMessageBox(CString(_T_LNGSTR(11)) + sFileBack );

	if(dFileAttr != -1)
		SetFileAttributes(sFile2, dFileAttr);
	

	return bRet;

}


DWORD GetFileSize(LPCTSTR sFileName, DWORD *pSizeHigh /*=NULL*/)
{
	DWORD dwSize = INVALID_FILE_SIZE;
	if(pSizeHigh)
		*pSizeHigh = 0;

	HANDLE hFile = ::CreateFile(sFileName, GENERIC_READ,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL/*dwFileAtributtes*/,
			NULL);
	
	if (hFile != INVALID_HANDLE_VALUE)
	{		
		dwSize = GetFileSize(hFile, pSizeHigh);
		CloseHandle(hFile);
		
		if(dwSize==INVALID_FILE_SIZE)
		{
			ASSERT(0);
			TRACE(_T("GetFileSize Cannot open file %s"), sFileName);
		}		
	}
	return dwSize;
}

BOOL MatchAll(LPCTSTR sMask)
{
	BOOL bRet = FALSE;
	const TCHAR cSep = ';';
	LPCTSTR sTmp = NULL;

	do
	{
		sTmp = _tcschr(sMask, ';');
		if(sTmp)
		{
			bRet = !_tcsncmp(sMask, _T("*"), sTmp - sMask) || !_tcsncmp(sMask, _T("*.*"), sTmp - sMask);
			sMask = sTmp + 1;
		}
		else
		{
			bRet = !_tcscmp(sMask, _T("*")) || !_tcscmp(sMask, _T("*.*"));
			sMask = NULL;
		}

	}while(sMask && *sMask && !bRet);


	return bRet;
}


BOOL Compare( const TCHAR *szMask, const TCHAR *szName )
{
	BOOL bRet = FALSE;
	TCHAR *sAllMask = _tcsdup(szMask);
	TCHAR *sToken = sAllMask;
	TCHAR *sPrev;
	const TCHAR cSep = ';';
	
	while(*sToken)
	{
		sPrev = sToken;
		sToken = _tcschr(sToken, cSep);
		if(sToken == NULL)
			sToken = sPrev + _tcslen(sPrev) - 1;
		else		
			*sToken = 0;
		if(CompareOne(sPrev, szName))
		{
			bRet = TRUE;
			break;
		}
		sToken++;
	}

	delete [] sAllMask;
	return bRet;
}

void CorrectMask( CString & sMask )
{
	while(sMask.Replace(_T("**"), _T("*")));	
}

BOOL CompareOne( const TCHAR *szMask, const TCHAR *szName )
{
 /////////////////////////////////////////////////////////////////////////
 // check if params are valid
 if( NULL == szMask || NULL == szName || _tcslen( szMask ) == 0 || _tcslen( szName ) == 0 )
 {
  return FALSE;
 }
 if( !_tcscmp( szMask, _T("*") ) || !_tcscmp(szMask, _T("*.*")) )
 {
	 return TRUE;
 }
 
 if(_tcscmp(szMask, _T(".*")) == 0)
 {
	 if(szName[0] == '.')
		return TRUE;
	 else
		return FALSE;
 }

 TCHAR* const sMask = _tcsdup( szMask );
 TCHAR* const sFileName = _tcsdup( szName ), *sName = sFileName;
 
 /////////////////////////////////////////////////////////////////////////
 // make sure that ".*" fits to file with any extension (also without ext)
 if( _tcsstr( szMask, _T(".*") ) == szMask + _tcslen( szMask ) - 2 ) 
 {
  sMask[ _tcslen( szMask ) - 2 ] = '\0';
  TCHAR* sPos = _tcsrchr( sName, '.' );
  if( sPos != NULL )
  {
   *sPos = '\0';
  }
 }
 TCHAR cLastChar = sMask[ _tcslen(sMask) - 1 ];
 const TCHAR* sPos;
 const TCHAR* sToken, *sSeps = _T("*");
 for( sToken = _tcstok( sMask, sSeps ); sToken != NULL; sToken = _tcstok( NULL, sSeps ) )
 {
  sPos = StrStr( sName,  sToken );
  if( sToken == sMask && sPos != sName || // no '*' at the beginning
   NULL == sPos )  // pattern not found
  {
   delete [] sMask;
   delete [] sFileName;
   return FALSE;
  }
  sName = (TCHAR*)sPos;
  sPos = sToken;
 }
 ////////////////////////////////////////////////////////////////////////
 // check if "*" is the last TCHARacter (_tcstok loses this information)
 if( ('*' != cLastChar && _tcslen( sPos ) != _tcslen( sName ) ) ||
  ('*' == cLastChar && _tcslen( sPos ) >  _tcslen( sName ) ) )
 {
 
  delete [] sMask;
  delete [] sFileName;
  return FALSE;
 }
 else
 {
  delete [] sMask;
  delete [] sFileName;
  return TRUE;
 }
}
 
const TCHAR* StrStr( const TCHAR *str, const TCHAR *TCHARset )
{
 const TCHAR* sRet = NULL;
 for( ; sRet == NULL && *str; str++ )
 {
  sRet = str;
        for( short i = 0; TCHARset[i]; i++ )
  {
   if( TCHARset[i] != '?' &&
    toupper( str[i] ) != toupper( TCHARset[i] ) )
   {
    sRet = NULL;
    break;
   }
  }
 }
 return sRet;
}


CString GetAttribStr(DWORD attrib)
{
	CString str = _T("");
	if(FILE_ATTRIBUTE_ARCHIVE & attrib)
		str += 'A';
	if(FILE_ATTRIBUTE_HIDDEN & attrib)
		str += 'H';
	if(FILE_ATTRIBUTE_READONLY & attrib)
		str += 'R';
	if(FILE_ATTRIBUTE_SYSTEM & attrib)
		str += 'S';
	
	return str;
}

// if bCreate = TRUE create temp dir
CString GetTempDir(BOOL bCreate)
{
	CString sPath;
	TCHAR szTemp[2048]={0};
	GetTempPath(sizeof(szTemp)/sizeof(szTemp[0]), szTemp);
	if(_tcslen(szTemp)>0)
	{
		sPath = AddPathNames(szTemp, _T("CoolExplorer"));
		if(bCreate)
			CreateDirectory(sPath, NULL);
	}
	return sPath;
}

BOOL MyDeleteFile(LPCTSTR sFile)
{
	BOOL bRet = DeleteFile(sFile);
	/*if(!bRet)
	{
		size_t nBufLen = _tcslen(sFile) + 2;
		LPTSTR szBuf = new TCHAR[nBufLen];
		memset(szBuf, 0, nBufLen * sizeof(TCHAR));
		lstrcpy(szBuf, sFile);

		SHFILEOPSTRUCT shi = {0};
		shi.hwnd = AfxGetMainWnd()->GetSafeHwnd(); 
		shi.wFunc = FO_DELETE; 
		shi.pFrom = szBuf;
		//fop.pTo = szNewBuf;
		shi.fFlags = 0;
		shi.fAnyOperationsAborted = TRUE; 
		shi.hNameMappings = NULL; 
		shi.lpszProgressTitle = _T(""); 

		bRet = SHFileOperation(&shi) == 0;

		delete [] szBuf;
	}*/
	return bRet;
}


BOOL MyRemoveDirectory(LPCTSTR sDir, CFtpConnection *pFtpCon)
{
	BOOL bRet = FALSE;
	if(pFtpCon == NULL)
	{
		bRet = ::RemoveDirectory(sDir);
		if(FALSE == bRet)
		{
			SetFileAttributes(sDir, FILE_ATTRIBUTE_NORMAL);
			if(::RemoveDirectory(sDir))
				bRet = TRUE;
			/*else
			{
				size_t nBufLen = _tcslen(sDir) + 2;
				LPTSTR szBuf = new TCHAR[nBufLen];
				memset(szBuf, 0, nBufLen * sizeof(TCHAR));
				lstrcpy(szBuf, sDir);
				
				SHFILEOPSTRUCT shi = {0};
				shi.hwnd = AfxGetMainWnd()->GetSafeHwnd(); 
				shi.wFunc = FO_DELETE; 
				shi.pFrom = szBuf;
				//fop.pTo = szNewBuf;
				shi.fFlags = 0;
				shi.fAnyOperationsAborted = TRUE; 
				shi.hNameMappings = NULL; 
				shi.lpszProgressTitle = _T(""); 

				bRet = SHFileOperation(&shi);

				delete [] szBuf;
			}*/
		}
	}
	else
	{
		bRet = pFtpCon->RemoveDirectory(sDir);
		if(!bRet)
			ErrorMessage(CString(_T_LNGSTR(12))+sDir, TRUE);
	}

	return bRet;		
}


BOOL MyRemoveDirWithSubDirs(LPCTSTR sPath, BOOL bMessageBox)
{
	ASSERT(_tcslen(sPath)>0);
	if(_tcslen(sPath)==0)
		return FALSE;

	BOOL bRet = FALSE;
	WIN32_FIND_DATA fd;
	memset(&fd, 0, sizeof(fd));
	HANDLE hFind = FindFirstFile(AddPathNames(sPath, _T("*.*")), &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		bRet = TRUE;
		do
		{
			ASSERT(hFind != INVALID_HANDLE_VALUE);
			
			{
				if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(_tcscmp(fd.cFileName, _T(".")) && _tcscmp(fd.cFileName, _T("..")))
					{
						MyRemoveDirWithSubDirs(AddPathNames(sPath,fd.cFileName), bMessageBox);
					}
				}
				else
				{
					CString sFile = AddPathNames(sPath,fd.cFileName);
					
					if(!DeleteFile(sFile))
					{
						if(!SetFileAttributes(sFile, FILE_ATTRIBUTE_NORMAL) || !DeleteFile(sFile))
						{
							bRet = FALSE;
							ErrorMessage(CString(_T("Error Removing directory with subdirs. Error DeleteFile.")) + sFile, bMessageBox);
						}
					}
				}
			}
		}
		while(FindNextFile(hFind, &fd));
		
		FindClose(hFind);

		MyRemoveDirectory(sPath,NULL);
	}
	else
	{
		bRet = FALSE;
		ErrorMessage(CString(_T("Error Removing directory with subdirs.")) + sPath, bMessageBox);
	}

	return bRet;
}

LONGLONG CountDirSize(LPCTSTR sPath, BOOL bMessageBox)
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
						lSize += CountDirSize(AddPathNames(sPath,fd.cFileName), bMessageBox);
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
		while(FindNextFile(hFind, &fd));
		
		FindClose(hFind);
		
	}
	else
	{
		//bRet = FALSE;
		ErrorMessage(CString(_T("Error Removing directory with subdirs. Unable to count dir size.")) + sPath, bMessageBox);
	}

	return lSize;
}



BOOL RemoveTempDir()
{
	CString sPath = GetTempDir(FALSE);

	CoolTrace(0, TRUE, _T("RemoveTempDir %s"), sPath);
	
	return MyRemoveDirWithSubDirs(sPath);
}


CString AddPathNames(LPCTSTR sBase, LPCTSTR sAdd, TCHAR dir_separator, LPCTSTR sPrefix, LPCTSTR sPostfix)
{
	if(_tcslen(sBase) == 0)
		return sAdd;
	CString str = sBase;
	if(str.Right(1) != dir_separator)
		str += dir_separator;
	if(sPrefix)
		str += sPrefix;
	str += sAdd;
	if(sPostfix)
		str += sPostfix;
	return str;
}

void GetDirFile(CString &sDir, CString &sFile, LPCTSTR sAll, TCHAR cSep)
{
	sDir = _T("");
	sFile = sAll;
	size_t len = _tcslen(sAll);
	if(len>0)
	{
		LPCTSTR pSep = _tcsrchr(sAll,cSep);
		if(pSep)
		{
			if(pSep - len == sAll)
				pSep = _tcsrchr(sAll,cSep);
			if(pSep)
			{
				sDir = sAll;
				sDir = sDir.Left((int)(pSep-sAll));
				sFile = pSep+1;
			}
		}
	}
}

CString GetLastDirFromPath(LPCTSTR sPath, TCHAR dir_separator)
{
	if(_tcslen(sPath) == 0)
		return "";
	CString str = sPath;
	if(str.Right(1) == dir_separator)
		str.Delete(str.GetLength()-1);
	int pos = str.ReverseFind(dir_separator);
	if(pos != -1)
		str = str.Right(str.GetLength() - pos);
	return str;
}

CString GetErrorText(int nError)
{
	LPVOID lpBuffer;
	CString sMessage;

	if(nError == 12003)
	{
		DWORD dwError;
		DWORD dwBufferLength = 1024;
		LPTSTR szBuffer = new TCHAR[1024];
		
		if(InternetGetLastResponseInfo( &dwError, szBuffer, &dwBufferLength))
		{
			sMessage = szBuffer;
		}
		delete [] szBuffer;
	}
	else if(FormatMessage(
		  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		  NULL,
		  nError,  // requested message identifier
		   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // language identifier for requested message
		  (LPTSTR)&lpBuffer,    // pointer to message buffer
		  0,        // maximum size of message buffer
		  NULL  // pointer to array of message inserts
		))
	{
		sMessage = (LPTSTR)lpBuffer;
		LocalFree( lpBuffer );
	}
	return sMessage;
}

int ErrorMessage(LPCTSTR sFileName, BOOL bMessageBox, UINT nType /*= MB_OK*/)
{
	int nRet = IDOK;
	LPVOID lpBuffer;
	DWORD error = GetLastError();
	if(error == 12003)
	{
		DWORD dwError;
		DWORD dwBufferLength = 1024;
		LPTSTR szBuffer = new TCHAR[1024];
		
		if(InternetGetLastResponseInfo( &dwError, szBuffer, &dwBufferLength))
		{
			CString sFile = sFileName;
			if(_tcslen(sFile))
				sFile += _T("\n");
			if(bMessageBox)
				nRet = AfxMessageBox(sFile + (LPTSTR)szBuffer, nType);
			else
				TRACE(sFile + (LPTSTR)szBuffer);
		}
		delete [] szBuffer;
	}
	if(FormatMessage(
		  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		  NULL,
		  error,  // requested message identifier
		   MAKELANGID(LANG_USER_DEFAULT, SUBLANG_DEFAULT), // language identifier for requested message
		  (LPTSTR)&lpBuffer,    // pointer to message buffer
		  0,        // maximum size of message buffer
		  NULL  // pointer to array of message inserts
		))
	{
		CString sFile = sFileName;
		if(_tcslen(sFile))
			sFile += _T("\r\n");
		if(bMessageBox)
			nRet = AfxMessageBox(sFile + (LPTSTR)lpBuffer, nType);
		else
			CoolTrace(0, TRUE, sFile + (LPTSTR)lpBuffer);
		LocalFree( lpBuffer );
	}
	return nRet;
}

CString FormatSize(LONGLONG nSize, CString strSeparator,eTypeOfSize bytes)
{
	CString str;
	LONG nUnitsSize;
	BOOL bToBytes = FALSE;
	//if(m_pCFG->div_size > 1)
	{

		if ((nSize>= 0)&&(nSize < 1024))
		{
			bytes = eBytes;
		}
		
		if(bytes == eAutoBytes)
		{
			if(nSize>10*1024*1024)
				bytes = eMBytes;
			else if(nSize>10*1024)
				bytes = eKBytes;
			else
				bytes = eBytes;
		}

		switch (bytes)
		{
		case eBytes:
				nUnitsSize = (LONG)nSize;
			break;
		case eKBytes:
				nUnitsSize = (LONG)((nSize>>10));
			break;
		case eMBytes:
				nUnitsSize = (LONG)((nSize>>10)>>10);
			break;

		}
		ASSERT(nUnitsSize>=0);
		if(nUnitsSize == 0 && nSize > 0)
			nUnitsSize = 1;
		str.Format(_T("%ld"), nUnitsSize);
	}

	// dodanie przecinkow
	CString str2;
	int nLen = str.GetLength();
	int i = 0;
	while(i < nLen)
	{
		if(i && (nLen - i)%3 == 0)
			str2 = str2 + strSeparator;

		str2 += str[i];
		i++;
	}
	str = str2;
	// koniec dodawania przecinkow

		switch (bytes)
		{
		case eBytes:
				//str += CString(" ") + "bytes";
			break;
		case eKBytes:
				str += CString(" ") + "KB";
			break;
		case eMBytes:
				str += CString(" ") + "MB";
			break;

		}


	return str;
}

int gTraceLevel = 9;
TracesList glistTrace;
int gMaxTraceCount = 10000;

void CoolTrace(int nLevel, BOOL bAddTime, LPCTSTR lpszFormat, ...)
{
	if(gTraceLevel >= nLevel)
	{
		CString sTrace;
		va_list argList;
		va_start(argList, lpszFormat);
		sTrace.FormatV(lpszFormat, argList);
		va_end(argList);

		if(bAddTime)
		{
			CTime CurTime = CTime::GetCurrentTime();
			sTrace = CurTime.Format(_T("%m/%d %H:%M:%S\t")) + sTrace;
		}
		
		glistTrace.Lock();
		if(glistTrace.GetCount() > gMaxTraceCount)
			glistTrace.RemoveHead();

		glistTrace.AddTail(sTrace);
		glistTrace.Unlock();
	}
}

void WriteTraces()
{
	char sUnicodeHeader[] = {char(0xFF), char(0xFE), '\0'};
	CString sLineEnd = _T("\r\n");
	CString sFileName = AddPathNames(gsFilesPath, _T("!CoolTrace.txt"));
	FILE *file = NULL;
	file = _tfopen(sFileName, _T("ab"));	// open in binary mode

	if(file == NULL)
	{
		//open in temp dir
		TCHAR szTemp[2048]={0};
		GetTempPath(sizeof(szTemp)/sizeof(szTemp[0]), szTemp);
		if(_tcslen(szTemp)>0)
		{
			sFileName = AddPathNames(szTemp, _T("!CoolTrace.txt"));
			file = _tfopen(sFileName, _T("wb"));	// open in binary mode
		}
	}
		
	if(file)
	{
		fseek(file,0,SEEK_END);
		if(sizeof(TCHAR)>1 && ftell(file) == 0)
			fwrite(sUnicodeHeader, sizeof(char), 2, file);
		glistTrace.Lock();

		while(!glistTrace.IsEmpty())
		{
			fwrite(glistTrace.GetHead(), sizeof(TCHAR), glistTrace.GetHead().GetLength(), file);
			fwrite(sLineEnd, sizeof(TCHAR), sLineEnd.GetLength(), file);
			glistTrace.RemoveHead();
		}
		glistTrace.Unlock();

		fclose(file);
	}
}
/*
void TraceToFile(LPCTSTR lpszFormat, ...)
{
	CTime CurTime = CTime::GetCurrentTime();
	CString TraceStr = CurTime.Format(_T("%m/%d %H:%M:%S\t"));
	ASSERT(AfxIsValidString(lpszFormat, -1));
//	mutex_trace.Lock(5000);
	CString sTrace;
	va_list argList;
	va_start(argList, lpszFormat);
	sTrace.FormatV(lpszFormat, argList);
	va_end(argList);

	CString sTraceFileName = AddPathNames(gsFilesPath, _T("!WinTrace.txt"));

	CFileStatus fs;

	CFile::GetStatus(sTraceFileName, fs);

	FILE * file_trace;

	if(fs.m_size  > 5 * 1024 * 1024)
		// wykasowanie zawartosci pliku
		file_trace = _tfopen(sTraceFileName, _T("wt"));
	else
		//dodanie trace'ów na koniec
		file_trace = _tfopen(sTraceFileName, _T("a+t"));
	if(!file_trace)
	{
		//AfxMessageBox((CString)_T("Error tracing\n") + *this);
		return;
	}
 	_fputts(TraceStr + sTrace, file_trace);
	//SetEndOfFile(file_trace);
//	_fputts(_T("\n"), file_trace);
	fclose(file_trace);
//mutex_trace.Unlock();
}*/


CString GetParentDirName(LPCTSTR sDir, TCHAR dir_separator)
{
	CString sParent = sDir;
	while(sParent.Right(1) == dir_separator)
		sParent.Delete(sParent.GetLength() - 1);
	int pos = sParent.ReverseFind(dir_separator);
	if(pos != -1)
		sParent = sParent.Left(pos);
	else
		sParent = _T("");

	return sParent;	
}

BOOL RecurseCreateDir(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	BOOL bRet = TRUE;
	if(_tcslen(lpPathName) == 0) // empty path - error
		return FALSE;
	
	if(!ExistsPath(lpPathName))
	{
		bRet = RecurseCreateDir(GetParentDirName(lpPathName), lpSecurityAttributes);
		if(bRet)
			bRet = CreateDirectory(lpPathName, lpSecurityAttributes);
	}

	return bRet;
}

BOOL MyCreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	BOOL bRet = TRUE; // optymista
	if(CreateDirectory(lpPathName, lpSecurityAttributes) == FALSE)
	{
		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile(lpPathName, &fd);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			ASSERT(0);
			bRet = FALSE;
			CString errmsg;
			errmsg.Format(_T("Error Creating Directory:%s"),lpPathName);
			AfxMessageBox(errmsg);
		}
		else
		{
			FindClose(hFind);
		}
	}
	return bRet;
}

BOOL MySetFileStatus(LPCTSTR sFileName, CDirFile *status, const ChangeStatusInfo *recInfo)
{
	DWORD dwOldStatus;
	//CString sFileName = AddPathNames(sPath, status->sName, '\\');

	if(recInfo)
	{
		dwOldStatus = status->m_attribute;
		ConvertStatus(status, recInfo);
	}
	else
	{
		dwOldStatus = GetFileAttributes(sFileName);
	}
	//CFile::SetStatus(AddPathNames(sPath, fs->sName, '\\'), *fs);
	//	DWORD wAttr;
//	FILETIME creationTime;
//	FILETIME lastAccessTime;
//	FILETIME lastWriteTime;
	LPFILETIME lpCreationTime = NULL;
	LPFILETIME lpLastAccessTime = NULL;
	LPFILETIME lpLastWriteTime = NULL;

	if(!recInfo || recInfo->bCreation || recInfo->bLastAccess || recInfo->bModified)
	{

		if(dwOldStatus & FILE_ATTRIBUTE_READONLY)
		{
			dwOldStatus &= ~FILE_ATTRIBUTE_READONLY;
			if (!SetFileAttributes((LPCTSTR)sFileName,  dwOldStatus))
			{
				ErrorMessage();
				return FALSE;
			}
		}

		if ((!recInfo || recInfo->bModified))
		{
//			AfxTimeToFileTime(status->m_mtime, &lastWriteTime);
			lpLastWriteTime = &status->m_mtime;
		}
		// last access time
		if ((!recInfo || recInfo->bLastAccess))
		{
//			AfxTimeToFileTime(status->m_atime, &lastAccessTime);
			lpLastAccessTime = &status->m_atime;
		}

		// create time
		if ((!recInfo || recInfo->bCreation))
		{
//			AfxTimeToFileTime(status->m_ctime, &creationTime);
			lpCreationTime = &status->m_ctime;
		}

		DWORD dwFileAtributtes;
		if(status->m_attribute & FILE_ATTRIBUTE_DIRECTORY)
			dwFileAtributtes = status->m_attribute | FILE_FLAG_BACKUP_SEMANTICS;
		else
			dwFileAtributtes = status->m_attribute | FILE_ATTRIBUTE_NORMAL;

		HANDLE hFile = ::CreateFile(sFileName, GENERIC_READ|GENERIC_WRITE,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, dwFileAtributtes,
			NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			// w win9x nie mozna uzyskac uchwytu do katalogu
			if(status->m_attribute & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(dwOldStatus != status->m_attribute)
				{
					if (!SetFileAttributes((LPCTSTR)sFileName, (DWORD)status->m_attribute))
					{
						ErrorMessage(sFileName);
						return FALSE;
					}					
				}
				return TRUE;
			}
			else
			{
				ErrorMessage(sFileName);
				return FALSE;
			}
		}

		if (!SetFileTime(hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime))
		{
			ErrorMessage(sFileName);
			if (!::CloseHandle(hFile))
				ErrorMessage(sFileName);
			return FALSE;
		}

		if (!::CloseHandle(hFile))
		{
			ErrorMessage(sFileName);
			return FALSE;
		}
		
	}

	if(dwOldStatus != status->m_attribute)
	{
		if (!SetFileAttributes((LPCTSTR)sFileName, (DWORD)status->m_attribute))
		{
			ErrorMessage(sFileName);
			return FALSE;
		}
	}
	return TRUE;
}

void ConvertStatus(CDirFile *fs, const ChangeStatusInfo *recInfo)
{
	if(recInfo->bCreation)
		fs->m_ctime = recInfo->tmCreation;
	if(recInfo->bLastAccess)
		fs->m_atime = recInfo->tmLastAccess;
	if(recInfo->bModified)
		fs->m_mtime = recInfo->tmModified;

// korekcja czasu tzn. uniemozliwienie ustawienia daty modyfikacji wczsniejszej niz utworzenia pliku
// nie jest to jednak dobry pomysl - ograniczenie mozliwosci programu i wykonywanie czegos czego uzytkownik nie chce
/*
	if(recInfo->bModified)
	{
		if(fs->m_ctime > fs->m_mtime)
			fs->m_ctime = fs->m_mtime;
	}
	else if(recInfo->bCreation)
	{
		if(fs->m_ctime > fs->m_mtime)
			fs->m_mtime = fs->m_ctime;
	}
*/
	if(recInfo->nReadOnly == BST_UNCHECKED)
		fs->m_attribute &= ~FILE_ATTRIBUTE_READONLY;
	else if(recInfo->nReadOnly == BST_CHECKED)
		fs->m_attribute |= FILE_ATTRIBUTE_READONLY;

	if(recInfo->nHidden == BST_UNCHECKED)
		fs->m_attribute &= ~FILE_ATTRIBUTE_HIDDEN;
	else if(recInfo->nHidden == BST_CHECKED)
		fs->m_attribute |= FILE_ATTRIBUTE_HIDDEN;

	if(recInfo->nSystem == BST_UNCHECKED)
		fs->m_attribute &= ~FILE_ATTRIBUTE_SYSTEM;
	else if(recInfo->nSystem == BST_CHECKED)
		fs->m_attribute |= FILE_ATTRIBUTE_SYSTEM;

	if(recInfo->nArchive == BST_UNCHECKED)
		fs->m_attribute &= ~FILE_ATTRIBUTE_ARCHIVE;
	else if(recInfo->nArchive == BST_CHECKED)
		fs->m_attribute |= FILE_ATTRIBUTE_ARCHIVE;
}

BOOL OleTimeToSystem(SYSTEMTIME &systm, const COleDateTime & oletm)
{
//	memset(&systm, 0, sizeof(systm));
	systm.wYear = oletm.GetYear();
	systm.wMonth = oletm.GetMonth();
	systm.wDayOfWeek = oletm.GetDayOfWeek();
	systm.wDay = oletm.GetDay();
	systm.wHour = oletm.GetHour();
	systm.wMinute = oletm.GetMinute();
	systm.wSecond = oletm.GetSecond();
	systm.wMilliseconds = 0;
	return TRUE; // co zrobic
}


BOOL OleTimeToUTCFileTime(FILETIME &filetm, const COleDateTime & oletm)
{
	SYSTEMTIME systime;
	FILETIME locfiletime;
	
	if (!OleTimeToSystem(systime, oletm))
		return FALSE;

	if (!SystemTimeToFileTime(&systime, &locfiletime))
		return FALSE;

	if (!LocalFileTimeToFileTime(&locfiletime, &filetm))
		return FALSE;

	return TRUE;
}

eWindowsVersion eReturnWinVersion()
{
	eWindowsVersion eWinVer = eWinUnknown;
	OSVERSIONINFO VI;

	memset((void*)&VI,'\0',sizeof(OSVERSIONINFO));
	VI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (GetVersionEx(&VI))
	{
		switch(VI.dwPlatformId)
		{
		case 1:
			{
				if (VI.dwMajorVersion == 4)
				{
					if (VI.dwMinorVersion == 0)
					{
						if (VI.dwBuildNumber == 950)
						{
							eWinVer = eWin95;

						}
						else if ((VI.dwBuildNumber > 950) 
							&& (VI.dwBuildNumber <= 1080))
						{
							eWinVer = eWin95_SP1;
						}

					}

					if ((VI.dwMinorVersion < 10)
						&& (VI.dwBuildNumber > 1080))
					{
						eWinVer = eWin95_OSR2;
					}
					
					if ( VI.dwMinorVersion == 10 )
					{
						if (VI.dwBuildNumber == 1998)
						{
							eWinVer = eWin98;
						} 
						else if ((VI.dwBuildNumber > 1998) && (VI.dwBuildNumber < 2183))
						{
							eWinVer = eWin98_SP1;
						}
						else if (VI.dwBuildNumber >= 2183)
						{
							eWinVer = eWin98_SE;
						}
					}
					
					if ((VI.dwMinorVersion == 90) && 	(VI.dwBuildNumber == 3000))
					{
						eWinVer = eWinMe;
					}
				}
			}
			break;
		case 2:
			{
				if (VI.dwMajorVersion == 3)
				{
					eWinVer = eWinNT_3_51;
				}
				else if (VI.dwMajorVersion == 4)
				{
					eWinVer = eWinNT_4;
				} 
				else if (VI.dwMajorVersion == 5)
				{
					if (VI.dwMinorVersion == 0)
					{
						eWinVer = eWin2000;
					}
					else if (VI.dwMinorVersion == 1)
					{
						eWinVer = eWinXP;
					}
				}
			}
			break;
		case 3:
			{
				if (VI.dwMajorVersion == 1)
				{
					eWinVer = eWinCE_1_0;
				}
				else if (VI.dwMajorVersion == 2) 
				{
					if (VI.dwBuildNumber == 0)
					{
						eWinVer = eWinCE_2_0;
					}
					else if (VI.dwBuildNumber == 0)
					{
						eWinVer = eWinCE_2_1;

					}
				}
				else if ( VI.dwMajorVersion == 3)
				{
					eWinVer = eWinCE_3_0;
				}
					 
			}
			break;

		}

	}

	return eWinVer;
}

BOOL ExistsPath(LPCTSTR sPath)
{
	WIN32_FIND_DATA fd = {0};
	HANDLE hFind = FindFirstFile(AddPathNames(sPath, _T("*.*")), &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return TRUE;
	}
	return FALSE;
}

BOOL IsFileSystem(LPCTSTR sPath)
{
	if(_tcsncmp(sPath, _T("\\\\"), 2))
	{
		CString sTmp = sPath;
		int nPos = sTmp.Find('\\');
		if(nPos != -1 && nPos < sTmp.GetLength()-1)
			sTmp.SetAt(nPos+1,0);
		if(GetDriveType(sTmp) == DRIVE_FIXED)
		{
			WIN32_FIND_DATA fd;
			memset(&fd, 0, sizeof(fd));
			HANDLE hFind = FindFirstFile(AddPathNames(sPath, _T("*.*")), &fd);
			if(hFind != INVALID_HANDLE_VALUE)
			{
				FindClose(hFind);
				return TRUE;
			}
		}
	}
	
	return FALSE;
}




BOOL CopyFrom(LPCTSTR sDirFrom, LPCTSTR sDirTo, BOOL bMove, const CSmartNameManager *pInfo)
{


	BOOL bRet = FALSE;
	WIN32_FIND_DATA fd;
	memset(&fd, 0, sizeof(fd));
	HANDLE hFind = FindFirstFile(AddPathNames(sDirFrom, _T("*.*")), &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		bRet = TRUE;
		do
		{
			ASSERT(hFind != INVALID_HANDLE_VALUE);
			
			{
				if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(_tcscmp(fd.cFileName, _T(".")) && _tcscmp(fd.cFileName, _T("..")))
					{
						//if(pInfo->bCopyWithoutDirTree )
						//	bRet = CopyFrom(AddPathNames(sDirFrom,fd.cFileName), sDirTo, pInfo);
						//else
							bRet = CopyFrom(AddPathNames(sDirFrom,fd.cFileName), AddPathNames(sDirTo, fd.cFileName), bMove, pInfo);
						if(bRet == -1)
							break;							

					}
				}
				/*else if(Compare(pInfo->m_sMask, fd.cFileName))
				{
					CString sNewDirTo;
					CString sFileFrom = AddPathNames(sDirFrom,fd.cFileName);
					CString sFileTo;
					pInfo->CreateName(fd.cFileName, sFileTo, sNewDirTo, sDirFrom, &fd);
					sFileTo = AddPathNames(sNewDirTo, sFileTo);

					//if(!pInfo->bCopyWithoutDirTree)
						RecurseCreateDir(sDirTo,NULL);
					
					if(bMove)
					{
						bRet = MoveFile(sFileFrom, sFileTo);
					}
					else
					{
						bRet = CopyFile(sFileFrom, sFileTo, TRUE);
					}
					
					
					if(!bRet)
					{
						CString sMess;
						sMess.Format( _T("Error copy/move file\nfrom:\t%s\nto:\t%s\n"), sFileFrom, sFileTo);
						if(ErrorMessage(sMess, TRUE, MB_OKCANCEL) == IDCANCEL)
						{
							bRet = -1;
							break;
						}
					}
				}*/
			}
		}
		while(FindNextFile(hFind, &fd));
		
		FindClose(hFind);
		
	}
	else
	{
		bRet = FALSE;		
	}

	return bRet;
}

BOOL CreateBatch(CString &sOut, LPCTSTR sDirFrom, const CSmartNameManager *pInfo);

BOOL CreateBatch(LPCTSTR sBatchFile, LPCTSTR sDirFrom, const CSmartNameManager *pInfo)
{
	BOOL bRet = TRUE;
	CString sOut;
	bRet = CreateBatch(sOut, sDirFrom, pInfo);

	if(!CUnicodeFile::WriteToFile(sBatchFile, sOut, TRUE))
	{
		CString sMes;
		sMes.Format(_T("Cannot open file: %s"), sBatchFile);
		ErrorMessage(sMes);
		bRet = FALSE;
	}
	return bRet;

}

BOOL CreateBatch(CString &sOut, LPCTSTR sDirFrom, const CSmartNameManager *pInfo)
{
	BOOL bRet = FALSE;
	WIN32_FIND_DATA fd;
	memset(&fd, 0, sizeof(fd));
	HANDLE hFind = FindFirstFile(AddPathNames(sDirFrom, _T("*.*")), &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		bRet = TRUE;
		do
		{
			ASSERT(hFind != INVALID_HANDLE_VALUE);
			
			{
				if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(_tcscmp(fd.cFileName, _T(".")) && _tcscmp(fd.cFileName, _T("..")))
					{
						
						bRet = CreateBatch(sOut, AddPathNames(sDirFrom,fd.cFileName), pInfo);
						if(bRet == -1)
							break;							

					}
				}
				/*else if(Compare(pInfo->m_sMask, fd.cFileName))
				{
					CString sNewDirTo;
					CString sFileFrom = AddPathNames(sDirFrom,fd.cFileName);
					CString sFileTo;
					pInfo->CreateName(fd.cFileName, sFileTo, sNewDirTo, sDirFrom, &fd);
					
					sFileTo += _T("\r\n");
					sOut += sFileTo;
					
					if(!bRet)
					{
						CString sMess;
						sMess.Format( _T("Error copy/move file\nfrom:\t%s\nto:\t%s\n"), sFileFrom, sFileTo);
						if(ErrorMessage(sMess, TRUE, MB_OKCANCEL) == IDCANCEL)
						{
							bRet = -1;
							break;
						}
					}
				}*/
			}
		}
		while(FindNextFile(hFind, &fd));
		
		FindClose(hFind);
		
	}
	else
	{
		bRet = FALSE;		
	}

	return bRet;
}


DWORD_PTR MySHGetFileInfo(LPCTSTR pszPath,DWORD dwFileAttributes, SHFILEINFO *psfi,UINT cbFileInfo, UINT uFlags)
{
	static CCriticalSection sect;
	sect.Lock();
	DWORD_PTR ret = 0;
	try
	{
		ret = SHGetFileInfo( pszPath, dwFileAttributes,  psfi, cbFileInfo, uFlags);
	}
	catch(...)
	{
		ASSERT(0);
	}
	sect.Unlock();
	return ret;
}
