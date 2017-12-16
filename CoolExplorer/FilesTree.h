// FilesTree.h: interface for the CFilesTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESTREE_H__41C2D00A_DA88_4BBE_BF3D_296FB22A7A7F__INCLUDED_)
#define AFX_FILESTREE_H__41C2D00A_DA88_4BBE_BF3D_296FB22A7A7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDirTree;
typedef CList<CDirTree *, CDirTree*> CDirTreeList;






class CDirEnumerator;
//class CFtpCopyDlg;

class CDirTree  
{
friend CDirEnumerator;
//friend CFtpCopyDlg;

public:
	static UINT ReadFtpThread(LPVOID pData);
	static UINT WriteFtpThread(LPVOID pData);

	BOOL MyCopyFtp(FtpProgressInfo *pInfo);
	BOOL GetFiles(CStringList &listFiles, CStringList &listDirs);
	void CopyFtp(CFtpConnection * pFtpCon, LPCTSTR sPathFrom, LPCTSTR strPathTo, FtpProgressInfo * pInfo);
	void Copy(CFtpConnection *pFtpCon, LPCTSTR sPathFrom, LPCTSTR sPathTo, FtpProgressInfo *pInfo);
	void Copy(LPCTSTR sPathFrom, LPCTSTR sPathTo, CopyInfo *pInfo);
	BOOL SetStatus(ChangeStatusInfo *pInfo, LPCTSTR sPath = _T(""));
	void Delete(LPCTSTR sPath, CopyInfo *pDelInfo);
	INT_PTR Count();
	ULONGLONG Init(LPCTSTR sPath, LPCTSTR sMask, const CStringList &lstrFiles, CopyInfo *pInfo);
	
	ULONGLONG FtpInit(CFtpConnection *pFtpConection, LPCTSTR sPath, LPCTSTR sMask, const CStringList *lstrFiles, CopyInfo *pInfo);
	CDirTree();
	virtual ~CDirTree();
	void Clean();
protected:
	ULONGLONG InitRecurse(LPCTSTR sPath, LPCTSTR sDir, LPCTSTR sMask, CopyInfo *pInfo);
	BOOL GetFiles(LPCTSTR sName, CStringList &listFiles, CStringList &listDirs);
	CDirFile m_recInfo;
	CDirTreeList m_lDir;
	CDirFilesList m_lFiles;
	BOOL m_bOtherFilesInDir;
};

class CDirEnumerator
{
public:
	void Enum(PFILEFUN pFileFun, PDIRFUN pDirFun, LPCTSTR sDesc);
	CDirEnumerator(LPCTSTR sPath, LPCTSTR sMask, const CStringList *lstrFiles = NULL, CFtpConnection *pFtpConection = NULL);
	virtual ~CDirEnumerator();
	void Enum(CDirTree *pDir = NULL, LPCTSTR sDesc = NULL);
protected:
	CDirTree m_DirTree;
	ULONGLONG m_nAllSize;
	PFILEFUN m_pFileFun;
	PDIRFUN m_pDirFun;
	CFtpConnection *m_pFtpConection;
};

#endif // !defined(AFX_FILESTREE_H__41C2D00A_DA88_4BBE_BF3D_296FB22A7A7F__INCLUDED_)
