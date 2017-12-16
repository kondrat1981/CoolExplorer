// ItemsStorageShell.cpp: implementation of the CItemsStorageShell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "coolexplorer.h"
#include "ItemsStorageShell.h"
#include "Shell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemsStorageShell::CItemsStorageShell(HWND hParent, CFilesListBase *pList) : CItemsStorage(hParent, pList), _pFolder(NULL), _DropTarget(NULL)
{
}

CItemsStorageShell::~CItemsStorageShell()
{
	if(_DropTarget)
	{
		_DropTarget->Release();
		_DropTarget = NULL;
	}
	CShell shell;
	for(int i=0; i<getPidl().GetSize(); i++)
	{
		LPITEMIDLIST p = (LPITEMIDLIST)getPidl()[i];
		shell.Free(p);
	}
	getPidl().RemoveAll();
	if(_pFolder)
		getFolder()->Release();
	_pFolder = NULL;
	setDir(_T(""));

}

BOOL CItemsStorageShell::saveItems(LISTDataPointers &listItems)
{
	BOOL bRet = TRUE;
	return bRet;
}

LPDROPTARGET CItemsStorageShell::createDropTarget()
{
	_DropTarget = CShell::GetDropTarget(this);
	return _DropTarget;
}


DROPEFFECT CItemsStorageShell::DoDragDrop()
{
	DROPEFFECT ef = DROPEFFECT_NONE;

	ef = CShell::DoDragDrop(getParent(), getFolder(), getPidl().GetData(), (UINT)getPidl().GetSize());

	return ef;
}

BOOL CItemsStorageShell::CopyClipboard()
{
	return CShell::InvokeExplorerMenuCommand(getParent(), getFolder(), getPidl().GetData(), (UINT)getPidl().GetSize(), "copy");
}

BOOL CItemsStorageShell::CutClipboard()
{
	return CShell::InvokeExplorerMenuCommand(getParent(), getFolder(), getPidl().GetData(), (UINT)getPidl().GetSize(), "cut");
}

BOOL CItemsStorageShell::PasteClipboard()
{
	return CShell::InvokeExplorerMenuCommand(getParent(), getFolder(), getPidl().GetData(), getPidl().GetSize(), "paste");
}

BOOL CItemsStorageShell::DisplayExplorerMenu(CPoint pt)
{
	return  CShell::DisplayExplorerMenu(this, pt);
}