// FilesListObservable.h: interface for the CFilesListObservable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESLISTOBSERVABLE_H__FFB36537_2F1B_4E3E_8215_48080EB4A94C__INCLUDED_)
#define AFX_FILESLISTOBSERVABLE_H__FFB36537_2F1B_4E3E_8215_48080EB4A94C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )
class ListData;
class CFilesListObserverInterface;
class CFilesListObservable
{
public:
	CFilesListObservable();
	virtual ~CFilesListObservable();
	virtual CString GetDir()const = 0;
	virtual BOOL SetDir(LPCTSTR sPath) = 0;
	virtual BOOL Select(const ListData *pld, int nDataVer) = 0;
	virtual BOOL SetDirForDrive(LPCTSTR sDrive) = 0;
	//call it from observer
	void RegisterObserver(CFilesListObserverInterface *pObserver);
	void UnRegisterObserver(CFilesListObserverInterface *pObserver);	
	virtual BOOL IsCurJobStillActive() = 0;
	void InitObservers();
	void TurnOff();
protected:
	void NotifyObservers(int nDataVer, BOOL bResult);
	void NotifyStopUse(LPCTSTR sNewPath);
private:
	std::map<CFilesListObserverInterface *, CFilesListObserverInterface *> m_mapObservers;
	CCriticalSection m_Lock;
};

#endif // !defined(AFX_FILESLISTOBSERVABLE_H__FFB36537_2F1B_4E3E_8215_48080EB4A94C__INCLUDED_)
