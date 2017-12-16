#ifndef FILEINFO_H
#define FILEINFO_H

class FileInfo
{
public:
	CString sName;
	CString sName_Without_Ext;
	CString sExtension_;
	CString sDescription;
	LONGLONG lSize;
	//CTime	tmCreat;
	FILETIME fileTime;
	//FILETIME ftCreationTime;

	DWORD	cAttrib;
	DWORD dwShAttrib;
	FileInfo(){};
	virtual ~FileInfo(){};
	/*fileinfo(LPCTSTR name, CFileStatus *status = NULL)
	{
		ASSERT(0);
		sName = name;
		if(status)
		{
			lSize = (long)status->m_size;
			tmCreat = status->m_mtime;
			cAttrib = status->m_attribute;
		}
		dwShAttrib = 0;
		
	};*/
	FileInfo(LPCTSTR name, CFtpFileFind *ftpffind)
	{
		
		sName = name;
		
		if(ftpffind)
		{
			ASSERT(sName != _T("opera"));
			BOOL b = ftpffind->MatchesMask(-1);
			lSize = ftpffind->GetLength();
			ftpffind->GetLastWriteTime(&fileTime);
			cAttrib = 0;
			if(ftpffind->IsReadOnly())
				cAttrib |= CFile::readOnly;
			if(ftpffind->IsHidden())
				cAttrib |= CFile::hidden;
			if(ftpffind->IsSystem())
				cAttrib |= CFile::system;
			if(ftpffind->IsDirectory())
				cAttrib |= CFile::directory;
			if(ftpffind->IsArchived())
				cAttrib |= CFile::archive;   
		}

		LPCTSTR sExt = _T("");
		if(!(cAttrib & CFile::directory))
		{
			sExt = _tcsrchr(sName, '.');
			if(sExt && sExt != sName)
				sExt++;
			else
				sExt = _T("");
			
		}
		else
		{
			lSize = -1;
		}
		sExtension_ = sExt;

		dwShAttrib = 0;
	}
	void Clear()
	{
		sName = _T("");
		sDescription = _T("");
		sExtension_ = _T("");
		lSize = 0;
		fileTime.dwHighDateTime = 0;
		fileTime.dwLowDateTime = 0;
		//tmCreat = CTime();
		cAttrib = 0;
		dwShAttrib = 0;
	}

	CString getTimeString()
	{
		CString sRet = "";
		FILETIME localTime;
		if (FileTimeToLocalFileTime(&fileTime, &localTime))
		{
			SYSTEMTIME sysTime;
			if (FileTimeToSystemTime(&localTime, &sysTime))
			{
				//"%d/%m/%Y %H:%M:%S"
				sRet.Format(_T("%02d/%02d/%04d %02d:%02d:%02d"), sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			}
		}
		return sRet;
	}


	FileInfo &operator =(const WIN32_FIND_DATA &fd)
	{
		cAttrib = fd.dwFileAttributes;
		dwShAttrib = 0;
		sName = fd.cFileName;
		LPCTSTR sExt = _T("");
		LPCTSTR sSingleName_ = _T("");
		if(!(cAttrib & CFile::directory))
		{
			sExt = _tcsrchr(sName, '.');
			
			if(sExt)
				sExt++;
			else
				sExt = _T("");
			LARGE_INTEGER tmp;
			tmp.LowPart = fd.nFileSizeLow;
			tmp.HighPart = fd.nFileSizeHigh;
			lSize = tmp.QuadPart;			
		}
		else
		{
			lSize = -1;
		}
		sExtension_ = sExt;
		if(fd.ftLastWriteTime.dwHighDateTime != 0 || fd.ftLastWriteTime.dwLowDateTime != 0)
			fileTime = fd.ftLastWriteTime;
		else
			fileTime = fd.ftCreationTime;
		/*try
		{
			tmCreat = CTime(fd.ftLastWriteTime);
		}
		catch (...) //error converting / file has a incorrect last write time
		{
			try
			{
				tmCreat = CTime(fd.ftCreationTime);//CTime::GetCurrentTime();
			}
			catch(...)
			{
				tmCreat = CTime::GetCurrentTime();
			}
		}*/
		
		//tmCreat.IsValidFILETIME
		//
		
		return *this;
	}
	
	CString ToString() const
	{
		CString str;
		if(this)
			str.Format(_T("FILEINFO: sName: %s; sDescription: %s, lSize: %ld, cAttrib: %c"), (LPCTSTR)sName, (LPCTSTR)sDescription, (long)lSize, (char)cAttrib);
		else
			str = _T("FILEINFO: Nic z tego dobierasz sie nulla");
		return str;
	}
	
	
};
#endif //FILEINFO_H