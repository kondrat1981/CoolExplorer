// FilesListFtp.h: interface for the CFilesListFtp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESLISTFTP_H__7E6567A8_EA43_47F4_A93F_22A2FC8A6318__INCLUDED_)
#define AFX_FILESLISTFTP_H__7E6567A8_EA43_47F4_A93F_22A2FC8A6318__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FilesListBase.h"

class CFilesListFtp : public CFilesListBase  
{
public:
	CFilesListFtp(CFilesListManager *pManager);
	virtual ~CFilesListFtp();

	//for base class
	virtual BOOL EnumFiles(LPCTSTR sPath, int nJobNum);
	virtual BOOL Select(const ListData *pld, BOOL &bBrowseItem, int nJob);
	virtual BOOL CreateDirectory(LPCTSTR sDir);
	virtual BOOL IsBrowseable(LPCTSTR sPath){return IsBrowseableByFtp(sPath);}
	virtual BOOL AreFilesOperationsAvailable() const {return TRUE;}


	//for ftp
	BOOL InitInternetConection(const CFtpConSet * pFtpSet, CWnd* pStatusbar);
	void FtpDisconect(BOOL bHideStatus);
	void SetFtpStatusBar(CWnd *pFtpStatusBar);
	BOOL IsStillConnected();
	BOOL CheckAndReconnect();
	CFtpConnection * DuplicateFtpConnection();
	const CFtpConSet *GetFtpSet()const {return &m_FtpSet;}
	FtpProgressInfo * GetFtpProgressInfo() { return &m_FtpProgressInfo;};
	CFtpConnection *GetFtpConnection() {return m_pFtpConnection;}

private:
	BOOL FtpEnumFiles(LPCTSTR sAddPath1, int nJob);
	CInternetSession *m_pInetSession;
	CFtpConSet m_FtpSet;
	CWnd *m_pFtpStatusBar;
	FtpProgressInfo m_FtpProgressInfo;
	CFtpConnection *m_pFtpConnection;
	CMutex m_mutexFTP;

};

#endif // !defined(AFX_FILESLISTFTP_H__7E6567A8_EA43_47F4_A93F_22A2FC8A6318__INCLUDED_)
