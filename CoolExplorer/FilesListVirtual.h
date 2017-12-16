// FilesListVirtual.h: interface for the CFilesListVirtual class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESLISTVIRTUAL_H__20DA6423_10AA_4FEC_8488_58C85D85F478__INCLUDED_)
#define AFX_FILESLISTVIRTUAL_H__20DA6423_10AA_4FEC_8488_58C85D85F478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FilesListBase.h"

class CFilesListNormal;
class CFilesListVirtual : public CFilesListBase  
{
public:
	CFilesListVirtual(CFilesListManager *pManager);
	virtual ~CFilesListVirtual();

	BOOL InitVirtualList(ListDataCreator * pListDataCreator);
	BOOL AddToVirtualList(LISTData &lst);
	BOOL RemoveFromVirtualList(const CStringList &lst);
	BOOL IsPureVirtualList(){return !m_bUsingNormal;}
	BOOL SaveToFile(LPCTSTR sFile);
	BOOL LoadFromFile(LPCTSTR sFile);

	virtual BOOL EnumFiles(LPCTSTR sPath, int nJob);
	virtual BOOL Select(const ListData *pld, BOOL &bBrowseItem, int nJob);
	virtual BOOL CreateDirectory(LPCTSTR sDir);
	virtual BOOL IsBrowseable(LPCTSTR sPath){return IsBrowseableByVirtual(sPath);}
	virtual BOOL AreFilesOperationsAvailable() const {return TRUE;}
//	virtual BOOL DisplayExplorerMenu(CItemsStorage *pData, CPoint pt);
	virtual CItemsStorage *SaveItems(LISTDataPointers &listItems);
//	virtual BOOL InvokeExplorerMenuCommand(HWND hwnd, const LISTDataPointers &listPointers, LPSTR sCommnad);


	virtual CString GetFileFullPath(const ListData *pdl) const;
	virtual CString GetDir() const;

	virtual ListData * GetNext(POSITION &pos);
	virtual POSITION GetHeadPosition() const;
	virtual INT_PTR GetAllCount() const;
	virtual int ReadIcon(ListData *pld, int nDataVer){return ReadIconUsingFileName(pld, nDataVer);}
private:
	CFilesListNormal *m_pList;
	BOOL m_bUsingNormal;
	CString m_sStartNormal;
};

#endif // !defined(AFX_FILESLISTVIRTUAL_H__20DA6423_10AA_4FEC_8488_58C85D85F478__INCLUDED_)
