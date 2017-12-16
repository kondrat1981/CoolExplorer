// DropSource.cpp: implementation of the CDropSource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "coolexplorer.h"
#include "DropSource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


HRESULT CDropSource::QueryInterface(REFIID riid, void **ppv)
{
  IUnknown *punk = NULL;
  if (riid == IID_IUnknown) {
    punk = static_cast<IUnknown*>(this);
  } else if (riid == IID_IDropSource) {
    punk = static_cast<IDropSource*>(this);
  }

  *ppv = punk;
  if (punk) {
    punk->AddRef();
    return S_OK;
  } else {
    return E_NOINTERFACE;
  }
}

ULONG CDropSource::AddRef()
{
  return ++m_cRef;
}

ULONG CDropSource::Release()
{
  ULONG cRef = --m_cRef;
  if (cRef == 0) delete this;
  return cRef;
}


HRESULT CDropSource::QueryContinueDrag(
          BOOL fEscapePressed, DWORD grfKeyState)
{
  if (fEscapePressed) return DRAGDROP_S_CANCEL;

  // [Update: missing paren repaired, 7 Dec]
  if (!(grfKeyState & (MK_LBUTTON | MK_RBUTTON)))
    return DRAGDROP_S_DROP;

  return S_OK;
}

HRESULT CDropSource::GiveFeedback(DWORD dwEffect)
{
  return DRAGDROP_S_USEDEFAULTCURSORS;
}
