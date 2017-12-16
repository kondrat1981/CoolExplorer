// FilesListManager.h: interface for the CFilesListManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESLISTMANAGER_H__988A543C_DC88_45E6_A147_7FAEB760A368__INCLUDED_)
#define AFX_FILESLISTMANAGER_H__988A543C_DC88_45E6_A147_7FAEB760A368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FilesListBase.h"
#include "FilesListObservable.h"
#include "ThreadManager.h"


#pragma warning( disable : 4786 )
typedef std::map <CString, CString> DRIVETOPATH_MAP;
class CFilesListFtp;
class CFilesListVirtual;
struct FilesListManagerThreadInfo;


class CFilesListManager : public CFilesListObservable, public CThreadManager
{
public:
	CFilesListManager();
	virtual ~CFilesListManager();

	

	HWND GetParent(){return m_hwndParent;}
	void SetParent(HWND hParent){m_hwndParent = hParent;}
	void SetEnumMetod(BOOL bShellEnum){m_bShellEnum = bShellEnum;}
	void SetSilenceMode(BOOL bSilenceMode){m_bSilenceMode = bSilenceMode;}
	void CountDirsSize(ListData *pld);
	
	//for ftp
	BOOL InitInternetConection(const CFtpConSet *pFtpSet, CWnd *pStatusBar);
	void FtpDisconect(BOOL bHideStatus = TRUE);
	BOOL IsStillConnected();
	BOOL CheckAndReconnect();
	CFtpConnection *DuplicateFtpConnection();
	const CFtpConSet *GetFtpSet()const ;
	FtpProgressInfo *GetFtpProgressInfo();
	CFtpConnection *GetFtpConnection() const;

	//ftp end
	BOOL InitVirtualList(ListDataCreator * pListDataCreator);
	BOOL AddToVirtualList(LISTData &lst);
	BOOL RemoveFromVirtualList(const CStringList &lst);
	BOOL IsPureVirtualList();
	BOOL SaveVirtualList(LPCTSTR sFile);
	BOOL LoadVirtualList(LPCTSTR sFile);


	CFilesListBase *GetList() const {return m_pList;}

	//observable interface
	virtual CString GetDir()const;
	BOOL Select(const ListData *pld, int nDataVer);
	virtual BOOL SetDir(LPCTSTR sPath);
	virtual BOOL SetDirForDrive(LPCTSTR sDrive);

	BOOL CanContinue(int nJob){return CThreadManager::CanContinue(nJob);}
	BOOL BreakCurJob(){return CThreadManager::BreakCurJob();}
	BOOL IsCurJobStillActive(){return CThreadManager::IsCurJobStillActive();}

	//filters
	const CString &GetShowFilter() const;
	const CString &GetHideFilter() const;
	void SetFilters(LPCTSTR sFShow, LPCTSTR sFHide);
	BOOL HasFilters(CString &str) const;




protected:
	// for CThreadManager
	virtual void Process(int nJobNum, void *pData);
	virtual void JobFinished(int nJobNum, void *pData);
	virtual void JobBroken(int nJobNum, void *pData);
	//	
private:
	void SwitchList(LPCTSTR sPath);
	void PostEnum(int nDataVer, BOOL bResult);
	void PreEnum(LPCTSTR sNewPath);
	//BOOL Select(const ListData *pld, BOOL &bBrowseItem);

	CFilesListBase *SetList(CFilesListBase *pNewList);
	CFilesListBase *SetListNet();
	CFilesListBase *SetListNormal();
	CFilesListBase *SetListFtp();
	CFilesListBase *SetListCOM();
	CFilesListBase *SetListVirtual();
	
	CFilesListBase *GetListFtp();

	void SaveList();
	void RestoreList(BOOL bResult);

	HWND m_hwndParent;
	BOOL m_bShellEnum;
	BOOL m_bSilenceMode;

// lists
	CFilesListBase *m_pListNormal;
	CFilesListBase *m_pListCOM;
	CFilesListFtp *m_pListFtp;
	CFilesListBase *m_pListNet;
	CFilesListVirtual *m_pListVirtual;
	CFilesListBase *m_pList;

	CFilesListBase *m_pListPrev;

	static DRIVETOPATH_MAP mapDriveToPath;
	CString GetPathForDrive(LPCTSTR sDrive);;
	void SetPathForDrive(LPCTSTR sDrive, LPCTSTR sPath);
	//CCriticalSection m_sectList;
	//void Lock(){m_sectList.Lock();}
	//void UnLock(){m_sectList.Unlock();}
	CString m_sFilterHide;
	CString m_sFilterShow;
};

#endif // !defined(AFX_FILESLISTMANAGER_H__988A543C_DC88_45E6_A147_7FAEB760A368__INCLUDED_)
