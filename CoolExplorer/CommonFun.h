#ifndef _COMMONFUN_H
#define _COMMONFUN_H

enum eWindowsVersion
{
	eWinUnknown = 0,
	eWin95 = 1,
	eWin95_SP1,
	eWin95_OSR2,
	eWin98,
	eWin98_SP1,
	eWin98_SE,
	eWinMe,
	eWinNT_3_51,
	eWinNT_4,
	eWin2000,
	eWinXP,
	eWinCE_1_0,
	eWinCE_2_0,
	eWinCE_2_1,
	eWinCE_3_0
};


enum eTypeOfSize
{
	eBytes = 0,
	eKBytes,
	eMBytes,
	eAutoBytes

};


int CALLBACK comp_names (const void *, const void *, int Crit);
int comp_names (const void *pEl1, const void *pEl2);
int comp_dates(const void *pEl1, const void *pEl2);
int comp_sizes(const void *pEl1, const void *pEl2);
int comp_names_desc (const void *pEl1, const void *pEl2);
int comp_dates_desc (const void *pEl1, const void *pEl2);
int comp_sizes_desc (const void *pEl1, const void *pEl2);
BOOL IsFileSystem(LPCTSTR sPath);
//CString GetDrive(LPCTSTR sPath);
BOOL ExistsPath(LPCTSTR sPath);


eWindowsVersion eReturnWinVersion();
CString			GetErrorText(int nError);
int				ErrorMessage(LPCTSTR sFileName = _T(""), BOOL bMessageBox = TRUE, UINT nType = MB_OK);
LONGLONG		CountDirSize(LPCTSTR sPath, BOOL bMessageBox = FALSE);
BOOL			MyDeleteFile(LPCTSTR sFile);
BOOL			MyRemoveDirectory(LPCTSTR sDir, CFtpConnection *pFtpCon);
BOOL			MyRemoveDirWithSubDirs(LPCTSTR sPath, BOOL bMessageBox = FALSE);
BOOL			RemoveTempDir();
CString			GetTempDir(BOOL bCreate = TRUE);
CString			FreeTempDir();
CString			AddPathNames(LPCTSTR sBase, LPCTSTR sAdd, TCHAR dir_separator = '\\', LPCTSTR sPrefix=NULL, LPCTSTR sPostfix=NULL);
void			GetDirFile(CString &sDir, CString &sFile, LPCTSTR sAll, TCHAR cSep = '\\');
BOOL			MatchAll(LPCTSTR sMask);
BOOL			Compare( const TCHAR *szMask, const TCHAR *szName );
CString			GetAttribStr(DWORD attrib);
CString			FormatSize(LONGLONG nSize, CString strSeparator = _T(","), eTypeOfSize bytes = eKBytes);
DWORD			GetFileSize(LPCTSTR sFileName, DWORD *pSizeHigh =NULL);
const TCHAR*	StrStr(const TCHAR *str, const TCHAR *TCHARset );
void			CorrectMask( CString & sMask );
BOOL			CompareOne( const TCHAR *szMask, const TCHAR *szName );
DWORD			GetFileSize(LPCTSTR sFileName, DWORD *pSizeHigh);
//void			TraceToFile(LPCTSTR lpszFormat, ...);
void			CoolTrace(int nLevel, BOOL bAddTime, LPCTSTR lpszFormat, ...);
void			WriteTraces();
BOOL			MyCreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
struct			CDirFile;
struct			ChangeStatusInfo;
BOOL			MySetFileStatus(LPCTSTR sPath, CDirFile *status, const ChangeStatusInfo *recInfo=NULL);
void			ConvertStatus(CDirFile *fs, const ChangeStatusInfo *recInfo);
BOOL			OleTimeToUTCFileTime(FILETIME &filetm, const COleDateTime & oletm);
void CALLBACK	MyFtpCallBack(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
struct CSmartNameManager;
BOOL			CopyFrom(LPCTSTR sDirFrom, LPCTSTR sDirTo, BOOL bMove, const CSmartNameManager *pInfo);
BOOL			CreateBatch(LPCTSTR sBatchFile, LPCTSTR sDirFrom, const CSmartNameManager *pInfo);
CString			GetParentDirName(LPCTSTR sDir, TCHAR dir_separator = '\\');
BOOL			RecurseCreateDir(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
DWORD_PTR		MySHGetFileInfo(LPCTSTR pszPath,DWORD dwFileAttributes, SHFILEINFO *psfi,UINT cbFileInfo, UINT uFlags);



BOOL			AreTheSameFiles( LPCTSTR sFile1, LPCTSTR sFile2);
template<class T> void SortAndRemoveDuplicates(T &vect)
{
	std::sort(vect.begin(), vect.end());
	T::iterator curr = vect.begin();
	T::iterator pos = vect.begin();
	//pos++;
	//T::interator prev = vect.end();
	while(pos != vect.end() && (pos+1) != vect.end())
	{
		if(*curr == *(pos+1))
		{
			pos++;
		}
		else
		{
			if(curr != pos)
				*curr = *pos;
			pos++;
			curr++;
		}		
	}
	if(pos != curr)
		vect.erase(curr);
}

template<class T> void Remove(std::vector<T> &vect, T elToDel)
{
	std::sort(vect.begin(), vect.end());
	std::vector<T>::iterator curr = vect.begin();
	std::vector<T>::iterator pos = vect.begin();
	//pos++;
	//T::interator prev = vect.end();
	while(pos != vect.end())
	{
		if(*pos == elToDel)
		{
			pos++;
		}
		else
		{
			if(curr != pos)
				*curr = *pos;
			pos++;
			curr++;
		}		
	}
	if(pos != curr)
		vect.erase(curr);
}


template <class T>
T FindInsensitiveT(LPCTSTR str, LPCTSTR string, T &llFileSize)
{
	if(string == NULL || *string == '\0')
		return 0;

	const TCHAR *pos_string = string;
	const TCHAR *pos = (const TCHAR *)str ;
	const TCHAR *end = (const TCHAR *)str + llFileSize - _tcslen(string) + 1;

	const TCHAR *pos_find;
	const TCHAR *pos_start = pos;

 while(pos<end)
 {
  pos_find = pos;
  while(_tolower(*pos_string) == _tolower(*pos))
  {
   pos_string++;
   pos++;
   if(!*pos_string)
    return pos_find-(const TCHAR *)str;
  }
  if(pos_string != string)
  {
   pos_string = string;
   pos = pos_find+1;
  }
  else
   pos++;
 }
 
 return -1;
}
template <class T>
T FindT(LPCTSTR str, LPCTSTR string, T &llFileSize)
{
	if(string == NULL || *string == '\0')
		return 0;

	const TCHAR *pos_string = string;
	const TCHAR *pos = (const TCHAR *)str ;
	const TCHAR *end = (const TCHAR *)str + llFileSize - _tcslen(string) + 1;

	const TCHAR *pos_find;
	const TCHAR *pos_start = pos;

 while(pos<end)
 {
  pos_find = pos;
  while(*pos_string == *pos)
  {
   pos_string++;
   pos++;
   if(!*pos_string)
    return pos_find-(const TCHAR *)str;
  }
  if(pos_string != string)
  {
   pos_string = string;
   pos = pos_find+1;
  }
  else
   pos++;
 }
 
 return -1;
}

template <class T>
T Find_ANSI_InsensitiveT(const char* str, const char* string, T &llFileSize)
{
	if(string == NULL || *string == '\0')
		return 0;

	const char *pos_string = string;
	const char *pos = (const char *)str ;
	const char *end = (const char *)str + llFileSize - strlen(string) + 1;

	const char *pos_find;
	const char *pos_start = pos;

 while(pos<end)
 {
  pos_find = pos;
  while(tolower(*pos_string) == tolower(*pos))
  {
   pos_string++;
   pos++;
   if(!*pos_string)
    return pos_find-(const char *)str;
  }
  if(pos_string != string)
  {
   pos_string = string;
   pos = pos_find+1;
  }
  else
   pos++;
 }
 
 return -1;
}

template <class T>
T Find_ANSI_T(const char* str, const char* string, T &llFileSize)
{
	if(string == NULL || *string == '\0')
		return 0;

	const char *pos_string = string;
	const char *pos = (const char *)str ;
	const char *end = (const char *)str + llFileSize - strlen(string) + 1;

	const char *pos_find;
	const char *pos_start = pos;

 while(pos<end)
 {
  pos_find = pos;
  while(*pos_string == *pos)
  {
   pos_string++;
   pos++;
   if(!*pos_string)
    return pos_find-(const char *)str;
  }
  if(pos_string != string)
  {
   pos_string = string;
   pos = pos_find+1;
  }
  else
   pos++;
 }
 
 return -1;
}

#endif _COMMONFUN_H