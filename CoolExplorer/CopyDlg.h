#if !defined(AFX_COPYDLG_H__E78A8FCC_098F_4BB2_B0F0_F3AF691E842A__INCLUDED_)
#define AFX_COPYDLG_H__E78A8FCC_098F_4BB2_B0F0_F3AF691E842A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CopyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCopyDlg dialog

#include "FilesTree.h"
#include "MyTimer.h"

class CProgressThread;
class CQueueDlg;

class CNewProgressDlg : public CDialog
{
	friend CQueueDlg;
public:
	BOOL PauseCopy();
	CNewProgressDlg(UINT nID, CWnd *pParent, LPCTSTR sPath, LPCTSTR sPathTo, CStringList *plFiles, CWnd* pSecWnd);
	virtual ~CNewProgressDlg();
	BOOL MainFun();
	BOOL OperationStarted() const {return m_bOperationStarted;}
	virtual void Cancel() = 0;
	CopyInfo * GetCopyInfo(CopyInfo &Info);
	CopyInfo * SetCopyInfo(CopyInfo &Info);
protected:
	virtual BOOL ThreadFun() = 0;
	virtual BOOL InitWnd() = 0;
	void FreeNotUsedWnd();
	void AllocateWnd();


	CArray<CWnd *, CWnd *> m_aRefresh;
	CopyInfo *m_pInfo;
	CopyInfo m_Info;
	CDirTree m_DirTree;
	CString m_sPathTo;
	CString m_sMes;
	CTime m_Time;
	CMyTimer m_Timer;
	BOOL m_bQueue;
	CSmartNameManager m_InfoSmart;
	
	UINT m_nWsp;
	ULONGLONG m_nSize;
	CString m_sPath;
	CString m_sMask;
	CString m_sRename;
	CStringList m_lstrFiles;

	CString m_sFilePrev;

	//pointers to windows
	CProgressCtrl	*m_pProgressOne;
	CProgressCtrl	*m_pProgress;

	CWnd *m_pWndCopied;
	CWnd *m_pWndTime;
	CWnd *m_pWndTimeElapsed;
	CWnd *m_pWndSpeed;
	CWnd *m_pWndStatus;
	CWnd *m_pWndFileName;

	BOOL m_bOperationStarted;

private:
	LONGLONG m_nCopied;
	CCriticalSection m_critSect;
	void Lock() {m_critSect.Lock();}
	void Unlock() {m_critSect.Unlock();}
	CProgressThread *m_pThread;
protected:
	BOOL ResumeCopy();
	// Generated message map functions
	//{{AFX_MSG(CCopyDlg)
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnCopyPause();
	//}}AFX_MSG
	afx_msg LRESULT OnRefresh(WPARAM type, LPARAM value);
	afx_msg LRESULT OnMyClose(WPARAM type, LPARAM value);
	DECLARE_MESSAGE_MAP()
};

class CCopyDlg : public CNewProgressDlg
{
// Construction
public:
	BOOL CopyFiles();

	CCopyDlg(LPCTSTR sPath, LPCTSTR sPathTo, CStringList *plFiles,CWnd* pParent, CWnd* pSecWnd, LPCTSTR sMes, BOOL bMove, int nFiles, int nDirs);   // standard constructor
	~CCopyDlg();

protected:
	virtual void Cancel();
	virtual BOOL ThreadFun();
	virtual BOOL InitWnd();
	BOOL m_bNotCopyEmpty;
	BOOL m_bMove;
	BOOL m_bSmartChangeName;
	int m_nFiles;
	int m_nDirs;

// Dialog Data
	//{{AFX_DATA(CCopyDlg)
	enum { IDD = IDD_COPY };	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopyDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL TryFastMove();

	// Generated message map functions
	//{{AFX_MSG(CCopyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCopyCancel();
	afx_msg void OnCopyQueue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYDLG_H__E78A8FCC_098F_4BB2_B0F0_F3AF691E842A__INCLUDED_)
