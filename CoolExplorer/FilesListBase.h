// FilesListBase.h: interface for the CFilesListBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESLISTBASE_H__AEFA388D_5172_408C_B7CA_D2A820129C46__INCLUDED_)
#define AFX_FILESLISTBASE_H__AEFA388D_5172_408C_B7CA_D2A820129C46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ListData.h"
#include "ListDataStorage.h"
#include "ShellIcons.h"

enum mode
{
	NoneMode,
	Normal,
	Ftp,
	//FFFN,
	Net,
	COM,
	Virtual
};



typedef CListDataStorage<ListData, ListData &> LISTDataStorage;

class CItemsStorage;
class CFilesListManager;
class CFilesListBase : public CShellIcons
{
public:
	CFilesListBase(CFilesListManager *pManager);
	virtual ~CFilesListBase();

public:
	virtual BOOL EnumFiles(LPCTSTR sPath, int nJobNum) = 0;
	virtual BOOL Select(const ListData *pld, BOOL &bBrowseItem, int nJobNum) = 0;
	
	virtual BOOL CreateDirectory(LPCTSTR sDir);

	virtual BOOL SetDir(LPCTSTR sDir, int nJobNum){return EnumFiles(sDir, nJobNum);}
	
	virtual BOOL IsBrowseable(LPCTSTR sPath) = 0;
	virtual BOOL AreFilesOperationsAvailable() const {return FALSE;}//Copy, move, delete ....
	virtual BOOL CanGoToMain() {return FALSE;}

//	virtual BOOL DisplayExplorerMenu(CItemsStorage *pData, CPoint pt) {return FALSE;}
//	virtual BOOL InvokeExplorerMenuCommand(HWND hwnd, const LISTDataPointers &listPointers, LPSTR sCommnad) {return FALSE;}
//	virtual BOOL CopyClipboard(HWND hwnd, const LISTDataPointers &listPointers) {return InvokeExplorerMenuCommand(hwnd, listPointers, "copy");}
//	virtual BOOL CutClipboard(HWND hwnd, const LISTDataPointers &listPointers) {return InvokeExplorerMenuCommand(hwnd, listPointers, "cut");}
//	virtual BOOL PasteClipboard(HWND hwnd, const LISTDataPointers &listPointers) {return InvokeExplorerMenuCommand(hwnd, listPointers, "paste");}	
	
	virtual CItemsStorage *SaveItems(LISTDataPointers &listItems){return NULL;}

	//virtual LPDROPTARGET GetDropTarget(CItemsStorage *pData){return NULL;}

	
	static BOOL IsBrowseableByNormal(LPCTSTR sPath);
	static BOOL IsBrowseableByNet(LPCTSTR sPath);
	static BOOL IsBrowseableByFtp(LPCTSTR sPath);
	static BOOL IsBrowseableByCOM(LPCTSTR sPath);
	static BOOL IsBrowseableByVirtual(LPCTSTR sPath);
	static BOOL CanUseFindFirstFile(LPCTSTR sPath);

	void LockList(){m_lFiles.Lock();}
	void UnlockList(){m_lFiles.Unlock();}

	BOOL CountDirsSize(ListData *pd , int nJob);	

	ListData *SearchInCurrentDir(LPCTSTR sHelpCom);

	//BOOL IsDir(const ListData *pld) const;

	// returns file system path for file
	//if pdl == NULL return file system path for current dir eg for Desktop -> C:\Documents and Settings\<user>\Desktop
	virtual CString GetFileFullPath(const ListData *pdl) const;
	virtual CString GetFileFullPath(const CString &sFile) const;
	
	virtual ListData * GetNext(POSITION &pos);
	virtual POSITION GetHeadPosition() const;
	virtual INT_PTR GetAllCount() const;
	BOOL DoCommand(LPCTSTR sCommand, BOOL bFromSelect = FALSE);// const removed - can enum files

	virtual CString GetDir() const
	{
		return m_sPath.IsEmpty() || m_sPath.Right(1) == dir_separator ? m_sPath : m_sPath + dir_separator;
	}
	
	CString GetDrive() const
	{
		if(m_sPath.GetLength()>=3 && m_sPath[1]==':')
			return TCHAR(_totupper(m_sPath[0]));
		return _T("");
	}
	CString GetDrivePath() const
	{
		CString s;
		if(m_sPath.GetLength()>=3 && m_sPath[1]==':')
		{
			s = TCHAR(_totupper(m_sPath[0]));
			s += _T(":") ;
			s += dir_separator;
		}
		return s;
	}

	const TCHAR GetDirSeparator() const {return dir_separator;}

	mode GetMode() const {return m_mode;}
	
	void SetSilenceMode(BOOL bSilenceMode){m_bSilenceMode = bSilenceMode;}

	void FreeOld() {m_lFiles.FreeOld();}
	CLockInterface &GetLockInterface() {return m_lFiles;}
protected:
	virtual int ReadIcon(ListData *pld, int nDataVer);
	int ReadIconUsingFileName(ListData *pld, int nDataVer);
	CItemsStorage *SaveItemsUsingFilesNames(LISTDataPointers &listItems, BOOL bFullPaths = FALSE);
	

	CString GetParentName(CString *psParentParentName = NULL) const;
	CSyncObject &GetSynObject() const {return m_lFiles.GetSynObject();}
	virtual HWND GetParent() const;
	void AddTail(ListData &pd){m_lFiles.AddTail(pd);}
	void AddHead(ListData &pd){m_lFiles.AddHead(pd);}
	void SetNewList(LISTData *pNewList){m_lFiles.SetNewList(pNewList);}

	BOOL IsLink(ListData *pld);
	void FreeList();
	CFilesListManager *m_pManager;
	TCHAR dir_separator;
	CString m_sPath;	
	void AddRetDir(LPCTSTR sCurDir);
	BOOL m_bSilenceMode;
	mode m_mode;
private:
	LONGLONG CountDirSize(LPCTSTR sPath, int nJob) const;
	LISTDataStorage m_lFiles;
protected:

	BOOL CanAdd(const ListData *ld);
};

#endif // !defined(AFX_FILESLISTBASE_H__AEFA388D_5172_408C_B7CA_D2A820129C46__INCLUDED_)
