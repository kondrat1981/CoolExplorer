// CoolExplorerFrame.cpp : implementation file
//

#include "stdafx.h"
#include "CoolExplorer.h"
#include "CoolExplorerFrame.h"
#include "CoolExplorerDoc.h"
#include "CoolExplorerView.h"
#include "MainFrm.h"
#include <afxpriv.h>
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerFrame

IMPLEMENT_DYNCREATE(CCoolExplorerFrame, CFrameWnd)

CCoolExplorerFrame::CCoolExplorerFrame()
{
	m_pView = NULL;
}

CCoolExplorerFrame::~CCoolExplorerFrame()
{
}


BEGIN_MESSAGE_MAP(CCoolExplorerFrame, CFrameWnd)
	ON_COMMAND_EX(CG_ID_VIEW_STATUSBAR, OnBarCheck)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_DRIVEBAR, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_STATUSBAR, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CCoolExplorerFrame)
	ON_WM_MOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_COMMAND(ID_STARTIPC, OnStartIPC)
	ON_UPDATE_COMMAND_UI(ID_STARTIPC, OnUpdateStartipc)
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_UPDATEPATH, OnUpdatePath)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerFrame message handlers

BOOL CCoolExplorerFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	BOOL bRet = FALSE;
	pContext->m_pNewViewClass = RUNTIME_CLASS(CCoolExplorerView);
	bRet = CFrameWnd::OnCreateClient(lpcs, pContext);
	return bRet;
}

void CCoolExplorerFrame::SetFocusOnGrid()
{
	CCoolExplorerView *pView;
	pView = GetView();
		
	if (pView)
		pView->SetFocus();
		//pView->PostMessage(WM_POST_SETFOCUS);
		//pView->m_Grid.SetFocus();

}
int CCoolExplorerFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CSettings cfg;
	
/*  if (!m_wndToolBar.CreateEx(this, NULL, 
                              WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(IDR_PANE_BAR))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }*/

//    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//    m_wndToolBar.SetBorders(3, 3, 3, 3);
//    EnableDocking(CBRS_ALIGN_ANY);
  //  DockControlBar(&m_wndToolBar);
	{
		if (!m_wndDriveBar.Create(this, CG_IDD_DRIVEBAR,
			TBSTYLE_FLAT | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE | CBRS_SIZE_DYNAMIC,
			CG_ID_VIEW_DRIVEBAR))
		{
			TRACE0("Failed to create dialog bar m_wndDriveBar\n");
			return -1;		// fail to create
		}
		m_wndDriveBar.EnableDocking(/*CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM */CBRS_ALIGN_ANY);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndDriveBar);	
		m_wndDriveBar.ShowWindow(cfg.m_settings.bDisplayDriveBar?SW_SHOW:SW_HIDE);
//		m_wndDriveBar.ShowWindow(/* m_wndDriveBar.IsWindowVisible() ? SW_HIDE : */SW_SHOW);
	}

	if (!m_wndDownStatusBar.Create(this,WS_MAXIMIZE | WS_CHILD | WS_VISIBLE | CBRS_BOTTOM) 
	
	||
		!m_wndDownStatusBar.SetIndicators(indicators_down,
		  sizeof(indicators_down)/sizeof(UINT))
		  )
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndDownStatusBar.SetPaneStyle(0,SBPS_STRETCH|SBPS_NOBORDERS);
	m_wndDownStatusBar.SetPaneText(0,_T_LNGSTR(114),TRUE);
	m_wndDownStatusBar.ShowWindow(cfg.m_settings.bDisplayDownToolBar?SW_SHOW:SW_HIDE);

	// Initialize dialog bar m_wndDriveBar
	if (!m_wndPathBar.Create(this, CG_IDD_PATHBAR,
		CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
		CG_ID_VIEW_DRIVEBAR))
	{
		TRACE0("Failed to create dialog bar m_wndDriveBar\n");
		return -1;		// fail to create
	}
	m_wndPathBar.ShowWindow(cfg.m_settings.bDisplayDriveAndPathBar?SW_SHOW:SW_HIDE);

	if (!m_wndFtpStatusBar.Create(this, CG_IDD_FTPSTATUSBAR,
		CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
		CG_ID_VIEW_DRIVEBAR))
	{
		TRACE0("Failed to create dialog bar m_wndFtpStatusBar\n");
		return -1;		// fail to create
	}

	m_wndFtpStatusBar.ShowWindow(SW_HIDE);
	//m_wndFtpStatusBar.ShowWindow(SW_SHOW);
	

	if ((!m_wndUpStatusBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP) )||
		(!m_wndUpStatusBar.SetIndicators(indicators_up,
		  sizeof(indicators_up)/sizeof(UINT))))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndUpStatusBar.SetPaneStyle(0,SBPS_STRETCH);
	m_wndUpStatusBar.SetPaneText(0,_T_LNGSTR(113),TRUE);
	m_wndUpStatusBar.ShowWindow(cfg.m_settings.bDisplayUpToolBar?SW_SHOW:SW_HIDE);
	// TODO: Add a menu item that will toggle the visibility of the
	// dialog bar named "Status Bar":
	//   1. In ResourceView, open the menu resource that is used by
	//      the CCoolExplorerFrame class
	//   2. Select the View submenu
	//   3. Double-click on the blank item at the bottom of the submenu
	//   4. Assign the new item an ID: CG_ID_VIEW_STATUSBAR
	//   5. Assign the item a Caption: Status Bar

	// TODO: Change the value of CG_ID_VIEW_STATUSBAR to an appropriate value:
	//   1. Open the file resource.h
/*	// CG: The following block was inserted by the 'Dialog Bar' component
	{
		// Initialize dialog bar m_wndPaneStatusBar
		if (!m_wndPaneStatusBar.Create(this, CG_IDD_STATUSBAR,
			CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
			CG_ID_VIEW_STATUSBAR))
		{
			TRACE0("Failed to create dialog bar m_wndPaneStatusBar\n");
			return -1;		// fail to create
		}
	}*/

	//GetView()->SetPathBarPtr(&m_wndPathBar);
	//SetFocusOnGrid();

	//GetView()->GetFilesList()->RegisterObserver(&m_wndDriveBar);
//	m_wndCommandBar


	return 0;
}


/*
LRESULT CCoolExplorerFrame::OnUpdatePath(WPARAM wFrom, LPARAM lsPath)
{
	GetView()->DisplayFiles((LPCSTR) lsPath);
	return 1L;
}
*/

CCoolExplorerView * CCoolExplorerFrame::GetView()
{
	if(NULL == m_pView)
	{
		m_pView = (CCoolExplorerView *)GetActiveView();
	}

	return m_pView;
}

void CCoolExplorerFrame::OnFinalRelease() 
{
	CFrameWnd::OnFinalRelease();
}

BOOL CCoolExplorerFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	return CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}



void CCoolExplorerFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
	
	// TODO: Add your message handler code here
	
}
void CCoolExplorerFrame::RefreshConfiguration()
{
	CSettings cfg;
	//refresh toolbars 	2,3,4,5  on view 
/*		bDisplayDriveBar;			//Display drive bar
		bDisplayDriveAndPathBar;	//Display Drive and Path toolbar
		bDisplayUpToolBar;			//Display upper panel status
		bDisplayDownToolBar;		//Display down panel status*/

	m_wndPathBar.ShowWindow(cfg.m_settings.bDisplayDriveAndPathBar?SW_SHOW:SW_HIDE);
	m_wndUpStatusBar.ShowWindow(cfg.m_settings.bDisplayUpToolBar?SW_SHOW:SW_HIDE);
	m_wndDownStatusBar.ShowWindow(cfg.m_settings.bDisplayDownToolBar?SW_SHOW:SW_HIDE);
	m_wndDriveBar.ShowWindow(cfg.m_settings.bDisplayDriveBar?SW_SHOW:SW_HIDE);
}

BOOL CCoolExplorerFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CCoolExplorerFrame::DoDropDownComboDrive()
{
//	GetView()->OnKillFocus(&m_wndPathBar);
	GetView()->m_Grid.OnKillFocus(&m_wndPathBar);
	m_wndPathBar.m_comboDrives.ShowDropDownWindow();
	m_wndPathBar.m_comboDrives.SetFocus();
}


void CCoolExplorerFrame::RefreshComboDrives()
{
	m_wndPathBar.m_comboDrives.RefreshDrives();
	m_wndPathBar.m_comboDrives.SetActiveDrive();
	m_wndDriveBar.RemoveButtons();
    m_wndDriveBar.RefreshDrives();
}

BOOL CCoolExplorerFrame::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;//CFrameWnd::OnEraseBkgnd(pDC);
}

void CCoolExplorerFrame::OnSize(UINT nType, int cx, int cy) 
{
	ASSERT(!(cx < 40));
	m_wndDriveBar.SetRect(cx); //must be called before CFrameWnd::OnSize
	m_wndDriveBar.SetBarSize(cx); //must be called before CFrameWnd::OnSize
	CFrameWnd::OnSize(nType, cx, cy);	
}


void CCoolExplorerFrame::RegisterObservers(CFilesListObservable *pObject)
{
	pObject->RegisterObserver(&m_wndDriveBar);
	pObject->RegisterObserver(&m_wndPathBar);

}


UINT ListenClipBoard(LPVOID pData)
{	
	UINT nRet = 0;


	while (true)
	{
		COleDataObject dataObject;
		dataObject.AttachClipboard();
		if(dataObject.IsDataAvailable(CF_TEXT))//RegisterClipboardFormat(_T("FileComparer"))))
		{
			HGLOBAL hmem = dataObject.GetGlobalData(CF_TEXT);//RegisterClipboardFormat(_T("FileComparer")));
			CMemFile sf((BYTE*) ::GlobalLock(hmem), (UINT)::GlobalSize(hmem));
			CString buffer;

			LPTSTR str = buffer.GetBufferSetLength((int)::GlobalSize(hmem));
			sf.Read(str, (UINT)::GlobalSize(hmem));
			::GlobalUnlock(hmem);


		}
		Sleep(500);
	}

	return nRet;
}

void CCoolExplorerFrame::OnStartIPC() 
{
	AfxBeginThread(ListenClipBoard, NULL);
}

void CCoolExplorerFrame::OnUpdateStartipc(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

BOOL CCoolExplorerFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}
