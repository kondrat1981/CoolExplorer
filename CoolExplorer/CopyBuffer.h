// CopyBuffer.h: interface for the CopyBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COPYBUFFER_H__A6420C79_30D7_4E64_90E2_3815BF1932C5__INCLUDED_)
#define AFX_COPYBUFFER_H__A6420C79_30D7_4E64_90E2_3815BF1932C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "myexplorer.h"

#include "ErrFile.h"


struct CMyFileStatus
{
	CTime m_ctime;          // creation date/time of file
	CTime m_mtime;          // last modification date/time of file
	CTime m_atime;          // last access date/time of file
	ULONGLONG m_size;            // logical size of file in bytes
	BYTE m_attribute;       // logical OR of CFile::Attribute enum values
	BYTE _m_padding;        // pad the structure to a WORD
	CMyFileStatus &operator =(const CFileStatus &st)
	{
		this->m_ctime = st.m_ctime;
		this->m_mtime = st.m_mtime;
		this->m_atime = st.m_atime;
		this->m_size = st.m_size;
		this->m_attribute = st.m_attribute;
		this->_m_padding = st._m_padding;
		return *this;
	}
	operator CFileStatus()
	{
		CFileStatus st;
		st.m_ctime = this->m_ctime;
		st.m_mtime = this->m_mtime;
		st.m_atime = this->m_atime;
		st.m_size = this->m_size;
		st.m_attribute = this->m_attribute;
		st._m_padding = this->_m_padding;
		_tcscpy(st.m_szFullName, _T(""));
		return st;
	}
};

class NameStatus{
public:
	NameStatus(){};
	NameStatus(LPCTSTR sFile, CFileStatus & fs) { sName = sFile; FileStat = fs; }
	CString sName;
	CMyFileStatus FileStat;
};



class BufDir{
public:
	BufDir() {};
	BufDir(LPCTSTR sName, CFileStatus & fs) { sDir = sName; fsDir = fs; }
	BufDir(const BufDir &bf) {operator = (bf);};
	const BufDir& operator = (const BufDir &bf){sDir = bf.sDir; fsDir = bf.fsDir; aFiles.Append(bf.aFiles); return *this;}
	CString sDir;
	CMyFileStatus fsDir;
	CArray<NameStatus, NameStatus &> aFiles;
};

class CopyBuffer  
{
public:
	void DeleteFiles(LPCTSTR sPath, CStringList &lNames);
	void Copy();
	static UINT CopyThread(LPVOID lParam);
	static INT_PTR RunProgress(LPVOID lParam);
	class CProgressDlg * m_pProgressDlg;
	BOOL CopyFiles(LPCTSTR sPathFrom, LPCTSTR sPathTo, CStringList & lFilesList, LPCTSTR sMask = _T("*.*"));
	BOOL Init(LPCTSTR sPath, CStringList & lFilesList);
	errors OpenNextFile(CErrFile &file, CPoint &pt, UINT nOpenFlags = CFile::modeRead, BOOL bRestoreStatus = FALSE);
	BOOL GetFiles(INT_PTR iIndex);
	CopyBuffer();
	virtual ~CopyBuffer();
	CString m_sMask;
	CString m_sPathFrom;
	CString m_sPathTo;
	CArray<BufDir, BufDir &> m_aDirs;
	CPoint m_ptFirstToCopy;
	CPoint m_ptFirstCopied;
	CPoint m_ptLastCopied;
	BOOL m_bLastCopied;
	ULONGLONG m_wLastFilePos;
	ULONGLONG m_wFirstFilePos;
	BYTE *m_pData;
	UINT m_BufferSize;
	CErrFile m_FileFrom;
	CErrFile m_FileTo;
	CPoint m_ptLastOpen;

private:
	UINT m_nCopyFlags;
	ULONGLONG m_nSourceSize;
	CString GetFileName(CPoint pt);
	errors OpenNextFileWrite(CErrFile &file, CPoint &pt);
	errors WriteBuffer();
	errors ReadBuffer();
	CString GetFromFileName(CPoint pt);
	CString GetToFileName(CPoint pt);
	errors GetNext(CPoint &pt);
	CString GetToDirName(CPoint pt);
	NameStatus &GetFile(CPoint pt);
	BOOL RemoveAt(CPoint pt);
};

#endif // !defined(AFX_COPYBUFFER_H__A6420C79_30D7_4E64_90E2_3815BF1932C5__INCLUDED_)
