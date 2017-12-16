// FilesListNormal.h: interface for the CFilesListNormal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESLISTNORMAL_H__E70DF701_9383_4BB0_9C5D_7964D7FDA9E6__INCLUDED_)
#define AFX_FILESLISTNORMAL_H__E70DF701_9383_4BB0_9C5D_7964D7FDA9E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FilesListBase.h"

class CFilesListArchive : public CFilesListBase  
{
public:
	CFilesListArchive(CFilesListManager *pManager);
	virtual ~CFilesListArchive();

	virtual BOOL EnumFiles(LPCTSTR sPath, int nJob);
	virtual BOOL Select(const ListData *pld, BOOL &bBrowseItem, int nJob);
	virtual BOOL IsBrowseable(LPCTSTR sPath){return IsBrowseableByNormal(sPath);};

	virtual BOOL CreateDirectory(LPCTSTR sDir);

private:

};

#endif // !defined(AFX_FILESLISTNORMAL_H__E70DF701_9383_4BB0_9C5D_7964D7FDA9E6__INCLUDED_)
