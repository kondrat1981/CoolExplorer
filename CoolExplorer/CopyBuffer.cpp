// CopyBuffer.cpp: implementation of the CopyBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoolExplorer.h"
#include "CopyBuffer.h"
#include "ProgressDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CopyBuffer::CopyBuffer()
{
	m_pProgressDlg = NULL;
	m_pData = NULL;
	m_nCopyFlags = 0;
}

CopyBuffer::~CopyBuffer()
{
	delete [] m_pData;
//	delete this;
}

BOOL CopyBuffer::GetFiles(INT_PTR iIndex)
{
	
	CFileFind ff;
	CFileStatus fs;

	CString sFindPath = m_sPathFrom + m_aDirs.GetAt(iIndex).sDir + _T("\\*.*");
	BOOL bContinue = ff.FindFile(sFindPath);
    while (bContinue)
	{
		bContinue = ff.FindNextFile();
		CString sFileNam = m_aDirs.GetAt(iIndex).sDir;
		if(sFileNam.IsEmpty())
			sFileNam = m_sPathFrom + ff.GetFileName();
		else
			sFileNam = m_sPathFrom + sFileNam + _T("\\") + ff.GetFileName();
		CFile::GetStatus(sFileNam, fs);
        if (ff.IsDirectory() && !ff.IsDots())
		{
			BufDir bfd;
			bfd.sDir = m_aDirs.GetAt(iIndex).sDir;
			if(!bfd.sDir.IsEmpty())
				bfd.sDir += _T("\\");
			bfd.sDir += ff.GetFileName();
			bfd.fsDir = fs;
			INT_PTR pos = m_aDirs.Add(bfd);
			GetFiles(pos);
		}
		if (!ff.IsDirectory() && !ff.IsDots())
		{
			if(Compare(m_sMask, ff.GetFileName()))
			{			
				m_aDirs[iIndex].aFiles.Add(NameStatus(ff.GetFileName(), fs));
				m_nSourceSize += fs.m_size;
			}
		}
	}
	return TRUE;
}

BOOL CopyBuffer::Init(LPCTSTR sPath, CStringList & lFilesList)
{
	BOOL bRet = TRUE;
	m_ptFirstCopied = CPoint(0,-1);
	m_ptLastCopied = CPoint(0,-1);
	m_bLastCopied = TRUE;
	m_wLastFilePos = 0;
	m_sPathFrom = sPath;
	BufDir bf;
	bf.sDir = _T("");
	INT_PTR nFromDir = m_aDirs.Add(bf);

	while(!lFilesList.IsEmpty() && bRet)
	{
		CFileStatus FileStat;
		CString sName;
		sName = lFilesList.RemoveHead();
		CFile::GetStatus(sPath + sName, FileStat);
		if(FileStat.m_attribute  & CFile::volume)
		{			
			ASSERT(0);
			bRet = FALSE;
			break;
		}
		if(FileStat.m_attribute  & CFile::directory)
		{
			INT_PTR pos = m_aDirs.Add(BufDir(sName, FileStat));
			bRet = GetFiles(pos);
		}
		else
		{
			if(Compare(m_sMask, sName))
			{
				m_aDirs[nFromDir].aFiles.Add(NameStatus(sName, FileStat));
				m_nSourceSize += FileStat.m_size;
			}
		}
	}
	return bRet;
}

BOOL CopyBuffer::CopyFiles(LPCTSTR sPathFrom, LPCTSTR sPathTo, CStringList & lFilesList, LPCTSTR sMask)
{
	CWaitCursor cw;

	m_sMask = sMask;

	if(m_sMask.IsEmpty())
		m_sMask = _T("*.*");

	m_aDirs.RemoveAll();

	m_nSourceSize = 0;

	m_sPathTo = sPathTo;

	Init(sPathFrom, lFilesList);

	MEMORYSTATUS MemStat;

	MemStat.dwLength = sizeof(MEMORYSTATUS);
	
	GlobalMemoryStatus(&MemStat);
	
	m_BufferSize = (UINT)MemStat.dwTotalPhys / 16;

	if(m_BufferSize * 8 > m_nSourceSize) m_BufferSize = (UINT)m_nSourceSize / 8 ;

	m_pProgressDlg->PostMessage(WM_PROGRESS, PROGRESS_RANGE, (LPARAM) (0.5 + double(m_nSourceSize)/m_BufferSize));

//	AfxBeginThread(RunProgress, this);

	Copy();

	return TRUE;
}

errors CopyBuffer::ReadBuffer()
{
	BOOL bBufNotFull = TRUE;

	m_ptFirstCopied = m_ptLastCopied;

	m_wFirstFilePos = m_wLastFilePos;

	UINT iFree = m_BufferSize;

	while(bBufNotFull)
	{
		if(m_pProgressDlg->GetSafeHwnd() == NULL)
			return interrupt;
		if(m_bLastCopied)
		{
			errors cResult = OpenNextFile(m_FileFrom, m_ptLastCopied);
			switch (cResult)
			{
				case last:
					return last;
				case fail:
					AfxMessageBox(_T("Read: Error Open file"));
					return fail;
				case interrupt:
					return interrupt;
				case ok:
					break;
				default:
					ASSERT(0);
			}
			m_wLastFilePos = 0;
		}
		ULONGLONG iFileSize = m_FileFrom.GetLength() - m_wLastFilePos;
		if(iFileSize >= iFree)
		{
			m_FileFrom.Seek(m_wLastFilePos, CFile::begin);
			m_FileFrom.Read(m_pData + m_BufferSize - iFree, iFree); 
			bBufNotFull = FALSE;
			m_bLastCopied = FALSE;
			m_wLastFilePos += iFree;
		}
		else
		{
			m_FileFrom.Seek(m_wLastFilePos, CFile::begin);
			m_FileFrom.Read(m_pData + m_BufferSize - iFree, (UINT)iFileSize);
			iFree -= (UINT)iFileSize;
			m_bLastCopied = TRUE;
		}		
	}
	return ok;
}

errors CopyBuffer::WriteBuffer()
{
	BOOL bBufNotEmpty = TRUE;
	CPoint pt = m_ptFirstCopied;
	ULONGLONG wFirstFilePos = m_wFirstFilePos;
	UINT wWriteCount = 0;
	ULONGLONG iFileSize = 0;
	while(bBufNotEmpty)
	{
		if(wFirstFilePos == 0)
		{
			if(m_pProgressDlg->GetSafeHwnd() == NULL)
				return interrupt;
			TCHAR cResult = OpenNextFileWrite(m_FileTo, pt);
			switch (cResult)
			{
				case last:	// last
					return last;
				case fail:
					AfxMessageBox(_T("Write: Error Open file"));
					return fail;
				case interrupt:
					return interrupt;
				case ok:
					break;
				case skip:

					break;
				default:
					ASSERT(0);
			}
			m_FileTo.SeekToBegin();
			iFileSize = GetFile(pt).FileStat.m_size;
		}
		else
		{
			m_FileTo.SeekToEnd();
			iFileSize = GetFile(pt).FileStat.m_size - wFirstFilePos;
			wFirstFilePos = 0;			
		}
				
		UINT wFree = m_BufferSize - wWriteCount;
		if(iFileSize >= wFree)
		{
			m_FileTo.Write(m_pData + wWriteCount, wFree);
			bBufNotEmpty = FALSE;
		}
		else
		{
			m_FileTo.Write(m_pData + wWriteCount, (UINT)iFileSize);
			wWriteCount += (UINT)iFileSize;
		}
	}
	return ok;
}

errors CopyBuffer::OpenNextFile(CErrFile &file, CPoint &pt, UINT nOpenFlags /* = CFile::modeRead*/, BOOL bRestoreStatus /*= FALSE*/)
{
	if(bRestoreStatus && file.m_hFile != CFile::hFileNull)
	{			
		file.Close();
		CFile::SetStatus(GetFromFileName(pt), GetFile(pt).FileStat);
	}

	BOOL bContinue = TRUE;
	CPoint old_point = pt;
	while(bContinue)
	{
		if(file.m_hFile != CFile::hFileNull)
			file.Close();
		CString sFileName;
		errors cResult;
		do{
			cResult = GetNext(pt);
		}while (cResult == directory);
		if(cResult == last)
		{
			TRACE(_T("Read: last file\n"));
			return last;
		}
		
		sFileName = GetFromFileName(pt);

		if(IsWindow(m_pProgressDlg->GetSafeHwnd()))
		{
			LPTSTR szName = new TCHAR[sFileName.GetLength()+1];
			_tcscpy(szName, sFileName);
			m_pProgressDlg->PostMessage(WM_PROGRESS, NAME_FROM, (LPARAM)szName);
		}
		ASSERT(sFileName.Find(_T("\\\\")) == -1 || sFileName.Find(_T("\\\\")) == 0);
		errors eRet = file.Open(sFileName, nOpenFlags, m_nCopyFlags);
		switch (eRet)
		{
			case ok:
				{
				bContinue = FALSE;
				NameStatus &ns = GetFile(pt);
				if((DWORD)file.GetLength() != ns.FileStat.m_size)
				{
					AfxMessageBox(_T("File changed"));
					CFileStatus fs;
					if(!file.GetStatus(fs))
					{
						ASSERT(0);
						bContinue = TRUE;
						RemoveAt(pt);
						pt = old_point;
					}
					else
					{
						ns.FileStat = fs;
					}
				}
				break;
				}
			case fail:
				bContinue = FALSE;
				TRACE(_T("Read: Error opening file %s\n"), sFileName);
				return fail;
			case skip:
				RemoveAt(pt);
				pt = old_point;
				break;
			case interrupt:
				return interrupt;
			default:
				ASSERT(0);
		}
	}
	return ok;
}

errors CopyBuffer::OpenNextFileWrite(CErrFile &file, CPoint &pt)
{
	if(file.m_hFile != CFile::hFileNull)
	{			
		file.Close();
		CFile::SetStatus(GetToFileName(pt), GetFile(pt).FileStat);
	}
	CString sFileName;
	errors cResult = ok;

	cResult = GetNext(pt);
	while(cResult == directory)
	{
		if(FALSE == MyCreateDirectory(GetToDirName(pt), NULL))
		{
		}
			//TRACE(_T("Failed to create dir %s\n"), GetToDirName(pt));
		cResult = GetNext(pt);
	}

	if(cResult == last)
	{
		TRACE(_T("Write: last file\n"));
		return last;
	}

	sFileName = GetToFileName(pt);
	if(::IsWindow( m_pProgressDlg->m_hWnd ))
	{
		LPTSTR szName = new TCHAR[sFileName.GetLength()+1];
		_tcscpy(szName, sFileName);	
		m_pProgressDlg->PostMessage(WM_PROGRESS, NAME_TO, (LPARAM)szName);
	}

	ASSERT(sFileName.Find(_T("\\\\")) == -1 || sFileName.Find(_T("\\\\")) == 0);
	
	errors eRet = file.OpenFileForWrite(sFileName, m_nCopyFlags);
	
	switch(eRet)
	{
	case ok:
		break;
	case skip:
		return skip;
	case fail:
		TRACE(_T("Write: Error opening file %s\n"), sFileName);
		return fail;
	case interrupt:

		return interrupt;
	default:
		ASSERT(0);
	}
	return ok;
}

CString CopyBuffer::GetFileName(CPoint pt)
{
	CString sFile;
	
	sFile = m_aDirs.GetAt(pt.x).sDir;
	if(sFile.ReverseFind('\\') != sFile.GetLength()-1)
		sFile += _T("\\");
	sFile += GetFile(pt).sName;
	return sFile;
}

BOOL CopyBuffer::RemoveAt(CPoint pt)
{
	ASSERT(pt.x >= 0 && pt.x < m_aDirs.GetSize() && pt.y >= 0 && pt.y < m_aDirs.GetAt(pt.x).aFiles.GetSize());
	m_aDirs[pt.x].aFiles.RemoveAt(pt.y);
	if(m_aDirs.GetSize() == 0)
		m_aDirs.RemoveAt(pt.x);
	return TRUE;
}

NameStatus &CopyBuffer::GetFile(CPoint pt)
{
	ASSERT(pt.x >= 0 && pt.x < m_aDirs.GetSize() && pt.y >= 0 && pt.y < m_aDirs.GetAt(pt.x).aFiles.GetSize());
	return m_aDirs[pt.x].aFiles[pt.y];	
}

CString CopyBuffer::GetFromFileName(CPoint pt)
{
	CString sFile;
	sFile = m_sPathFrom + GetFileName(pt);
	return sFile;
}

CString CopyBuffer::GetToFileName(CPoint pt)
{
	CString sFile;

	sFile = m_sPathTo + GetFileName(pt);

	return sFile;
}

CString CopyBuffer::GetToDirName(CPoint pt)
{
	ASSERT(pt.x >= 0 && pt.x < m_aDirs.GetSize() && pt.y == -1);
	CString sDir;
	sDir = m_sPathTo;
	sDir = sDir.Left(sDir.GetLength() - 1);
	sDir += _T("\\") + m_aDirs.GetAt(pt.x).sDir;
	if(sDir.ReverseFind('\\') == sDir.GetLength()-1)
		sDir = sDir.Left(sDir.GetLength() - 1);
	return sDir;
}

errors CopyBuffer::GetNext(CPoint &pt)
{
//	ASSERT(pt != CPoint(4,0));
	pt.y++;
	if(pt.x == -1 || pt.y >= m_aDirs.GetAt(pt.x).aFiles.GetSize())
	{
		pt.x++;
		pt.y = -1;
		if(pt.x >= m_aDirs.GetSize())
			return last; // last file
		return directory;
	}
	return file;
}

INT_PTR CopyBuffer::RunProgress(LPVOID lParam)
{
	return ((CopyBuffer *)lParam)->m_pProgressDlg->DoModal();
}

UINT CopyBuffer::CopyThread(LPVOID lParam)
{
	((CopyBuffer *)lParam)->Copy();
	return 0;
}

void CopyBuffer::Copy()
{
	m_pData = new BYTE[m_BufferSize];	

	errors bContinue = ok;
	
	int nProgress = 0;

	while(bContinue == ok && IsWindow(m_pProgressDlg->m_hWnd))
	{
		bContinue = ReadBuffer();
		if(fail != bContinue &&
			interrupt != bContinue)
		{
			if(IsWindow(m_pProgressDlg->m_hWnd))
			{
				m_pProgressDlg->PostMessage(WM_PROGRESS, PROGRESS_FROM, ++nProgress);
				bContinue = WriteBuffer();
			}
		}
		if(IsWindow(m_pProgressDlg->m_hWnd))
			m_pProgressDlg->PostMessage(WM_PROGRESS, PROGRESS_TO, nProgress);
	}

	if(IsWindow(m_pProgressDlg->m_hWnd))
		m_pProgressDlg->PostMessage(WM_CLOSE);

	if(m_FileFrom.m_hFile != CFile::hFileNull)
	{
		ASSERT(0 || bContinue==interrupt);
		m_FileFrom.Close();
	}
	
	if(m_FileTo.m_hFile != CFile::hFileNull)
	{
		ASSERT(0 || bContinue==interrupt);
		m_FileTo.Close();
	}
	delete [] m_pData;
	m_pData = NULL;
}

void CopyBuffer::DeleteFiles(LPCTSTR sPath, CStringList &lNames)
{	
/*	CFileFind ff;
	CFileStatus fs;

	CString sFindPath = m_sPathFrom + m_aDirs.GetAt(iIndex).sDir + "\\*.*";
	BOOL bContinue = ff.FindFile(sFindPath);
    while (bContinue)
	{
		bContinue = ff.FindNextFile();
		CString sFileNam = m_aDirs.GetAt(iIndex).sDir;
		if(sFileNam.IsEmpty())
			sFileNam = m_sPathFrom + ff.GetFileName();
		else
			sFileNam = m_sPathFrom + sFileNam + "\\" + ff.GetFileName();
		CFile::GetStatus(sFileNam, fs);
        if (ff.IsDirectory() && !ff.IsDots())
		{
			BufDir bfd;
			bfd.sDir = m_aDirs.GetAt(iIndex).sDir;
			if(!bfd.sDir.IsEmpty())
				bfd.sDir += "\\";
			bfd.sDir += ff.GetFileName();
			bfd.fsDir = fs;
			int pos = m_aDirs.Add(bfd);
			GetFiles(pos);
		}
		if (!ff.IsDirectory() && !ff.IsDots())
		{
			m_aDirs[iIndex].aFiles.Add(NameStatus(ff.GetFileName(), fs));
			m_nSourceSize += fs.m_size;
		}
	}
	return TRUE;
	*/
}

void DeleteFiles(LPCTSTR sPathName)
{	
/*	CFileFind ff;
	CFileStatus fs;

	CString sFindPath = m_sPathFrom + m_aDirs.GetAt(iIndex).sDir + "\\*.*";
	BOOL bContinue = ff.FindFile(sFindPath);
    while (bContinue)
	{
		bContinue = ff.FindNextFile();
		CString sFileNam = m_aDirs.GetAt(iIndex).sDir;
		if(sFileNam.IsEmpty())
			sFileNam = m_sPathFrom + ff.GetFileName();
		else
			sFileNam = m_sPathFrom + sFileNam + "\\" + ff.GetFileName();
		CFile::GetStatus(sFileNam, fs);
        if (ff.IsDirectory() && !ff.IsDots())
		{
			BufDir bfd;
			bfd.sDir = m_aDirs.GetAt(iIndex).sDir;
			if(!bfd.sDir.IsEmpty())
				bfd.sDir += "\\";
			bfd.sDir += ff.GetFileName();
			bfd.fsDir = fs;
			int pos = m_aDirs.Add(bfd);
			GetFiles(pos);
		}
		if (!ff.IsDirectory() && !ff.IsDots())
		{
			m_aDirs[iIndex].aFiles.Add(NameStatus(ff.GetFileName(), fs));
			m_nSourceSize += fs.m_size;
		}
	}
	return TRUE;
	*/
}
