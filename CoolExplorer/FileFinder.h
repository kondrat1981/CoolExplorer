#ifndef _FILE_FINDER_H
#define _FILE_FINDER_H

#include "FileInfo.h"

struct _result_list
{
public:
	CString GetFilePath() const  {return path;}
	CString GetFileName() const {return file_info.sName;}
	FileInfo GetFileInfo() const {return file_info;}
	void Init(const CString &sPath, const WIN32_FIND_DATA &fd)
	{
		path = sPath;
		file_info = fd;
	}
	private:
    //CString filename;
    CString path;
	FileInfo file_info;
};
struct _net_ff_mask
{
	BOOL bFileFind;		//search for filename
	CString sFileName;	//filename to find for
	BOOL bPCFind;		//search only PC name,pc description
	CString sPCName;	//PC name do find for
	_net_ff_mask()
	{
		bFileFind = FALSE;
		sFileName = _T("");
		bPCFind = FALSE;
		sPCName = _T("");
	}
};

enum __ff_operators
{
	ffequal = '=',
	ffsmaller = '<',
	ffbigger = '>'
};

struct _ff_mask
{
    CString _filename_mask;
	//standard settings - find substring
    BOOL bFindSubstring;
    CString sSubstring;
    BOOL bMatchCase;

    //advanced settings FILESIZE
	BOOL bAdvFileSize;
	__ff_operators iAdvFileSizeOperator;
	int iAdvFileSizeValue;
	int iAdvFileSizeModifier;

	//advanced settings FILEDATE
	BOOL bAdvDate;
	__ff_operators iAdvDateOperator;
	COleDateTime iAdvDateValue;
	int iAdvDateModifier;

	//advanced settings FILE ATTRIBUTES
	BOOL bAttrib;
	DWORD dwAttEnabled;
	DWORD dwAttDisabled;

	_ff_mask()
	{
		Clear();

	}
	void Clear()
	{
		_filename_mask = _T("");
		sSubstring = _T("");
		bAdvFileSize = FALSE;
		bAdvDate = FALSE;
        bFindSubstring = FALSE;
		bAttrib = FALSE;
		dwAttEnabled = 0;
		dwAttDisabled = 0;
		bMatchCase = FALSE;
	}
};

using namespace std;

class CFileFinder
{


public:

    CMutex m_SrchInProgressMutex;
    CCriticalSection m_UpdateListCritSection;

    std::vector <_result_list> m_result;
    BOOL m_bSInProgress;
    _ff_mask m_SrchMask;
    
	CFileFinder();    
	void Clear();

	void SetSrchMask( _ff_mask mask );
	void GetSrchMask( _ff_mask &mask );

    
};

#endif // _FILE_FINDER_H