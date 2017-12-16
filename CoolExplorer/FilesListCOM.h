// FilesListCOM.h: interface for the CFilesListCOM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESLISTCOM_H__B309B3ED_EBE1_4EFC_B86A_AC26FC104069__INCLUDED_)
#define AFX_FILESLISTCOM_H__B309B3ED_EBE1_4EFC_B86A_AC26FC104069__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FilesListBase.h"

typedef CList<LPITEMIDLIST, LPITEMIDLIST> LISTItemIDList;
class CShell;
class CItemsStorageShell;
class CFilesListCOM : public CFilesListBase  
{
public:
	CFilesListCOM(CFilesListManager *pManager = NULL);
	virtual ~CFilesListCOM();

	virtual BOOL EnumFiles(LPCTSTR sPath, int nJob);
	virtual BOOL Select(const ListData *pld, BOOL &bBrowseItem, int nJob);
	virtual BOOL IsBrowseable(LPCTSTR sPath){return IsBrowseableByCOM(sPath);}
	virtual CString GetDir() const
	{
		return m_sPath;
	}
	virtual BOOL AreFilesOperationsAvailable() const {return m_bIsFileSystem || IsDesktop();}
	virtual BOOL CanGoToMain();
	virtual BOOL CreateDirectory(LPCTSTR sDir);
//	virtual BOOL DisplayExplorerMenu(CItemsStorage *pData, CPoint pt);

	virtual CItemsStorage *SaveItems(LISTDataPointers &listItems);
	
	virtual CString GetFileFullPath(const ListData *pdl) const;

protected:
	virtual int ReadIcon(ListData *pld, int nDataVer);

private:
	BOOL IsDesktop() const {return NULL == m_pidlList && NULL != m_pShellFolder;}
	BOOL GetSubFolder(IShellFolder *&pNewFolder, LPITEMIDLIST &pNewIDList, const LPITEMIDLIST pListSub, int nJob);
	BOOL GetParentFolder(IShellFolder *&pFolder, LPITEMIDLIST &listID, int nJob, LPITEMIDLIST *pChildId = NULL);
	BOOL SheelEnumFiles(IShellFolder *pFolder, LISTData *plist, int nJob);
	//BOOL ShellEnumFiles(ListData * pListData, BOOL bBindDesktop);
	//BOOL ShellEnumFiles(LPCTSTR sPath);
	BOOL GetNetworkFileInfo(IShellFolder *pFolder, ListData *fi, LPITEMIDLIST pidlList);
	BOOL GetSystemFileInfo(IShellFolder *pFolder, ListData * fi, LPITEMIDLIST pidlList);



	BOOL GetFileInfo(IShellFolder *pFolder, ListData * pListData);
	BOOL ExecuteItem(LPCTSTR sDir, LPITEMIDLIST idDir, LPITEMIDLIST idSelItem);



	LPITEMIDLIST m_pidlList;
	IShellFolder * m_pShellFolder;
	CShell *m_pShell;
	BOOL m_bSilence;
	BOOL m_bIsFileSystem;

	BOOL m_bIsNetwork;
	map<CString, CString> m_mapDisp2Parse;// 

};

#endif // !defined(AFX_FILESLISTCOM_H__B309B3ED_EBE1_4EFC_B86A_AC26FC104069__INCLUDED_)
