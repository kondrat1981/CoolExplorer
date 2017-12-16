#include "stdafx.h"
#include "CommonStruct.h"

CString CSmartNameManager::GetHelp()
{
	return
	_T_LNGSTR(223) + (CString)_T(" :\"aa") + TAG_FULL_NAME + (CString)_T("|s1:s2|s3:s4|s5|\"\r\n") +
	_T_LNGSTR(224) + _T("\r\n\r\n") +
	_T_LNGSTR(222)+  (CString)_T(":\r\n") +//("Available tags:") + (CString)'\n' +
	TAG_FULL_NAME +			(CString)_T(" - ") + _T_LNGSTR(214) + _T("\r\n")+//file full name\r\n") +
	TAG_NAME +				(CString)_T(" - ") + _T_LNGSTR(215) + _T("\r\n")+//file name without extension\r\n") +
	TAG_EXT +				(CString)_T(" - ") + _T_LNGSTR(216) + _T("\r\n")+//file extension\r\n") +
	TAG_FILE_SIZE  +		(CString)_T(" - ") + _T_LNGSTR(217) + _T("\r\n")+//file size in bytes\r\n") +
	TAG_FILE_PATH +			(CString)_T(" - ") + _T_LNGSTR(218) + _T("\r\n")+//file path\r\n") +
	TAG_FILE_PATH_NAME  +	(CString)_T(" - ") + _T_LNGSTR(219) + _T("\r\n")+//file full path and name\r\n") +
	TAG_RELATIVE_PATH  +	(CString)_T(" - ") + _T_LNGSTR(220) + _T("\r\n")+//relative path\r\n") +
	TAG_ADD_NUM +			(CString)_T(" - ") + _T_LNGSTR(221) + _T("\r\n");//number (added if destination file exist)\r\n");	

}


void CSmartNameManager::RemoveTags(CString &sInOut)
{
	sInOut.Replace(TAG_FULL_NAME, _T(""));
	sInOut.Replace(TAG_NAME, _T(""));
	sInOut.Replace(TAG_EXT, _T(""));
	sInOut.Replace(TAG_FILE_SIZE, _T(""));
	sInOut.Replace(TAG_FILE_PATH, _T(""));
	sInOut.Replace(TAG_FILE_PATH_NAME, _T(""));
	sInOut.Replace(TAG_RELATIVE_PATH, _T(""));
	sInOut.Replace(TAG_ADD_NUM, _T(""));
	BOOL bNetwork = FALSE;
	if(!_tcsncmp(sInOut, _T("\\\\"), 2))
		bNetwork = TRUE;
	while(sInOut.Replace(_T("\\\\"), _T("\\")));
	if(bNetwork)
		sInOut = '\\' + sInOut;
}

BOOL CSmartNameManager::CreateName(CString sNameFrom, CString & sNameTo, CString &sDirTo, LPCTSTR sDirFrom, const WIN32_FIND_DATA * fd) const
{
	BOOL bRet = TRUE;
	CString sTmp;
	int pos = 0;
	sDirTo = this->m_sDestDirPattern;
	CString sRelativePath = _T("");
	if(!_tcsncmp(sDirFrom, m_sMainFromDir, m_sMainFromDir.GetLength()))
		sRelativePath = sDirFrom + m_sMainFromDir.GetLength();
	ASSERT(!_tcsncmp(sDirFrom, m_sMainFromDir, m_sMainFromDir.GetLength()) || !_tcsncmp(CString(sDirFrom)+'\\', m_sMainFromDir, m_sMainFromDir.GetLength()));

	//
	if(m_dwFlags == CSmartNameManager::none)
		sNameTo = sNameFrom;
	else
	{
		sNameTo = m_sDestFilePattern;
		
		if(m_dwFlags & CSmartNameManager::full_name)
		{
			sTmp = sNameFrom;
			sNameTo.Replace(TAG_FULL_NAME, sTmp);
		}
		if(m_dwFlags & CSmartNameManager::name)
		{
			if((pos = sNameFrom.ReverseFind('.')) != -1)
				sTmp = sNameFrom.Left(pos);
			else
				sTmp = sNameFrom;
			
			sNameTo.Replace(TAG_NAME, sTmp);
		}
		if(m_dwFlags & CSmartNameManager::ext)
		{
			if((pos = sNameFrom.ReverseFind('.')) != -1)
				sTmp = sNameFrom.Right(sNameFrom.GetLength() - pos - 1);
			else
				sTmp = "";
			sNameTo.Replace(TAG_EXT, sTmp);
		}
		if(m_dwFlags & CSmartNameManager::file_size)
		{
			sTmp.Format(_T("%d"), fd->nFileSizeLow);
			sNameTo.Replace(TAG_FILE_SIZE, sTmp);			
		}
		if(m_dwFlags & path)
		{
			sNameTo.Replace(TAG_FILE_PATH, sDirFrom);
		}
		if(m_dwFlags & path_name)
		{
			sNameTo.Replace(TAG_FILE_PATH_NAME, AddPathNames(sDirFrom, sNameFrom));
		}
		if(m_dwFlags & relative_path)
		{
			sNameTo.Replace(TAG_RELATIVE_PATH, sRelativePath);
		}		
	}
	//
	if(m_dwDirFlags == CSmartNameManager::none)
	{
	}
	else
	{
		sDirTo = m_sDestDirPattern;
		
		if(m_dwDirFlags & CSmartNameManager::full_name)
		{
			sTmp = sNameFrom;
			sDirTo.Replace(TAG_FULL_NAME, sTmp);
		}
		if(m_dwDirFlags & CSmartNameManager::name)
		{
			if((pos = sNameFrom.ReverseFind('.')) != -1)
				sTmp = sNameFrom.Left(pos);
			else
				sTmp = sNameFrom;
			
			sDirTo.Replace(TAG_NAME, sTmp);
		}
		if(m_dwDirFlags & CSmartNameManager::ext)
		{
			if((pos = sNameFrom.ReverseFind('.')) != -1)
				sTmp = sNameFrom.Right(sNameFrom.GetLength() - pos - 1);
			else
				sTmp = "";
			sDirTo.Replace(TAG_EXT, sTmp);
		}
		if(m_dwDirFlags & CSmartNameManager::file_size)
		{
			sTmp.Format(_T("%d"), fd->nFileSizeLow);
			sDirTo.Replace(TAG_FILE_SIZE, sTmp);			
		}
		if(m_dwDirFlags & path)
		{
			sDirTo.Replace(TAG_FILE_PATH, sDirFrom);
		}
		if(m_dwDirFlags & path_name)
		{
			sDirTo.Replace(TAG_FILE_PATH_NAME, AddPathNames(sDirFrom, sNameFrom));
		}
		if(m_dwDirFlags & relative_path)
		{
			sDirTo.Replace(TAG_RELATIVE_PATH, sRelativePath);
		}		
	}
	//
	map<CString,CString >::const_iterator it = m_mapReplace.begin();
	while(it != m_mapReplace.end())
	{
		sNameTo.Replace((*it).first, (*it).second);
		it++;
	}

	//
	it = m_mapDirReplace.begin();
	while(it != m_mapDirReplace.end())
	{
		sDirTo.Replace((*it).first, (*it).second);
		it++;
	}
	
	if(m_dwFlags & add_num)
	{
		//if(m_sMainFromDir!=m_sMainToDir)
		{
			sTmp = sNameTo;
			sTmp.Replace(TAG_ADD_NUM, _T(""));
			HANDLE hFile = CreateFile(AddPathNames(sDirTo, sTmp), 0, 0, 0, OPEN_EXISTING,0,NULL);
			if(hFile != INVALID_HANDLE_VALUE)
			{
				int nCount = 0;
				sTmp = sNameTo;
				TCHAR sNum[128]={0};
				
				while(hFile != INVALID_HANDLE_VALUE)
				{
					CloseHandle(hFile);
					sTmp = sNameTo;				
					sTmp.Replace(TAG_ADD_NUM, _itot(nCount++, sNum, 10));
					hFile = CreateFile(AddPathNames(sDirTo, sTmp), 0, 0, 0, OPEN_EXISTING,0,NULL);
				}
				sNameTo = sTmp;
			}
		}
		sNameTo.Replace(TAG_ADD_NUM, _T(""));
	}
	
	//sNameTo = AddPathNames(sDirTo, sNameTo);
	
	return TRUE;
}


/*BOOL CSmartNameManager::CreateDirName(CString & sDirTo, LPCTSTR sDirFrom) const
{
	BOOL bRet = TRUE;
	CString sTmp;
	int pos = 0;
	sDirTo = this->m_sDestDirPattern;
	CString sRelativePath = _T("");
	if(!_tcsncmp(sDirFrom, m_sMainFromDir, m_sMainFromDir.GetLength()))
		sRelativePath = sDirFrom + m_sMainFromDir.GetLength();
	ASSERT(!_tcsncmp(sDirFrom, m_sMainFromDir, m_sMainFromDir.GetLength()) || !_tcsncmp(CString(sDirFrom)+'\\', m_sMainFromDir, m_sMainFromDir.GetLength()));

	//
	if(m_dwDirFlags == CSmartNameManager::none)
	{

	}
	else
	{
		sDirTo = m_sDestDirPattern;		

		if(m_dwDirFlags & relative_path)
		{
			sDirTo.Replace(TAG_RELATIVE_PATH, sRelativePath);
		}		
	}	
	//
	
	//
	map<CString,CString >::const_iterator it = m_mapDirReplace.begin();
	while(it != m_mapDirReplace.end())
	{
		sDirTo.Replace((*it).first, (*it).second);
		it++;
	}	
	
	return TRUE;
}*/

BOOL CSmartNameManager::TestPatterns(LPCTSTR sFilePattern, LPCTSTR sDirPattern)
{
	BOOL bRet = TRUE;
	CString sMes;
	LPCTSTR sTmp = NULL;
	
	LPCTSTR tab[] = {sFilePattern, sDirPattern};
	for(int i=0; i<2; i++)
	{
		CString sPat = tab[i];
		CString sRep;
		int pos = sPat.Find('|');
		if(pos != -1)
		{
			sRep = sPat.Mid(pos);
			sPat = sPat.Left(pos);
			//sRep = sRep.SpanIncluding(_T("<>;"));
			if(_tcschr(sRep,'<') || _tcschr(sRep,'>') || _tcschr(sRep,';'))
			{
				sMes += CString(_T("Not allowed characters in Replace part ")) + _T("<>;") + ((i==0)? (CString)_T(" in file pattern\n") :(CString)_T(" in dir pattern\n"));
				bRet = FALSE;
			}
		}
		RemoveTags(sPat);
		sTmp = sPat;
		BOOL bTag = FALSE;
		CString sTag;
		while(*sTmp)
		{
			if(*sTmp == '<')
			{
				if(bTag)
					sMes += _T("Tag: ") + sTag + _T(" not ended\n");
				bTag = TRUE;
				sTag = *sTmp;
				bRet = FALSE;
			}
			else if(*sTmp == '>')
			{
				if(!bTag)
					sMes += _T("Syntax error \'>\' without \'<\'\n");
				else
				{
					sTag += *sTmp;
					sMes += _T("Unknown tag:") + sTag + '\n';
				}
				bTag = FALSE;
				sTag.Empty();
				bRet = FALSE;
			}
			else if(bTag)
			{
				sTag += *sTmp;
			}

			sTmp++;
		}
		if(bTag)
			sMes += _T("Tag: ") + sTag + _T(" not ended\n");


	}

	if(!bRet)
		AfxMessageBox(sMes, MB_ICONERROR);


	return bRet;
}

BOOL CSmartNameManager::CreateBatch(CString &sOut, LPCTSTR sDirFrom, LPCTSTR sMask, BOOL bRun)
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
						
						bRet = CreateBatch(sOut, AddPathNames(sDirFrom,fd.cFileName), sMask, bRun);
						if(bRet == -1)
							break;							

					}
				}
				else if(Compare(sMask, fd.cFileName))
				{
					CString sNewDirTo;
					CString sFileFrom = AddPathNames(sDirFrom,fd.cFileName);
					CString sFileTo;
					CreateName(fd.cFileName, sFileTo, sNewDirTo, sDirFrom, &fd);
					if(!sNewDirTo.IsEmpty())
					{
						sFileTo = AddPathNames(sNewDirTo, sFileTo);
					}
					if(bRun)
						bRet = _tsystem(sFileTo) == 0;
					sFileTo += _T("\r\n");
					sOut += sFileTo;
					
					if(!bRet)
					{
						CString sMess;
						sMess.Format( _T("Error run command:\n%s\nfor original file:\n%s"), sFileTo, sFileFrom);
						if(ErrorMessage(sMess, TRUE, MB_OKCANCEL) == IDCANCEL)
						{
							bRet = -1;
							break;
						}
					}
				}
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