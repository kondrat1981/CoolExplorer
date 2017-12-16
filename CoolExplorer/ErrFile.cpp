// ErrFile.cpp: implementation of the CErrFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoolExplorer.h"
#include "ErrFile.h"
#include "QuestionDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CErrFile::CErrFile()
{
	m_bNullFile = TRUE;
}

CErrFile::~CErrFile()
{
}

errors CErrFile::OpenFileForWrite(LPCTSTR lpszFileName, UINT &nErrroFlags)
{
	m_bNullFile = TRUE;

	HANDLE hFile = CreateFile(lpszFileName, 0, 0, 0, OPEN_EXISTING,0,NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		CString str;
		str.Format(_T("File %s exist in destimation dir\nOverwrite this file?"), lpszFileName);
		
		if(nErrroFlags & SKIP_ALL)
			return skip;

		if(!(nErrroFlags & YES_TO_ALL))
		{
			CQuestionDlg dlg(str, CQuestionDlg::OverWrite);
			switch(dlg.DoModal())
			{
				case SKIP:
					return skip;
				case SKIP_ALL:
					nErrroFlags |= SKIP_ALL;
					return skip;
				case YES:
					break;
				case YES_TO_ALL:
					nErrroFlags |= YES_TO_ALL;
					break;
				case ABORT:
					nErrroFlags |= ABORT;
					return interrupt;
			}
		}
		if(FALSE == DeleteFile(lpszFileName))
		{
			//ErrorMessage();
			//usuniecie atrybutu read only, sytem itd...
			//i ponowna proba usuniecia

			// wazne : mozna wstawic pytanie o usuniecie pliku read only, sytemowego lub ukrytego
			if(FALSE == SetFileAttributes(lpszFileName, FILE_ATTRIBUTE_NORMAL)
				|| FALSE == DeleteFile(lpszFileName))
			{
				CString str;
				str.Format(_T("CannotDeleteFile %s\n"), lpszFileName);
				ErrorMessage(str);
			
				return interrupt;
			}			
		}		
	}
	m_bNullFile = FALSE;

	return Open(lpszFileName, CFile::modeCreate | CFile::modeWrite, nErrroFlags);
}


errors CErrFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags, UINT &nErrroFlags)
{
	m_bNullFile = TRUE;
	CFileException Error;
	BOOL bRetry = FALSE;
	do
	{
		if(FALSE == CFile::Open(lpszFileName, nOpenFlags, &Error))
		{
			switch(Error.m_cause)
			{
			case CFileException::sharingViolation:
				if(SkipShared(nErrroFlags))
				{
					return skip;
				}
				else
				{
					CString str;
					str.Format(_T("File %s.\n Sharing violation."), lpszFileName);
					CQuestionDlg dlg(str, CQuestionDlg::Shared);
					switch(dlg.DoModal())
					{
					case SKIP:
						return skip;						
					case SKIP_ALL:
						nErrroFlags |= SKIP_ALL;
						return skip;
					case SKIP_ALL_THIS_TYPE:
						nErrroFlags |= SKIP_ALL_SHARED;
						return skip;
					case YES:
						bRetry = TRUE;
						break;
					case ABORT:
						nErrroFlags |= ABORT;
						//ASSERT(bRetry == FALSE);
						return interrupt;
					default: 
						ASSERT(0);
						break;
					}
				}
				break;
			
			default:
				if(SkipShared(nErrroFlags))
				{
					return skip;
				}
				else
				{
					CString str;
					str.Format(_T("File %s.\n Access Denied."), lpszFileName);
					CQuestionDlg dlg(str, CQuestionDlg::AccessDenied);
					switch(dlg.DoModal())
					{
					case SKIP:
						return skip;						
					case SKIP_ALL:
						nErrroFlags |= SKIP_ALL;
						return skip;
					case SKIP_ALL_THIS_TYPE:
						nErrroFlags |= SKIP_ALL_SHARED;
						return skip;
					case YES:
						bRetry = TRUE;
						break;
					case ABORT:
						nErrroFlags |= ABORT;
						//ASSERT(bRetry == FALSE);
						return interrupt;
					default: 
						ASSERT(0);
						break;
						}
					}
				break;
			}
		}
		else
		{
			m_bNullFile = FALSE;
			return ok;
		}
	}while(bRetry);
	ASSERT(FALSE);
	return fail;
}

void CErrFile::Write( const void* lpBuf, UINT nCount )
{
	if(!m_bNullFile)
		CFile::Write(lpBuf, nCount);

	/* tu */ 
}

UINT CErrFile::Read( void* lpBuf, UINT nCount )
{
	if(!m_bNullFile)
		return CFile::Read(lpBuf, nCount);
	return nCount;
}

ULONGLONG CErrFile::Seek(LONGLONG lOff, UINT nFrom)
{
	if(!m_bNullFile)
		return CFile::Seek(lOff, nFrom);
	return 0;
}



BOOL CErrFile::OverReadOnly(UINT value)
{
	return	   value & YES_TO_ALL_READ_ONLY
			|| value & YES_TO_ALL;
}

BOOL CErrFile::OverSystem(UINT value)
{
	return	   value & YES_TO_ALL_SYSTEM
			|| value & YES_TO_ALL;
}

BOOL CErrFile::OverHidden(UINT value)
{
	return	   value & YES_TO_ALL_HIDDEN
			|| value & YES_TO_ALL;
}

BOOL CErrFile::SkipReadOnly(UINT value)
{
	return	   value & SKIP_ALL_READ_ONLY
			|| value & SKIP_ALL;
}

BOOL CErrFile::Skipsystem(UINT value)
{
	return	   value & SKIP_ALL_SYSTEM
			|| value & SKIP_ALL;
}

BOOL CErrFile::SkipHidden(UINT value)
{
	return	   value & SKIP_ALL_HIDDEN
			|| value & SKIP_ALL;
}

BOOL CErrFile::SkipShared(UINT value)
{
	return	   value & SKIP_ALL_SHARED
			|| value & SKIP_ALL;
}
