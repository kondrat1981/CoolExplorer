#if !defined(AFX_COOLEXPLORERFRAME_H__9944EF3A_28E6_40F9_8068_346F48E42DFB__INCLUDED_)
#define AFX_COOLEXPLORERFRAME_H__9944EF3A_28E6_40F9_8068_346F48E42DFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolExplorerFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerFrame frame
#include "PathBar.h"
#include "FtpStatusBar.h"
#include "MyDriveBar.h"
//#include "DriveBar.h"

static UINT BASED_CODE indicators_down[] =
{
ID_INDICATOR_TEXT,
};

static UINT BASED_CODE indicators_up[] =
{
  ID_INDICATOR_TEXT_UP,
};

class CCoolExplorerView;
class CCoolExplorerFrame : public CFrameWnd
{
	friend CCoolExplorerView;
	DECLARE_DYNCREATE(CCoolExplorerFrame)
protected:
	CCoolExplorerFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CString m_strFrameName;

// Operations
public:
	
	CCoolExplorerView * GetView();
	void RefreshConfiguration();
	void RefreshComboDrives();
	void DoDropDownComboDrive();
	void SetFocusOnGrid();
	void RegisterObservers(CFilesListObservable *pObject);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolExplorerFrame)
	public:
	virtual void OnFinalRelease();
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL


// Implementation
protected:
	CCoolExplorerView *m_pView;
	CMyDriveBar		m_wndDriveBar;
	CStatusBar  m_wndUpStatusBar;
	CStatusBar  m_wndDownStatusBar;
//	CDialogBar m_wndPaneStatusBar;
	CPathBar m_wndPathBar;
	CFtpStatusBar m_wndFtpStatusBar;
//	CDriveBar m_wndDriveBar;
	//CToolBar m_wndToolBar;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual ~CCoolExplorerFrame();

	// Generated message map functions
	//{{AFX_MSG(CCoolExplorerFrame)
	afx_msg void OnMove(int x, int y);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnStartIPC();
	afx_msg void OnUpdateStartipc(CCmdUI* pCmdUI);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG
	//afx_msg LRESULT OnUpdatePath(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLEXPLORERFRAME_H__9944EF3A_28E6_40F9_8068_346F48E42DFB__INCLUDED_)
