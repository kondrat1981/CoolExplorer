// DropTarget.h: interface for the CDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROPTARGET_H__015337CE_2157_11D5_A0A4_E1568ACA9839__INCLUDED_)
#define AFX_DROPTARGET_H__015337CE_2157_11D5_A0A4_E1568ACA9839__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDropTarget : public IDropTarget
{
public:
	CDropTarget() { m_dwRef=0; m_pView=NULL;m_pIDTFolder=NULL; };
	~CDropTarget() { ASSERT(m_dwRef==0); }; // else we're being "destroyed" too early!
	
	void SetITarget(LPDROPTARGET pIDTFolder) { m_pIDTFolder = pIDTFolder;}
	LPDROPTARGET GetITarget() { return m_pIDTFolder;}
	void SetPath(const CString &sPath){m_sPath = sPath;}
	void SetView(CView *pView){m_pView = pView;}
	// IUnknown methods
	STDMETHOD(QueryInterface)(REFIID iid, void** ppvObject) {
		*ppvObject = NULL;
		// we only support IUnknown and IDropTarget, which is the one and same thing
		if( IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IDropTarget) ) {
			*ppvObject = this; // direct inheritance
			AddRef(); // implicitly assumed for each successful query
			return S_OK;
		}
		return E_NOINTERFACE;
		// nobody is ever likely to query us for anything else
	}
	STDMETHOD_(ULONG, AddRef)() { return ++m_dwRef;	}
	STDMETHOD_(ULONG, Release)() { 
		return --m_dwRef; 
		// note that this object is NOT meant for dynamic allocation...
		// that's why we don't delete ourselves when m_dwRef reaches zero
	}

	// drop target methods
	STDMETHOD(DragEnter)(LPDATAOBJECT pDataObject, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	STDMETHOD(DragOver)(DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);
	STDMETHOD(DragLeave)();
	STDMETHOD(Drop)(LPDATAOBJECT pDataObject, DWORD grfKeyState, POINTL pt, LPDWORD pdwEffect);


public:
	CView* m_pView; // the dialog window we communicate with
private:
	DWORD m_dwRef; // "COM" object reference counter
	CString m_sPath;
	LPDROPTARGET m_pIDTFolder;

	LPDATAOBJECT m_pDataObj_;
	DWORD m_xrefDataObj_; // reference counter for the dragged data object
	DWORD m_prevFlags;
#ifdef _DEBUG
	
	// We'll need that to make sure there are no outstanding references on it,
	// ensuring that the data object will be released ok and we won't cause any leaks
#endif
};


#endif // !defined(AFX_DROPTARGET_H__015337CE_2157_11D5_A0A4_E1568ACA9839__INCLUDED_)
