#ifndef COMMONSTRUCT_H_INCLUDED
#define COMMONSTRUCT_H_INCLUDED
struct SafetyStruct
{
public:
	inline BOOL Lock()
	{
		return m_critsec.Lock();
	}
	inline BOOL Unlock( )
	{
		return m_critsec.Unlock();
	}


private:
	CCriticalSection m_critsec;
};

struct TracesList : public SafetyStruct, public CStringList
{
};


struct CopyThreadInfo
{
	CString sFrom;
	CString sTo;
	CStringList lNames;
	CString sMask;
};

struct ChangeStatusInfo
{
	FILETIME tmCreation;
	FILETIME tmModified;
	FILETIME tmLastAccess;
	BOOL bCreation;
	BOOL bModified;
	BOOL bLastAccess;
	int nReadOnly;
	int nHidden;
	int nSystem;
	int nArchive;
	ChangeStatusInfo ()
	{
		bCreation = FALSE;
		bModified = FALSE;
		bLastAccess = FALSE;
		nReadOnly = BST_INDETERMINATE;
		nHidden = BST_INDETERMINATE;
		nSystem = BST_INDETERMINATE;
		nArchive = BST_INDETERMINATE;
	}
	
};

struct CopyInfo : public SafetyStruct
{
	CWnd *pWnd;
	ULONGLONG nFileCopied;
	ULONGLONG nAllCopied;
	ULONGLONG nFileSize;
	ULONGLONG nAllSize;
	CString sNameFrom;
	CString sNameTo;
	CString sDirName; //only for listing
	CString sStatus; //only for listing
	CString sDestMod;//zmodyfikowana nazwa pliku/katalogu docelowego
	CString sPrefix;
	CString sPostfix; 
private:
	DWORD dwFlags;
public:
	BOOL bNotCopyEmpty;
	BOOL bCancel;
	BOOL bPause;
	BOOL bMove;
	BOOL bQueue;
	BOOL bFtpSent;
	int nCount;
	CFtpConnection * pFtpCon;
	CFtpConnection * pFtpConDest;
	CTime time;
	BOOL bMesInErr;
	CSmartNameManager *m_pinfoSmart;
	//PCOPYPROGRESSFUN pFun;
	void Clear()
	{
		pWnd = NULL;
		nFileCopied = 0;
		nAllCopied = 0;
		nFileSize = 0;
		nAllSize = 0;
	
		sNameFrom = _T("");
		sNameTo = _T("");
		sDirName = _T("");
		sStatus = _T("");
		sDestMod = _T("");
		sPrefix = _T("");
		sPostfix = _T("");
		dwFlags = 0;
		bNotCopyEmpty = FALSE;
		bCancel = FALSE;
		nCount = 0;
		bFtpSent = FALSE;
		pFtpCon = NULL;
		pFtpConDest = NULL;
		bPause = FALSE;
		bMove = FALSE;
		bQueue = FALSE;
		bMesInErr = FALSE;
		m_pinfoSmart = NULL;
//		pFun = NULL;
	}
	inline DWORD AddFlags(DWORD dwAdd, BOOL bLock=TRUE)
	{
		if(bLock) Lock();
		dwFlags |= dwAdd;
		if(bLock) Unlock();
		return dwFlags;
	}
	
	inline BOOL CheckFlags(DWORD dwF, BOOL bLock=TRUE)
	{
		BOOL bRet = FALSE;
		if(bLock) Lock();
		bRet = (dwFlags & dwF)/* == dwF*/;
		if(bLock) Unlock();
		return bRet;
	}

	inline void SetFlags(DWORD dwF, BOOL bLock=TRUE)
	{
		BOOL bRet = FALSE;
		if(bLock) Lock();
		dwFlags = dwF;
		if(bLock) Unlock();		
	}
	inline DWORD GetFlags(BOOL bLock=TRUE)
	{
		DWORD bRes = 0;
		if(bLock) Lock();
		bRes = dwFlags;
		if(bLock) Unlock();
		return bRes;
	}

	inline BOOL IsQueue()
	{
		Lock();
		BOOL bQ = bQueue; 
		Unlock();
		return bQueue;
	}

	inline BOOL IsCancelled(BOOL bLock = TRUE)
	{
		BOOL bRet = FALSE;
		if(this)
		{
			if(bLock)
				Lock();
			bRet = (dwFlags & ABORT) || bCancel;
			if(bLock)
				Unlock();
		}
		return bRet;
	}
	CopyInfo &operator =(CopyInfo &src)
	{
		src.Lock();
		
		pWnd = src.pWnd;
		nFileCopied = src.nFileCopied;
		nAllCopied = src.nAllCopied;
		nFileSize = src.nFileSize;
		nAllSize = src.nAllSize;
		sNameFrom = src.sNameFrom;
		sNameTo = src.sNameTo;
		sDirName = src.sDirName;
		sStatus = src.sStatus;
		sDestMod = src.sDestMod;
		sPrefix = src.sPrefix;
		sPostfix =src.sPostfix;
		dwFlags = src.dwFlags;
		bNotCopyEmpty = src.bNotCopyEmpty;
		bCancel = src.bCancel;
		bPause = src.bPause;
		bMove = src.bMove;
		bQueue = src.bQueue;
		bFtpSent = src.bFtpSent;
		nCount = src.nCount;
		pFtpCon = src.pFtpCon;
		pFtpConDest = src.pFtpConDest;
		time = src.time;		
		
		src.Unlock();
		return *this;
	}
};

struct ProgressThreadInfo
{
	AFX_THREADPROC pfnThreadProc;
	CWnd *pWnd;
	CopyInfo Info;
	CString sOutFile;
	BOOL bRet;
	BOOL bInProgress;
	void Clear()
	{
		bInProgress = FALSE;
		pfnThreadProc = NULL;
		pWnd = NULL;
		Info.Clear();
		sOutFile = "";
		bRet = FALSE;
	}
};


struct FtpProgressInfo
{
	CWnd *pWnd;
	CopyInfo *pCopyInfo;
	void Clear()
	{
		pWnd = NULL;
		pCopyInfo = NULL;
	}
};

class CFtpDataList;
struct FtpCopyInfo : public SafetyStruct
{
	FtpProgressInfo *pFtpCopyInfo;
	CInternetFile* pFileTo;
	CInternetFile* pFileFrom;
	CFtpDataList* pList;

	void Clear()
	{
		pFileTo = NULL;
		pFileFrom = NULL;
		pFtpCopyInfo = NULL;
		pList = NULL;
	}
};


/*
struct DelInfo : public SafetyStruct
{
	CWnd *pWnd;
	UINT nDelCount;
	CString sName;
	BOOL bDeleted;
	DWORD dwFlags;
	BOOL bMesInErr;
	CFtpConnection * pFtpCon;
	void Clear()
	{
		pWnd = NULL;
		nDelCount = 0;
		sName = _T("");
		bDeleted = FALSE;
		dwFlags = 0;
		bMesInErr = FALSE;
		pFtpCon = NULL;
	}
	DelInfo(){Clear();}
	BOOL IsCancelled()
	{
		Lock();
		BOOL bRet = (dwFlags & ABORT);
		Unlock();
		return bRet;
	}
	//	BOOL bCancel;
};
*/

struct CDirFile
{
	CString sName;
	CString sPath;
	FILETIME m_ctime;          // creation date/time of file
	FILETIME m_mtime;          // last modification date/time of file
	FILETIME m_atime;          // last access date/time of file
	LARGE_INTEGER m_size;            // logical size of file in bytes
	//DWORD m_hsize;
	DWORD m_attribute;       // logical OR of CFile::Attribute enum values
	BYTE _m_padding;        // pad the structure to a WORD
	CDirFile &operator =(const WIN32_FIND_DATA &fd)
	{
		sName = fd.cFileName;
		m_ctime = fd.ftCreationTime;
		m_atime = fd.ftLastAccessTime;
		m_mtime = fd.ftLastWriteTime;

//		if (m_ctime.dwLowDateTime/*.GetTime()*/ == 0 && m_ctime.dwHighDateTime == 0)
//			m_ctime = m_mtime;
//		
//		if (m_atime.dwLowDateTime/*.GetTime()*/ == 0 && m_atime.dwHighDateTime == 0)
//			m_atime = m_mtime;

		m_size.LowPart = fd.nFileSizeLow;
		m_size.HighPart = fd.nFileSizeHigh;
		m_attribute = fd.dwFileAttributes;
		//this->_m_padding = st._m_padding;
		return *this;
	}

	operator WIN32_FIND_DATA()
	{
		WIN32_FIND_DATA fd = {0};
		
		_tcscpy(fd.cFileName, sName);
		fd.ftCreationTime = m_ctime;
		fd.ftLastAccessTime = m_atime;
		fd.ftLastWriteTime = m_mtime;

//		if (m_ctime.dwLowDateTime/*.GetTime()*/ == 0 && m_ctime.dwHighDateTime == 0)
//			m_ctime = m_mtime;
//		
//		if (m_atime.dwLowDateTime/*.GetTime()*/ == 0 && m_atime.dwHighDateTime == 0)
//			m_atime = m_mtime;

		fd.nFileSizeLow = m_size.LowPart;
		fd.nFileSizeHigh = m_size.HighPart;
		fd.dwFileAttributes = m_attribute;
		return fd;
	}
/*	operator CFileStatus()
	{
		CFileStatus st;
		st.m_ctime = m_ctime;
		st.m_mtime = m_mtime;
		st.m_atime = m_atime;
		st.m_size = m_size.QuadPart;
		st.m_attribute = (BYTE)m_attribute;
		st._m_padding = _m_padding;
		_tcscpy(st.m_szFullName, _T(""));
		return st;
	}*/
};
typedef CList<CDirFile, CDirFile &> CDirFilesList;

struct CFtpConSet
{
public:
	CString m_sName;
	CString m_sServer;
	CString m_sUser;
	CString m_sPassword;
	CString m_sRemoteDir;
	INTERNET_PORT m_nPort;
	BOOL m_bPassive;
	BOOL m_bSSL;
	void Clear()
	{		
		m_sName;
		m_sServer;
		m_sUser;
		m_sPassword;
		m_sRemoteDir;
		m_nPort = 0;
		m_bPassive = FALSE;		
		m_bSSL = FALSE;		
	}
	CFtpConSet() {Clear();}
};

struct EnumFilesInfo : public SafetyStruct
{
	EnumFilesInfo(){sPath = ""; bRet = FALSE;bFinished = FALSE;bCanceled = FALSE;}
	CString sPath;
	CString GetPath()
	{
		CString s;
		Lock(); 
		s = sPath;
		Unlock();
		return s;
	}
	BOOL bRet;
	BOOL GetRet()
	{
		BOOL b = FALSE;
		Lock(); 
		b = bRet;
		Unlock();
		return b;
	}
	BOOL bFinished;
	void SetFinished(BOOL bValue){Lock(); bFinished = bValue;	Unlock();}
	BOOL GetFinished()
	{
		BOOL b = FALSE;
		Lock(); 
		b = bFinished;
		Unlock();
		return b;
	}
	BOOL bCanceled;
	BOOL GetCanceled()
	{
		BOOL b = FALSE;
		Lock(); 
		b = bCanceled;
		Unlock();
		return b;
	}
	void SetCanceled(BOOL bValue){Lock(); bCanceled = bValue;	Unlock();}
};


#define TAG_FULL_NAME _T("<full_name>")
#define TAG_NAME _T("<name>")
#define TAG_EXT _T("<ext>")
#define TAG_FILE_SIZE _T("<file_size>")
#define TAG_FILE_PATH _T("<file_path>")
#define TAG_FILE_PATH_NAME _T("<file_path_name>")
#define TAG_RELATIVE_PATH _T("<relative_path>")
#define TAG_ADD_NUM _T("<add_num>")

#pragma warning( disable : 4786 )
#include <map>
struct CSmartNameManager
{
	CSmartNameManager() :m_dwFlags(none), m_dwDirFlags(none){};
	CSmartNameManager(LPCTSTR sMainDirFrom, LPCTSTR sDestFilePattern, LPCTSTR sDestDirPattern)
	{
		Init(sMainDirFrom, sDestFilePattern, sDestDirPattern);
	}

	BOOL Init(LPCTSTR sMainDirFrom, LPCTSTR sDestFilePattern, LPCTSTR sDestDirPattern)
	{
		if(TestPatterns(sDestFilePattern, sDestDirPattern))
		{
			m_sMainFromDir = sMainDirFrom;		
			m_sDestFilePattern = sDestFilePattern;
			m_sDestDirPattern = sDestDirPattern;
			return UpdateFlags();
		}
		return FALSE;		
	}
	
	enum Conversion{
		none,
		full_name = 0x01,
		name = 0x02,
		ext = 0x04,
		file_size = 0x08,
		path = 0x10,
		path_name = 0x20,
		relative_path = 0x40,
		add_num = 0x80
	};
	
	BOOL UpdateFlags()
	{
		UpdateFlags(m_mapReplace, m_dwFlags, m_sDestFilePattern);
		UpdateFlags(m_mapDirReplace, m_dwDirFlags, m_sDestDirPattern);
		
		return TRUE;
	}	


	BOOL CreateBatch(CString &sOut, LPCTSTR sDirFrom, LPCTSTR sMask, BOOL bRun);
	BOOL CreateName(CString sNameFrom, CString & sNameTo, CString &sDirTo, LPCTSTR sDirFrom, const WIN32_FIND_DATA * fd) const;
	//BOOL CreateDirName(CString & sDirTo, LPCTSTR sDirFrom) const;
	static void RemoveTags(CString &sInOut);
	static BOOL TestPatterns(LPCTSTR sFilePattern, LPCTSTR sDirPattern);
	static CString GetHelp();
	BOOL IsNeeded() {return m_dwFlags != none || m_dwDirFlags != none;}

private:
	CString m_sMainFromDir;
	//CString m_sMainToDir;
	CString m_sDestFilePattern;
	CString m_sDestDirPattern;
	//CString m_sMask;
	//BOOL bMove;
	//BOOL bCopyWithoutDirTree;
	//BOOL m_bGenerateNumIfFileExists;

	DWORD m_dwFlags;
	DWORD m_dwDirFlags;

	map<CString,CString> m_mapReplace;
	map<CString,CString> m_mapDirReplace;
	static BOOL UpdateFlags(map<CString,CString> &map_replace, DWORD &flags, CString & sPattern)
	{
		flags = none;
		map_replace.clear();
		if(sPattern.Find(TAG_FULL_NAME) != -1)
			flags |= full_name;
		if(sPattern.Find(TAG_NAME) != -1)
			flags |= name;
		if(sPattern.Find(TAG_EXT) != -1)
			flags |= ext;
		if(sPattern.Find(TAG_FILE_SIZE) != -1)
			flags |= file_size;
		if(sPattern.Find(TAG_FILE_PATH) != -1)
			flags |= path;
		if(sPattern.Find(TAG_FILE_PATH_NAME) != -1)
			flags |= path_name;
		if(sPattern.Find(TAG_RELATIVE_PATH) != -1)
			flags |= relative_path;
		if(sPattern.Find(TAG_ADD_NUM) != -1)
			flags |= add_num;

		/*CString sTmp = sDestFilePattern;
		sTmp.Replace(TAG_FULL_NAME, _T(""));
		sTmp.Replace(TAG_NAME,		_T(""));
		sTmp.Replace(TAG_EXT,		_T(""));
		sTmp.Replace(TAG_FILE_SIZE,	_T(""));
		sTmp.Replace(TAG_FILE_SIZE,	_T(""));
		sTmp.Replace(TAG_FILE_SIZE,	_T(""));
		if(sTmp.Find('%')!=-1)
			return FALSE;*/

		int pos = sPattern.Find('|');
		if(pos!= -1)
		{
			CString sTmp = sPattern.Right(sPattern.GetLength() - pos  - 1);
			sPattern.Delete(pos, sPattern.GetLength()-pos);
			
			CString sOld,sNew;
			BOOL bOld = TRUE;
			const TCHAR * p = sTmp;
			while(*p)
			{
				if(*p == '|')
				{
					if(!sOld.IsEmpty())
						map_replace[sOld] = sNew;
					bOld = TRUE;
					sOld = sNew = _T("");
				}
				else if(*p == ':')
				{
					bOld = FALSE;
				}				
				else if(bOld)
				{
					sOld += *p;
				}
				else
				{
					sNew += *p;
				}

				p++;
			}
			if(!sOld.IsEmpty())
				map_replace[sOld] = sNew;
			
		}

		return TRUE;
	}	

		

};


typedef CArray<CFtpConSet, CFtpConSet> CFtpConSetArray;
typedef CArray<LPCITEMIDLIST, LPCITEMIDLIST> ITEMIDARRAY;

class CLockInterface
{
public:
	void Lock() const {m_sect.Lock();}
	void Unlock() const {m_sect.Unlock();}
	CSyncObject &GetSynObject() const {return m_sect;}
private:
	mutable CCriticalSection m_sect;
//#define LOCK(lockobject) CSingleLock lock(&(lockobject).GetSynObject()); lock.Lock(); ASSERT(lock.IsLocked());
};

#endif//COMMONSTRUCT_H_INCLUDED