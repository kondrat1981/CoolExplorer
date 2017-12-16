// DropTarget.cpp: implementation of the CDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "dropDlg.h"
#include "DropTarget.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// debug helper, emiting the format names in a data object
void DumpDataObject(LPDATAOBJECT pDO)
{
	IEnumFORMATETC* pEnum;
	HRESULT hr = pDO->EnumFormatEtc(DATADIR_GET, &pEnum);
	if(pEnum) {
		TRACE(_T("Dumping data object 0x%p...\n"), pDO);

		FORMATETC fets;
		TCHAR buf[MAX_PATH];
		while(S_OK == pEnum->Next(1, &fets, NULL)) {
			// CF_HDROP=15 it won't return a name...
			GetClipboardFormatName(fets.cfFormat, buf, sizeof(buf));
			CString str;
			str.Format(_T("Format name: \"%s\" (%d)\n"), buf, fets.cfFormat);
			TRACE(_T("%s"), (LPCTSTR)str);
		}

		pEnum->Release();
	}
	
}

// first called when the mouse enters our target window space
STDMETHODIMP CDropTarget::DragEnter(LPDATAOBJECT pDataObject, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
#ifdef _DEBUG
	TRACE(_T("In DragEnter(), examining source data object\n"));
	DumpDataObject(pDataObject); // see what's in store for us

	// estimate the current reference counter on the dragged object
	// i'm keeping hold of the object, making sure that it won't be released too early
	// this should be released somewhere before the end
#endif
	m_xrefDataObj_ = pDataObject->AddRef() - 1/*don't count our extra reference*/; 
	m_pDataObj_ = pDataObject; // keep a pointer to it for DragLeave()


	BOOL ok = FALSE;
	ASSERT(m_pView);

	TRACE(_T("Before m_pView->OnDragEnter ITarget:0x%X\n"), m_pIDTFolder);
	m_pView->OnDragEnter(NULL,  grfKeyState, CPoint(pt.x, pt.y));
	TRACE(_T("After m_pView->OnDragEnter ITarget:%X\n"), m_pIDTFolder);

	HRESULT hr = S_OK;

	if(m_pIDTFolder)
	{
		hr = m_pIDTFolder->DragEnter(pDataObject, grfKeyState, pt, pdwEffect);
		if(SUCCEEDED(hr))
			ok = TRUE;
		TRACE(_T("After m_pIDTFolder->DragEnter ok:%d\n"), ok);
	}

	

	if(!ok) {
		*pdwEffect = DROPEFFECT_NONE; // we can't understand this thing
		TRACE(_T("DragEnter failed for this or the other reason\n"));
	}
	return S_OK;
}

STDMETHODIMP CDropTarget::DragOver(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	m_prevFlags = grfKeyState;
	ASSERT(m_pView);
	TRACE(_T("CDropTarget::DragOver begin pdwEffect:%d\n"), *pdwEffect);

	HRESULT hr = S_OK;
	if(m_pView)
	{
		LPDROPTARGET pIDTFolder = m_pIDTFolder;
		TRACE(_T("CDropTarget::DragOver before m_pView->OnDragOver pI:0x%X\n"), m_pIDTFolder);
		DROPEFFECT ef = m_pView->OnDragOver(NULL, grfKeyState, CPoint(pt.x, pt.y));
		TRACE(_T("CDropTarget::DragOver after m_pView->OnDragOver pI:0x%X\n"), m_pIDTFolder);
		if(DROPEFFECT_NONE == ef && m_pIDTFolder)//target changed
			hr = m_pIDTFolder->DragEnter(m_pDataObj_, grfKeyState, pt, pdwEffect);

	}
	else
	{
		*pdwEffect = DROPEFFECT_NONE;
	}

	ASSERT(hr==S_OK);
	if(m_pIDTFolder && hr == S_OK)
	{
		hr = m_pIDTFolder->DragOver(grfKeyState, pt, pdwEffect);
	}
	else
	{
		*pdwEffect = DROPEFFECT_NONE;		
	}
	TRACE(_T("CDropTarget::DragOver end pdwEffect:%d hr:%d\n"), *pdwEffect,hr);

	return hr;
}

// Either DragLeave or Drop will be called for finishing the operation, but NOT both
// DragLeave is for when the mouse leaves the target, or the drag is somehow cancelled
STDMETHODIMP CDropTarget::DragLeave()
{
	TRACE(_T("In DragLeave()\n"));

	HRESULT hr = S_OK;
	if(m_pIDTFolder)
	{
		hr = m_pIDTFolder->DragLeave();
		m_pIDTFolder = NULL;
	}

	

	DWORD cnt = m_pDataObj_->Release();

	// the reference counter on the data object should now be equal to the original
//	DWORD cnt = m_pDataObj_->Release();
	// as it turns out, the dataObject is fully released by OLE when we get in here!!!
	ASSERT(m_xrefDataObj_ == cnt+1);	

	if(m_pView)
	{
		m_pView->OnDragLeave();
	}


	return hr;
}

// exit point when something is actually dropped
STDMETHODIMP CDropTarget::Drop(LPDATAOBJECT pDataObject, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect)
{
	TRACE(_T("In Drop(), effect=%ld\n"), *pdwEffect);

	ASSERT(m_pView);
	ASSERT(m_pIDTFolder);
	ASSERT(*pdwEffect != DROPEFFECT_NONE);

	HRESULT hr = S_OK;

	BOOL bCopy = TRUE;
	if(m_prevFlags& MK_LBUTTON &&*pdwEffect & DROPEFFECT_MOVE && AfxMessageBox(CString(_T_LNGSTR(53)) + _T("?"), MB_OKCANCEL | MB_ICONQUESTION) != IDOK)
		bCopy = FALSE;
	
	if(bCopy)
		hr = m_pIDTFolder->Drop(pDataObject, grfKeyState, pt, pdwEffect);

	m_pIDTFolder = NULL;

	if(m_pView)
		m_pView->OnDrop(NULL, hr, CPoint(pt.x, pt.y));	


#ifdef _DEBUG
	DumpDataObject(pDataObject);
#endif
	ASSERT(pDataObject == m_pDataObj_);
	DWORD cnt = m_pDataObj_->Release();
	// contrary to DragLeave() here OLE still holds onto the data object
	ASSERT(m_xrefDataObj_ == cnt); // perfect agreement


	return hr;
}
