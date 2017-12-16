// FilesListNormal.h: interface for the CFilesListNormal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESLISTNORMAL_H__E70DF701_9383_4BB0_9C5D_7964D7FDA9E6__INCLUDED_)
#define AFX_FILESLISTNORMAL_H__E70DF701_9383_4BB0_9C5D_7964D7FDA9E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FilesListBase.h"

class CItemsStorageShell;
class CFilesListNormal : public CFilesListBase  
{
public:
	CFilesListNormal(CFilesListManager *pManager);
	virtual ~CFilesListNormal();

	virtual BOOL EnumFiles(LPCTSTR sPath, int nJob);
	virtual BOOL Select(const ListData *pld, BOOL &bBrowseItem, int nJob);
	virtual BOOL IsBrowseable(LPCTSTR sPath){return IsBrowseableByNormal(sPath);};
	virtual BOOL AreFilesOperationsAvailable() const {return TRUE;}
	virtual BOOL CanGoToMain();

//	virtual BOOL DisplayExplorerMenu(CItemsStorage *pData, CPoint pt);
	virtual CItemsStorage *SaveItems(LISTDataPointers &listItems){return SaveItemsUsingFilesNames(listItems);}

//	virtual BOOL InvokeExplorerMenuCommand(HWND hwnd, const LISTDataPointers &listPointers, LPSTR sCommnad);

	virtual BOOL CreateDirectory(LPCTSTR sDir);
	virtual int ReadIcon(ListData *pld, int nDataVer){return ReadIconUsingFileName(pld, nDataVer);}
private:

};

#endif // !defined(AFX_FILESLISTNORMAL_H__E70DF701_9383_4BB0_9C5D_7964D7FDA9E6__INCLUDED_)
