// FilesListObservable.cpp: implementation of the CFilesListObservable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FilesListObservable.h"
#include "FilesListObserverInterface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilesListObservable::CFilesListObservable()
{

}

CFilesListObservable::~CFilesListObservable()
{

}

void CFilesListObservable::RegisterObserver(CFilesListObserverInterface *pObserver)
{
	m_Lock.Lock();
	m_mapObservers[pObserver] = pObserver;
//	pObserver->Update(this, 0, FALSE);
	m_Lock.Unlock();
}

void CFilesListObservable::UnRegisterObserver(CFilesListObserverInterface *pObserver)
{
	m_Lock.Lock();
	m_mapObservers.erase(pObserver);
	m_Lock.Unlock();
}

void CFilesListObservable::NotifyObservers(int nDataVer, BOOL Result)
{
	m_Lock.Lock();
	map<CFilesListObserverInterface *, CFilesListObserverInterface*>::iterator it = m_mapObservers.begin();
	while(it != m_mapObservers.end())
	{
		(*it).second->Update(this, nDataVer, Result);
		it++;
	}
	m_Lock.Unlock();
}

void CFilesListObservable::NotifyStopUse(LPCTSTR sNewPath)
{
	m_Lock.Lock();
	map<CFilesListObserverInterface *, CFilesListObserverInterface*>::iterator it = m_mapObservers.begin();
	while(it != m_mapObservers.end())
	{
		(*it).second->StopUseObservable(this, sNewPath);
		it++;
	}
	m_Lock.Unlock();
}

void CFilesListObservable::InitObservers()
{
	NotifyObservers(0, TRUE);
}

void CFilesListObservable::TurnOff()
{
	m_Lock.Lock();
	m_mapObservers.clear();
	m_Lock.Unlock();
}