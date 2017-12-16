// FilesListObserverInterface.h: interface for the CFilesListObserverInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESLISTOBSERVERINTERFACE_H__EB49DE87_7053_4A65_A598_231A3E7E3408__INCLUDED_)
#define AFX_FILESLISTOBSERVERINTERFACE_H__EB49DE87_7053_4A65_A598_231A3E7E3408__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFilesListObservable;
class CFilesListObserverInterface  
{
public:
	CFilesListObserverInterface();
	virtual ~CFilesListObserverInterface();
	virtual void Update(CFilesListObservable *, int nDataVersion, BOOL Result) = 0;
	virtual void StopUseObservable(CFilesListObservable *, LPCTSTR sNewPath){;}
//	virtual PathChanged(const CString &sNewPath);

};

#endif // !defined(AFX_FILESLISTOBSERVERINTERFACE_H__EB49DE87_7053_4A65_A598_231A3E7E3408__INCLUDED_)
