// ItemsStorageVirt.h: interface for the CItemsStorageVirt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSSTORAGEVIRT_H__4446CA8F_1CB3_4B5B_A1C3_8169662C4274__INCLUDED_)
#define AFX_ITEMSSTORAGEVIRT_H__4446CA8F_1CB3_4B5B_A1C3_8169662C4274__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemsStorage.h"

class CItemsStorageShell;
class CItemsStorageVirt : public CItemsStorage  
{
public:
	CItemsStorageVirt(HWND hParent, CFilesListBase *pList, CItemsStorageShell *pShellStorage);
	virtual ~CItemsStorageVirt();

	virtual DROPEFFECT DoDragDrop();
	virtual BOOL CopyClipboard();
	virtual BOOL CutClipboard();
	virtual BOOL PasteClipboard();
	virtual BOOL DisplayExplorerMenu(CPoint pt);

	CItemsStorageShell *getShellList(){return _pShellStorage;}
private:
	CItemsStorageShell *_pShellStorage;

};

#endif // !defined(AFX_ITEMSSTORAGEVIRT_H__4446CA8F_1CB3_4B5B_A1C3_8169662C4274__INCLUDED_)
