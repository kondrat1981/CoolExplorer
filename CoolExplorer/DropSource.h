// DropSource.h: interface for the CDropSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROPSOURCE_H__CB1928AF_98FD_499D_A1A4_0C3F4473D84E__INCLUDED_)
#define AFX_DROPSOURCE_H__CB1928AF_98FD_499D_A1A4_0C3F4473D84E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDropSource : public IDropSource
{
public:
  // *** IUnknown ***
  STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();

  // *** IDropSource ***
  STDMETHODIMP QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState);
  STDMETHODIMP GiveFeedback(DWORD dwEffect);

  CDropSource() : m_cRef(1) { }
private:
  ULONG m_cRef;
};

#endif // !defined(AFX_DROPSOURCE_H__CB1928AF_98FD_499D_A1A4_0C3F4473D84E__INCLUDED_)
