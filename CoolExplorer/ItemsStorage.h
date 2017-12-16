// ItemsStorage.h: interface for the CItemsStorage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSSTORAGE_H__F8FEC873_2AA1_4125_A11B_4C16B948470F__INCLUDED_)
#define AFX_ITEMSSTORAGE_H__F8FEC873_2AA1_4125_A11B_4C16B948470F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFilesListBase;
class CItemsStorage  
{
public:
	CItemsStorage(HWND hParent, CFilesListBase *pList);
	virtual ~CItemsStorage();
	//virtual BOOL saveItems(LISTDataPointers &listItems) = 0;
	virtual DROPEFFECT DoDragDrop() = 0;
	virtual BOOL CopyClipboard() = 0;
	virtual BOOL CutClipboard() = 0;
	virtual BOOL PasteClipboard() = 0;
	virtual BOOL DisplayExplorerMenu(CPoint pt)=0;
	virtual LPDROPTARGET createDropTarget(){return NULL;}
	
	
	HWND getParent() const {return _hParent;}
	CFilesListBase *getList(){return _pList;}
	const CString &getDir() const {return _sDir;}
	void setDir(LPCTSTR sDir){_sDir = sDir;}
	//const CString &getDropTarget() const {return _sDir;}
private:
	//virtual BOOL releaseItems() = 0;
	CFilesListBase *_pList;
	HWND _hParent;
	CString _sDir;
};

#endif // !defined(AFX_ITEMSSTORAGE_H__F8FEC873_2AA1_4125_A11B_4C16B948470F__INCLUDED_)
