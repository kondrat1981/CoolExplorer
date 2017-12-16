// FilesListNormal.cpp: implementation of the CFilesListNormal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FilesListArchive.h"
#include "FilesListManager.h"
#include "unrar.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilesListArchive::CFilesListArchive(CFilesListManager *pManager) : CFilesListBase(pManager)
{
}

CFilesListArchive::~CFilesListArchive()
{

}

BOOL CFilesListArchive::EnumFiles(LPCTSTR sPath, int nJob)
{

	return TRUE;
}

BOOL CFilesListArchive::Select(const ListData *pld, BOOL &bBrowseItem, int nJob)
{
	return TRUE;
}

BOOL CFilesListArchive::CreateDirectory(LPCTSTR sDir)
{
	BOOL bRet = FALSE;
	
	return bRet;
}