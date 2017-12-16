// ItemsStorageVirt.cpp: implementation of the CItemsStorageVirt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "coolexplorer.h"
#include "ItemsStorageVirt.h"
#include "ItemsStorageShell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemsStorageVirt::CItemsStorageVirt(HWND hParent, CFilesListBase *pList, CItemsStorageShell *pShellStorage) 
:CItemsStorage(hParent, pList), _pShellStorage(pShellStorage)
{

}

CItemsStorageVirt::~CItemsStorageVirt()
{
	delete _pShellStorage;
}

DROPEFFECT CItemsStorageVirt::DoDragDrop()
{
	if(_pShellStorage)
		return _pShellStorage->DoDragDrop();
	return DROPEFFECT_NONE;
}
BOOL CItemsStorageVirt::CopyClipboard()
{
	if(_pShellStorage)
		return _pShellStorage->CopyClipboard();
	return FALSE;
}

BOOL CItemsStorageVirt::CutClipboard()
{
	if(_pShellStorage)
		return _pShellStorage->CutClipboard();
	return FALSE;
}

BOOL CItemsStorageVirt::PasteClipboard()
{
	if(_pShellStorage)
		return _pShellStorage->PasteClipboard();
	return FALSE;
}

BOOL CItemsStorageVirt::DisplayExplorerMenu(CPoint pt)
{
	if(_pShellStorage && _tcsncmp(getDir(), VIRT_NAME, _tcslen(VIRT_NAME)))// not pure virtual
	{
		return _pShellStorage->DisplayExplorerMenu(pt);
	}
	else
	{
		CMenu menu;
		menu.CreatePopupMenu();
		
		int nCmd = 100;
		menu.AppendMenu(MF_STRING, ID_DELETE_FROMVIRTUAL, _T_LNGSTR(245));//"Remove from Virtual Panel\t+Del"
		menu.AppendMenu(MF_STRING, ID_DELETE, _T_LNGSTR(246));//"Delete file(s) from disk\tCtrl+Del"
		
		nCmd = ::TrackPopupMenu(menu.m_hMenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, getParent(), NULL);
		//menu.
		CString sSelected;
		if(nCmd>0)
		{
			::SendMessage(getParent(), WM_COMMAND, nCmd, 0);
		}
		
		menu.DestroyMenu();
		return TRUE;
	}	
}