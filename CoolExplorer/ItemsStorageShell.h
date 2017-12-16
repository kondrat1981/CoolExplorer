// ItemsStorageShell.h: interface for the CItemsStorageShell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSSTORAGESHELL_H__02294928_2A48_498F_B286_FF0A90DF962A__INCLUDED_)
#define AFX_ITEMSSTORAGESHELL_H__02294928_2A48_498F_B286_FF0A90DF962A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemsStorage.h"

class CItemsStorageShell : public CItemsStorage  
{
public:
	CItemsStorageShell(HWND hParent, CFilesListBase *pList);
	virtual ~CItemsStorageShell();

	virtual BOOL saveItems(LISTDataPointers &listItems);	
	virtual DROPEFFECT DoDragDrop();
	
	virtual LPDROPTARGET createDropTarget();
	virtual BOOL CopyClipboard();
	virtual BOOL CutClipboard();
	virtual BOOL PasteClipboard();
	virtual BOOL DisplayExplorerMenu(CPoint pt);

	ITEMIDARRAY &getPidl(){return _aPidl;}
	IShellFolder *getFolder(){return _pFolder;}
	void setFolder(IShellFolder *pNew){ASSERT(NULL == _pFolder); _pFolder=pNew;}
//	void setPidl(ITEMIDARRAY &aNew){_aPidl=aNew;}
private:
	//virtual BOOL releaseItems();
	ITEMIDARRAY _aPidl;
	IShellFolder *_pFolder;
	LPDROPTARGET _DropTarget;
};

#endif // !defined(AFX_ITEMSSTORAGESHELL_H__02294928_2A48_498F_B286_FF0A90DF962A__INCLUDED_)
