// ItemsStorage.cpp: implementation of the CItemsStorage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "coolexplorer.h"
#include "ItemsStorage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemsStorage::CItemsStorage(HWND hParent, CFilesListBase *pList) : _hParent(hParent), _pList(pList)
{

}

CItemsStorage::~CItemsStorage()
{
	//releaseItems();
}
