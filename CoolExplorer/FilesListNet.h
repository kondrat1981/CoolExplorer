// FilesListNet.h: interface for the CFilesListNet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESLISTNET_H__9B080C4C_AD56_4D43_B73B_764373A36CE4__INCLUDED_)
#define AFX_FILESLISTNET_H__9B080C4C_AD56_4D43_B73B_764373A36CE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FilesListBase.h"

class CFilesListNet : public CFilesListBase  
{
public:
	CFilesListNet(CFilesListManager *pManager);
	virtual ~CFilesListNet();

	virtual BOOL EnumFiles(LPCTSTR sPath, int nJob);
	virtual BOOL Select(const ListData *pld, BOOL &bBrowseItem, int nJob);
	virtual BOOL IsBrowseable(LPCTSTR sPath){return IsBrowseableByNet(sPath);}

	virtual CItemsStorage *SaveItems(LISTDataPointers &listItems);

private:
	BOOL EnumerateServers(int nJob);
	BOOL EnumerateNetResource(int nJob, LPCTSTR sServer);

};

#endif // !defined(AFX_FILESLISTNET_H__9B080C4C_AD56_4D43_B73B_764373A36CE4__INCLUDED_)
