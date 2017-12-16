// ErrFile.h: interface for the CErrFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRFILE_H__C57A266D_1DBF_4A3D_B673_FB250E7FA5EA__INCLUDED_)
#define AFX_ERRFILE_H__C57A266D_1DBF_4A3D_B673_FB250E7FA5EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum errors
{
	ok,
		skip,
		last,
		directory,
		fail,
		interrupt,
		file
};

class CErrFile : public CFile  
{
public:
	BOOL SkipShared(UINT value);
	BOOL SkipHidden(UINT value);
	BOOL Skipsystem(UINT value);
	BOOL SkipReadOnly(UINT value);
	BOOL OverHidden(UINT value);
	BOOL OverSystem(UINT value);
	BOOL OverReadOnly(UINT value);
	errors CErrFile::OpenFileForWrite(LPCTSTR lpszFileName, UINT &nErrroFlags);
	errors Open( LPCTSTR lpszFileName, UINT nOpenFlags, UINT &nErrroFlags);
	CErrFile();
	virtual ~CErrFile();
	void Write( const void* lpBuf, UINT nCount );
	UINT Read( void* lpBuf, UINT nCount );
	ULONGLONG Seek(LONGLONG lOff, UINT nFrom);

private:
	BOOL m_bNullFile;

};

#endif // !defined(AFX_ERRFILE_H__C57A266D_1DBF_4A3D_B673_FB250E7FA5EA__INCLUDED_)
