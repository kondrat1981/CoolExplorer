// FilesListManager.cpp: implementation of the CFilesListManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FilesListManager.h"
#include "FilesListNormal.h"
#include "FilesListNet.h"
#include "FilesListFtp.h"
#include "FilesListCOM.h"
#include "FilesListVirtual.h"
#include "MyTimer.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DRIVETOPATH_MAP CFilesListManager::mapDriveToPath;


//CMyTimer gTimer;

struct FilesListManagerThreadInfo
{	
	CString sPath;
	ListData ld;
	CString sFullTmpPath;
	BOOL bResult;
	BOOL bDisconnectFtp;
	BOOL bHideStatus;
	BOOL bCountDirSizeJob;
	ListData *pCountDirSizePld;
	FilesListManagerThreadInfo() : bResult(FALSE), bDisconnectFtp(FALSE), bHideStatus(FALSE), 
								bCountDirSizeJob(FALSE), pCountDirSizePld(NULL){;}
};

CFilesListManager::CFilesListManager()
{
	m_pListPrev = NULL;

	m_pListCOM = NULL;
	m_pListFtp = NULL;
	m_pListNet = NULL;
	m_pListNormal = NULL;
	m_pListVirtual = NULL;
	
	if(CSettings::m_settings.bUseShellForListing)
		m_pList = m_pListCOM = new CFilesListCOM(this);
	else
		m_pList = m_pListNormal = new CFilesListNormal(this);
}

CFilesListManager::~CFilesListManager()
{
	BreakAllJobs();

	delete m_pListCOM;
	delete m_pListFtp;
	delete m_pListNet;
	delete m_pListNormal;
	delete m_pListVirtual;
}


CFilesListBase *CFilesListManager::SetListNet()
{
	Lock();
	if(NULL == m_pListNet)
	{
		m_pListNet = new CFilesListNet(this);
	}
	//if(nJob == JOBNUM_NONE || (nJob == GetCurJobNum() && CanContinue(nJob)))
	m_pList = m_pListNet;
	Unlock();

	return m_pListNet;
}

CFilesListBase *CFilesListManager::SetListCOM()
{
	Lock();
	if(NULL == m_pListCOM)
	{
		m_pListCOM = new CFilesListCOM(this);
	}

	m_pList = m_pListCOM;
	Unlock();

	return m_pListCOM;
}

CFilesListBase *CFilesListManager::SetListNormal()
{
	Lock();
	if(NULL == m_pListNormal)
	{
		m_pListNormal = new CFilesListNormal(this);
	}

	m_pList = m_pListNormal;
	Unlock();

	return m_pListNormal;
}

CFilesListBase *CFilesListManager::SetListVirtual()
{
	Lock();
	if(NULL == m_pListVirtual)
	{
		m_pListVirtual = new CFilesListVirtual(this);
	}

	m_pList = m_pListVirtual;
	Unlock();

	return m_pListVirtual;
}


CFilesListBase *CFilesListManager::GetListFtp()
{
	Lock();
	if(NULL == m_pListFtp)
	{
		m_pListFtp = new CFilesListFtp(this);
	}

	Unlock();

	return m_pListFtp;
}

CFilesListBase *CFilesListManager::SetListFtp()
{
	Lock();
	if(NULL == m_pListFtp)
	{
		m_pListFtp = new CFilesListFtp(this);
	}

	m_pList = m_pListFtp;
	Unlock();


	return m_pListFtp;
}

CFilesListBase *CFilesListManager::SetList(CFilesListBase *pNewList)
{
	Lock();
	m_pList = pNewList;
	Unlock();
	return m_pList = pNewList;
}

BOOL CFilesListManager::Select(const ListData *pld, int nDataVer)
{
	FilesListManagerThreadInfo *p = NULL;
	BOOL bRet = FALSE;
	Lock();
	{
#ifdef _DEBUG
		BOOL bActive = IsCurJobStillActive();
		int nNum = GetCurJobNum();
#endif //_DEBUG
		if(pld != NULL && !IsCurJobStillActive() && nDataVer == GetCurJobNum())
		{			
			bRet = TRUE;
			
			GetList()->LockList();
			const CString sGetName = pld->getName();
			const CString sGetDir = GetList()->GetDir();
			CString sGetExt;
			CString sFileFullPath;
			BOOL bFile = pld->isFile();
			if(bFile)
			{
				sGetExt = pld->getExtension();
				sFileFullPath = GetList()->GetFileFullPath(pld);
			}
			BOOL bDir = pld->isDir();

			GetList()->UnlockList();

			TRACE(_T("Select %s %s\n"), (LPCTSTR)sGetDir, (LPCTSTR)sGetName);
			
			if(bFile && sGetExt == _T("cvp"))
			{
				LoadVirtualList(sFileFullPath);
			}
			else
			{
				p = new FilesListManagerThreadInfo;
				if(bFile)
					p->sFullTmpPath = sGetDir;
				else
					p->sFullTmpPath = sGetDir + sGetName;

				//
				PreEnum(p->sFullTmpPath);
				//
				CString sPath = sGetDir;
				if(sGetName == RET_NAME && m_pList == m_pListNormal)
				{
					if(sPath.GetLength() == 3 && sPath.Right(2) == _T(":\\"))
					{
						SetListCOM();
						p->sPath = sPath + RET_NAME;
					}
					else
					{
						//go to parent dir
						sPath = sPath.Left(sPath.Left(sPath.GetLength() - 1).ReverseFind('\\')+1);
						if(m_pList->IsBrowseableByNet(sPath))
							SetListNet();
						p->sPath = sPath;
					}
				}
				else if(bDir && sGetName != RET_NAME 
					&& ((m_pList == m_pListCOM && !CSettings::m_settings.bUseShellForListing) || m_pList == m_pListNet)
					&& GetList()->IsBrowseableByNormal(sPath + sGetName))
				{
					SetListNormal();
					p->sPath = sPath + sGetName;
				}
				else if(bDir && !CSettings::m_settings.bUseShellForListing
					&& m_pList == m_pListNormal && !GetList()->IsBrowseableByNormal(sPath + sGetName))
				{
					SetListCOM();
					p->sPath = sPath + sGetName;
				}
				else
					p->ld = *pld;
			}
		}
	}
	Unlock();

	if(p)
		DoJob(p);

	return bRet;
}

BOOL CFilesListManager::SetDir(LPCTSTR sPath)
{
//	gTimer.Start();
	BOOL bRet = TRUE;
	PreEnum(sPath);
	SwitchList(sPath);
	FilesListManagerThreadInfo *p = new FilesListManagerThreadInfo;
	p->sPath = sPath;
	p->sFullTmpPath = sPath;
	TRACE(_T("SetDir %s\n"), sPath);
	DoJob(p);
	//BOOL bRet = m_pList->EnumFiles(sPath, nJobNum);
	//if(bRet)
	//	PostEnum();
	return bRet;
}

BOOL CFilesListManager::InitInternetConection(const CFtpConSet * pFtpSet, CWnd* pStatusBar)
{
	if(m_pListFtp && m_pListFtp->GetFtpConnection())
	{
		if(IDOK == AfxMessageBox(_T("Disconnect current FTPsession?"), MB_OKCANCEL))
			FtpDisconect();
		else
			return TRUE;
	}

	GetListFtp();
	//m_pListFtp->SetFtpStatusBar(pStatusWnd);
	BOOL bRet = m_pListFtp->InitInternetConection(pFtpSet, pStatusBar);

	if(bRet)
	{
		SetList(GetListFtp());
		PostEnum(GetCurJobNum(), TRUE);
	}
	return bRet;
}

void CFilesListManager::FtpDisconect(BOOL bHideStatus)
{
	if(m_pListFtp == NULL)
		return;
	/*FilesListManagerThreadInfo *pInfo = new FilesListManagerThreadInfo;
	pInfo->bDisconnectFtp = TRUE;
	pInfo->bDisconnectFtp = TRUE;
	DoJob(pInfo);*/
	m_pListFtp->FtpDisconect(bHideStatus);
	if(m_pList == m_pListFtp)
		SetListNormal();
	SetDir(GetDir());//refresh
}

BOOL CFilesListManager::IsStillConnected()
{
	ASSERT(m_pListFtp != NULL);
	return m_pListFtp->IsStillConnected();
}
BOOL CFilesListManager::CheckAndReconnect()
{
	ASSERT(m_pListFtp != NULL);
	return m_pListFtp->CheckAndReconnect();
}
CFtpConnection * CFilesListManager::DuplicateFtpConnection()
{
	ASSERT(m_pListFtp != NULL);
	return m_pListFtp->DuplicateFtpConnection();
}
const CFtpConSet *CFilesListManager::GetFtpSet()const
{
	ASSERT(m_pListFtp != NULL);
	return m_pListFtp->GetFtpSet();
}

FtpProgressInfo * CFilesListManager::GetFtpProgressInfo()
{
	ASSERT(m_pListFtp != NULL);
	return m_pListFtp->GetFtpProgressInfo();
}

CFtpConnection *CFilesListManager::GetFtpConnection() const
{
	if(m_pListFtp != NULL && GetList()->GetMode() == Ftp)
		return m_pListFtp->GetFtpConnection();
	return NULL;
}

CString CFilesListManager::GetDir()const
{
	Lock();
	CString sPath = m_pList->GetDir();
	Unlock();
	return sPath;
}

/*BOOL CFilesListManager::SetDir(LPCTSTR sPath)
{
	return EnumFiles(sPath);
}*/

BOOL CFilesListManager::SetDirForDrive(LPCTSTR sDrive)
{
	return SetDir(GetPathForDrive(sDrive));
}


CString CFilesListManager::GetPathForDrive(LPCTSTR sDrive)
{
	CString sPath;
	
	if(mapDriveToPath.count(sDrive) > 0)
		sPath = mapDriveToPath[sDrive];
	
	if(sPath.IsEmpty())
	{
		sPath = sDrive;
	}
	else
	{
		if(!ExistsPath(sPath))
			sPath = sDrive;
	}
	return sPath;
}

void CFilesListManager::SetPathForDrive(LPCTSTR sDrive, LPCTSTR sPath)
{
	Lock();
	if(_tcslen(sPath)>=2 && sPath[1] == ':')
		mapDriveToPath[sDrive] = sPath;
	Unlock();
}

void CFilesListManager::PostEnum(int nDataVer, BOOL bResult)
{
	RestoreList(bResult);
	SetPathForDrive(m_pList->GetDrivePath(), m_pList->GetDir());
	NotifyObservers(nDataVer, bResult);
}

void CFilesListManager::PreEnum(LPCTSTR sNewPath)
{
	SaveList();
	BreakCurJob();
	NotifyStopUse(sNewPath);
}

//
void CFilesListManager::Process(int nJobNum, void *pData)
{
	FilesListManagerThreadInfo *pInfo = (FilesListManagerThreadInfo *)pData;
	if(pInfo->bDisconnectFtp)
	{
		m_pListFtp->FtpDisconect(pInfo->bHideStatus);
	}
	else if(pInfo->bCountDirSizeJob)
	{
		pInfo->bResult = m_pList->CountDirsSize(pInfo->pCountDirSizePld, nJobNum);
	}
	else if(pInfo->sPath.IsEmpty())
	{	
		BOOL bBrowsable = FALSE;
		pInfo->bResult = m_pList->Select(&pInfo->ld, bBrowsable, nJobNum);
	}
	else
	{
		pInfo->bResult = m_pList->EnumFiles(pInfo->sPath, nJobNum);		
	}

}

void CFilesListManager::JobFinished(int nJobNum, void *pData)
{
	FilesListManagerThreadInfo *pInfo = (FilesListManagerThreadInfo *) pData;
	PostEnum(/*pInfo->sFullTmpPath,*/nJobNum, pInfo->bResult);
	pInfo->ld.setList(NULL);
	delete pInfo;
//	gTimer.Stop(TRUE);
}

void CFilesListManager::JobBroken(int nJobNum, void *pData)
{
	FilesListManagerThreadInfo *pInfo = (FilesListManagerThreadInfo *) pData;

	if(nJobNum == GetCurJobNum())
		PostEnum(nJobNum, pInfo->bResult);
	pInfo->ld.setList(NULL);
	delete pInfo;
}


//
void CFilesListManager::SwitchList(LPCTSTR sPath)
{
	if(!GetList()->IsBrowseable(sPath))
	{
		if(GetList()->IsBrowseableByVirtual(sPath))
		{
			SetListVirtual();
			return;
		}
		if(m_pListFtp && GetList()->IsBrowseableByFtp(sPath))
		{
			SetListFtp();
			return;
		}
		if(GetList()->IsBrowseableByNet(sPath))
		{
			SetListNet();
			return;
		}
		if(GetList()->IsBrowseableByNormal(sPath) && !CSettings::m_settings.bUseShellForListing)
		{
			SetListNormal();
			return;
		}
		if(GetList()->IsBrowseableByCOM(sPath))
		{
			SetListCOM();
			return;
		}
	}
}

void CFilesListManager::CountDirsSize(ListData *pld)
{
	if(!IsCurJobStillActive())
	{
		PreEnum(GetDir());
		FilesListManagerThreadInfo *p = new FilesListManagerThreadInfo;
		//p->sPath = GetDir();
		p->bCountDirSizeJob = TRUE;
		p->pCountDirSizePld = pld;
		p->sFullTmpPath = GetDir();
//		TRACE(_T("SetDir %s\n"), sPath);
		DoJob(p);
	}
}

void CFilesListManager::SaveList()
{
	Lock();
	m_pListPrev = m_pList;
	Unlock();
}

void CFilesListManager::RestoreList(BOOL bResult)
{
	Lock();
	if(!bResult && m_pListPrev && m_pListPrev != m_pList)
		m_pList = m_pListPrev;
	m_pListPrev = NULL;

	Unlock();
}

BOOL CFilesListManager::HasFilters(CString &str) const
{
	str = _T("");
	BOOL bRet = FALSE;
	BOOL m_bUseFilterShow = !m_sFilterShow.IsEmpty();
	BOOL m_bUseFilterHide = !m_sFilterHide.IsEmpty();

	if (m_bUseFilterShow &&m_bUseFilterHide)
	{
		bRet = TRUE;
		str.Format(_T_LNGSTR(116),GetShowFilter(),GetHideFilter());

	}
	else
	{
		if (m_bUseFilterShow && (!m_bUseFilterHide)) 
		{
			bRet = TRUE;
			str.Format(_T_LNGSTR(116),GetShowFilter(),_T_LNGSTR(115));
			
		}
		else if (!m_bUseFilterShow && (m_bUseFilterHide)) 
		{
			str.Format(_T_LNGSTR(116),_T_LNGSTR(115),GetHideFilter());
			
		}
	}
	return bRet;
}

void CFilesListManager::SetFilters(LPCTSTR sFShow, LPCTSTR sFHide)
{
	m_sFilterShow = sFShow;
	m_sFilterHide = sFHide;

	if(m_sFilterShow == _T("*") || m_sFilterShow == _T("*.*"))
		m_sFilterShow = _T("");// for better performance
	
}

const CString &CFilesListManager::GetShowFilter() const
{
	return m_sFilterShow;
}

const CString &CFilesListManager::GetHideFilter() const
{
	return m_sFilterHide;
}

BOOL CFilesListManager::InitVirtualList(ListDataCreator * pListDataCreator)
{
	Lock();
	BOOL bRet = FALSE;
	CString sName = VIRT_NAME;

	SetListVirtual();
	
	PreEnum(sName);
	bRet = m_pListVirtual->InitVirtualList(pListDataCreator);
	m_pList = m_pListVirtual;
	PostEnum(GetCurJobNum(), bRet);
	Unlock();

	return bRet;
}

BOOL CFilesListManager::AddToVirtualList(LISTData &lst)
{
	BOOL bRet = FALSE;
	Lock();
	{		
		if(GetList() == m_pListVirtual)
		{
			CString sName = m_pListVirtual->GetDir();
			PreEnum(sName);
			m_pListVirtual->AddToVirtualList(lst);
			bRet = TRUE;
			PostEnum(GetCurJobNum(), bRet);
		}		
	}
	Unlock();
	return bRet;
}

BOOL CFilesListManager::RemoveFromVirtualList(const CStringList &lst)
{
	BOOL bRet = FALSE;
	Lock();
	{		
		if(GetList() == m_pListVirtual)
		{
			CString sName = m_pListVirtual->GetDir();
			PreEnum(sName);
			m_pListVirtual->RemoveFromVirtualList(lst);
			bRet = TRUE;
			PostEnum(GetCurJobNum(), bRet);
		}		
	}
	Unlock();
	return bRet;
}

BOOL CFilesListManager::IsPureVirtualList()
{
	BOOL bRet = FALSE;
	Lock();
	{		
		if(GetList() == m_pListVirtual)
		{
			bRet = m_pListVirtual->IsPureVirtualList();			
		}		
	}
	Unlock();
	return bRet;
}

BOOL CFilesListManager::SaveVirtualList(LPCTSTR sFile)
{
	BOOL bRet = FALSE;
	Lock();
	{		
		if(NULL != m_pListVirtual)
		{
			bRet = m_pListVirtual->SaveToFile(sFile);
		}		
	}
	Unlock();
	return bRet;
}
BOOL CFilesListManager::LoadVirtualList(LPCTSTR sFile)
{
	BOOL bRet = FALSE;
	Lock();
	{			
		//if(NULL != m_pListVirtual)
		{
			PreEnum(VIRT_NAME);
			SetListVirtual();			
			bRet = m_pListVirtual->LoadFromFile(sFile);
			PostEnum(GetCurJobNum(), bRet);
		}		
	}
	Unlock();
	return bRet;
}