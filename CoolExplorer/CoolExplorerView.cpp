// CoolExplorerView.cpp : implementation of the CCoolExplorerView class
//

#include "stdafx.h"
#include "CoolExplorer.h"
#include "CoolExplorerDoc.h"
#include "CoolExplorerView.h"
#include "CopyDlg.h"
#include "FtpCopyDlg.h"
#include "MarkMaskDlg.h"
#include "DelDlg.h"
#include "SimpleEdit.h"
#include "LoginFtpDlg.h"
#include "ConnectFtpDlg.h"
#include "ChangeAttribDlg.h"
#include "MyFtpFile.h"
#include "MyTimer.h"
#include "ChangeVolumeLabelDlg.h"
#include "MainFrm.h"
#include "SetFiltersDlg.h"
#include "TViewer.h"
#include "DirMonitor.h"
#include "CreateBatchDlg.h"
#include "ShellIcons.h"
#include "Shell.h"
#include "ListData.h"
#include "PathHistory.h"
#include "ItemsStorage.h"
//#define __field_bcount(x)
//#include "WinCred.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CString gsFilesPath;
CMainSearchDlg CCoolExplorerView::m_wndMainSrchDlg;
CImageList imgList;
/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerView

IMPLEMENT_DYNCREATE(CCoolExplorerView, CView)

BEGIN_MESSAGE_MAP(CCoolExplorerView, CView)
	//{{AFX_MSG_MAP(CCoolExplorerView)
	ON_WM_SIZE()
	ON_COMMAND(ID_COPY_FILE, OnCopyFile)
	ON_COMMAND(ID_MOVE_FILE, OnMoveFile)
	ON_COMMAND(ID_CREATE_DIR, OnCreateDir)
	ON_COMMAND(ID_VIEW_FILE, OnViewFile)
	ON_COMMAND(ID_EDIT_FILE, OnEditFile)
	ON_COMMAND(ID_CREATE_FILE, OnCreateFile)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FTP_CONNECTFTP, OnFtpConnect)
	ON_COMMAND(ID_FTP_DISCONECT, OnFtpDisconect)
	ON_COMMAND(ID_FTP_LOGIN, OnFtpLogin)
	ON_COMMAND(ID_CHANGE_ATTRIB, OnChangeAttrib)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_SHOW_THESAME, OnShowThesameFiles)
	ON_COMMAND(ID_SHOW_DIFFERENT, OnShowDifferentFiles)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_HIST_PREV, OnHistPrev)
	ON_COMMAND(IDC_HIST_NEXT, OnHistNext)
	ON_UPDATE_COMMAND_UI(IDC_HIST_NEXT, OnUpdateHistNext)
	ON_UPDATE_COMMAND_UI(IDC_HIST_PREV, OnUpdateHistPrev)
	ON_COMMAND(IDC_FAVORITIES, OnFavorities)
	ON_COMMAND(IDC_GOTOMAIN, OnGotomain)
	ON_UPDATE_COMMAND_UI(IDC_GOTOMAIN, OnUpdateGotomain)
	ON_COMMAND(ID_CREATE_BATCH, OnCreateBatch)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_CREATE_VIRT, OnCreateVirt)
	ON_UPDATE_COMMAND_UI(ID_CREATE_VIRT, OnUpdateCreateVirt)
	ON_COMMAND(ID_LOAD_VIRTPANEL, OnLoadVirtpanel)
	ON_UPDATE_COMMAND_UI(ID_LOAD_VIRTPANEL, OnUpdateLoadVirtpanel)
	ON_COMMAND(ID_SAVE_VIRTPANEL, OnSaveVirtpanel)
	ON_UPDATE_COMMAND_UI(ID_SAVE_VIRTPANEL, OnUpdateSaveVirtpanel)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_DELETE_FROMVIRTUAL, OnDeleteFromvirtual)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_STARTIPC, OnUpdateStartipc)
	ON_COMMAND(ID_OPERATIONS_ADDTAB, AddNewTab)
	ON_COMMAND(ID_OPERATIONS_REMOVETAB, RemoveActiveTab)
	ON_COMMAND(ID_OPER_FINDFILES, OnAppFindDialog)
	ON_COMMAND(ID_OTHERS_CHANGEVOLUMELABEL, OnAppChanegeVolLabel)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_REFRESH, OnRefresh)
	ON_MESSAGE( WM_RENAME, RenameItem )
	ON_MESSAGE(WM_UPDATE_DRIVE_CAPACITY,  RefreshDriveCapacityFrame )
	ON_MESSAGE( WM_SEARCH_WINDOW_FINISHED, OnSearchWindowFinished )
	ON_COMMAND(ID_STARTIPC, OnStartipc)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_MESSAGE(WM_SETPMENU, OnSetPMenu)
	ON_MESSAGE(WM_REFRESHICON, OnRefreshIcon)
	ON_MESSAGE(WM_POST_SETFOCUS, OnPostSetFocus)
	ON_MESSAGE(WM_EVENTUPDATE, OnEventUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerView construction/destruction

CCoolExplorerView::CCoolExplorerView() 
	:m_dir_monitor(NULL), m_listFiles()
{
	// TODO: add construction code here
	m_pDragTarget = NULL;
	m_pFilesListObject = &m_listFiles;	
	m_pContextMenu = NULL;
	m_strViewName = _T("");
	m_nDropTargetItem = -1;
	m_nDraggedItem = -1;
	//m_Grid.SetFilesList(&m_listFiles);
//	m_DropEffect = DROPEFFECT_NONE;

	
//	m_nPrevSelected = 0;
//	m_nPrevFirst = 0;
//	m_nPrevLast = 0;
	m_pHistory = NULL;
	m_bCurJobStillActive = FALSE;
	m_nDataVersion = -1;

	m_timer.Start();
}

CCoolExplorerView::~CCoolExplorerView()
{
	imgList.Detach();
	BOOL bRet = TRUE;
	POSITION pos;
	pos = m_lCopyFtpDlg.GetHeadPosition();
	while(pos)
	{
		CWnd *pWnd = m_lCopyFtpDlg.GetNext(pos);
		ASSERT(!IsWindow(pWnd->GetSafeHwnd()));
		delete pWnd;
	}
	//
	pos = m_lDelDlg.GetHeadPosition();
	while(pos)
	{
		CWnd *pWnd = m_lDelDlg.GetNext(pos);
		ASSERT(!IsWindow(pWnd->GetSafeHwnd()));
		delete pWnd;
	}
	//
	pos = m_lCopyDlg.GetHeadPosition();
	while(pos)
	{
		CNewProgressDlg *pWnd = m_lCopyDlg.GetNext(pos);
		ASSERT(!IsWindow(pWnd->GetSafeHwnd()));
		delete pWnd;
	}
	if (m_dir_monitor)
	{
		delete m_dir_monitor;
	}

	delete m_pHistory;

}
CMainFrame *CCoolExplorerView::GetMainFrame()
{
	CFrameWnd *pFrame = GetParentFrame();
	
	if(pFrame)
		pFrame->SetActiveView(this);//ccoolexplorerframe

	if(pFrame = pFrame->GetParentFrame())
	{
		return (CMainFrame*)pFrame;
	}

	return NULL;
}

BOOL CCoolExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerView drawing

void CCoolExplorerView::OnDraw(CDC* pDC)
{
	CCoolExplorerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerView printing

BOOL CCoolExplorerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCoolExplorerView::RefreshConfiguration()
{
	CSettings cfg;
	m_Grid.SetColumnName(0,_T_LNGSTR(0));
	m_Grid.SetColumnName(1,_T_LNGSTR(1));
	m_Grid.SetColumnName(2,_T_LNGSTR(2));
	m_Grid.SetColumnName(3,_T_LNGSTR(3));
	m_Grid.SetColumnName(4,_T_LNGSTR(4));
	m_Grid.ReloadGridSettings(cfg.m_settings,TRUE);
	m_listFiles.SetEnumMetod(cfg.m_settings.bUseShellForListing);

	HRESULT hr = RevokeDragDrop(this->GetSafeHwnd());
	if(!CSettings::m_settings.bDisableDragDrop)
		hr = RegisterDragDrop(GetSafeHwnd(), &m_dropTarget);



	((CCoolExplorerFrame*)GetParentFrame())->RefreshConfiguration();
	
	OnRefresh(0,0);

}
void CCoolExplorerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCoolExplorerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerView diagnostics

#ifdef _DEBUG
void CCoolExplorerView::AssertValid() const
{
	CView::AssertValid();
}

void CCoolExplorerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCoolExplorerDoc* CCoolExplorerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCoolExplorerDoc)));
	return (CCoolExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerView message handlers

void CCoolExplorerView::OnInitialUpdate() 
{
	CSettings cfg;
		// register the OLE drop target window

	/*try
    {
		m_DropTarget.Register(this);
    }catch(...)
    {
		//ModifyStyleEx(NULL,  WS_EX_ACCEPTFILES);
        DWORD error = GetLastError();
    }*/

	CView::OnInitialUpdate();
	/*CFont ft;
	ft.CreateStockObject(DEFAULT_GUI_FONT);
	SetFont(&ft);
	ft.Detach();*/
	m_listFiles.SetParent(this->GetSafeHwnd());

	CCoolExplorerFrame *pFrame = (CCoolExplorerFrame *)GetParentFrame();
	if(pFrame)
	{
//		pFrame->SetActiveView(this);
		
		//pFrame->m_wndFtpStatusBar.ShowWindow(SW_SHOW);
		// set view name
		m_strViewName = pFrame->m_strFrameName;
	}
	m_pHistory = new CPathHistory(CSettings::m_settings.m_listPathsHistory[m_strViewName == _T("CoolExpl_Left")? 0:1]);
	pFrame->m_wndPathBar.SetHistoryObject(m_pHistory);

	pFrame->RegisterObservers(&m_listFiles);
	//GetMainFrame()->RegisterObservers(&m_listFiles);
	m_listFiles.RegisterObserver(&m_Grid);
	m_listFiles.RegisterObserver(this);

	

	

	ModifyStyle(WS_BORDER, 0);	
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	RECT r ;//= this;
	GetClientRect(&r );
	
	if(imgList.m_hImageList == NULL)
		imgList.Attach(CShellIcons::GetImageList());
	m_Grid.AttachIcons(&imgList);
	m_Grid.Create(_T("FilesList"),WS_CHILD|WS_VISIBLE,r,this,11,NULL);
	
	if(!CSettings::m_settings.bDisableDragDrop)
		HRESULT hr = RegisterDragDrop(GetSafeHwnd(), &m_dropTarget);
	
	// connect the dialog to the drop target interface
	m_dropTarget.SetView(this);


	m_listFiles.InitObservers();
	
	if (m_strViewName == _T("CoolExpl_Left"))
	{
		CString s= _T_LNGSTR(0);
		m_Grid.AddColumn(_T_LNGSTR(0),cfg.m_settings.lLeftColFilenameWidth,DT_LEFT, comp_names, comp_names_desc);
		m_Grid.AddColumn(_T_LNGSTR(1),cfg.m_settings.lLeftColExtensionsWidth,DT_LEFT, comp_names, comp_names_desc);
		m_Grid.AddColumn(_T_LNGSTR(2),cfg.m_settings.lLeftColSizeWidth,DT_RIGHT, comp_sizes, comp_sizes_desc);
		m_Grid.AddColumn(_T_LNGSTR(3),cfg.m_settings.lLeftColDateWidth,DT_LEFT, comp_dates, comp_dates_desc);
		m_Grid.AddColumn(_T_LNGSTR(4),cfg.m_settings.lLeftColAttributesWidth);

	}
	else if(m_strViewName == _T("CoolExpl_Right"))
	{
		m_Grid.AddColumn(_T_LNGSTR(0),cfg.m_settings.lRightColFilenameWidth,DT_LEFT, comp_names, comp_names_desc);
		m_Grid.AddColumn(_T_LNGSTR(1),cfg.m_settings.lRightColExtensionsWidth,DT_LEFT, comp_names, comp_names_desc);
		m_Grid.AddColumn(_T_LNGSTR(2),cfg.m_settings.lRightColSizeWidth,DT_RIGHT, comp_sizes, comp_sizes_desc);
		m_Grid.AddColumn(_T_LNGSTR(3),cfg.m_settings.lRightColDateWidth,DT_LEFT, comp_dates, comp_dates_desc);
		m_Grid.AddColumn(_T_LNGSTR(4),cfg.m_settings.lRightColAttributesWidth);
	}

	m_Grid.SetFunWnd(this);
	m_Grid.SetFunSel((PMESSFUN)&CCoolExplorerView::NSelectRow);
	m_Grid.SetFunRightClick((PMESSFUN)&CCoolExplorerView::NRightClick);
	m_Grid.SetFunMarkingChanged((PMESSFUN_NONE)&CCoolExplorerView::NMarkingChanged);

	//aditing method;
	m_Grid.SetFunEditItem((PMESSFUN_TWO_STR)&CCoolExplorerView::NEditChanged);
	m_Grid.SetFunCheckCommandBar((PMESSFUN_NONE)&CCoolExplorerView::CheckIsEmptyCommandBar);
	m_Grid.SetFunRunCommandBar((PMESSFUN_NONE)&CCoolExplorerView::RunCommandBar);
	m_Grid.SetFunKeyPressed((PMESSFUN_CUST_KEY)&CCoolExplorerView::KeyPressed);

	CString sDir;
	std::vector<CString>::iterator itr;
	std::vector<CString> &tab = m_strViewName == _T("CoolExpl_Left") ? cfg.m_settings.sLeftDir : cfg.m_settings.sRightDir;
	if(tab.size() >= 1)
		sDir = tab[0];
	if(tab.size() > 1)
	{
		for(itr = tab.begin(); itr != tab.end(); ++itr)
		{	
			m_Grid.AddTab(&(*itr));
		}
	}

	m_dir_monitor = new CDirMonitor();
	m_dir_monitor->SetCALLBACK(this->m_hWnd);
		
	BOOL bLoaded = m_listFiles.LoadVirtualList(AddPathNames(gsFilesPath, m_strViewName + _T("VirtPanel") + _T(".cvp")));
	if(bLoaded && !_tcsncmp(sDir, VIRT_NAME, _tcslen(VIRT_NAME)))
	{
		//nothing to - list is already loaded		
	}
	else
	{
		if(!IsFileSystem(sDir))
			sDir =  _T("C:\\");		
		
		
		m_listFiles.SetSilenceMode(TRUE);
		m_pFilesListObject->SetDir(sDir);
		m_listFiles.SetSilenceMode(FALSE);
	}

	GetParentFrame()->SetActiveView(this);

//	ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	//m_OleDropTarget.Register(this);

	//create fast file search dialog
	RECT rect; 
	rect.top = 0;
	rect.left = 0;
	rect.bottom = 100;
	rect.right = 100;
	m_wndFastFileSrchDlg.SetPointers(this);
	m_wndFastFileSrchDlg.Create(NULL,NULL,0,rect,GetParentFrame()->GetParentFrame(),NULL);



	pFrame->RegisterObservers(&m_listFiles);
//	GetMainFrame()->RegisterObservers(&m_listFiles);

}


void CCoolExplorerView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	//m_Grid.Resize(int cx, int cy);
	if (::IsWindow(m_Grid.GetSafeHwnd()))
	{
		m_Grid.OnSize(nType, cx, cy);
	}

	// TODO: Add your message handler code here
	
}

void CCoolExplorerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

LRESULT CCoolExplorerView::OnRefresh(WPARAM wp, LPARAM)
{
	if(wp)
	{
		SetTimer(1, 1000, NULL);
	}
	else
	{
	if(m_listFiles.IsCurJobStillActive())
		return 0L;

	m_pFilesListObject->SetDir(m_listFiles.GetDir());
	}

	return 1L;
	
	//	
/*	if(m_Grid.GetNumRows() < m_Grid.GetCountPerPage())
	{
		nFirst = 0;
		nLast = m_Grid.GetNumRows();
	}
	else
	{
		if(nFocused != -1) //focused was visible
		{
			int nCur = m_Grid.GetFocusedRow();
			int nDiff = nFocused - nFirst;
			nFirst = nCur - nDiff;
		}
		if(nFirst > m_Grid.GetNumRows()-m_Grid.GetCountPerPage())
			nFirst = m_Grid.GetNumRows()-m_Grid.GetCountPerPage();
		
		nLast = nFirst + m_Grid.GetCountPerPage();
	}
	//
	
	m_Grid.SetDispRange(nFirst, nLast);
	m_Grid.RecalcScrollBar();
	
	return 1;*/
}

BOOL CCoolExplorerView::DisplayFiles(LPCTSTR sPath, ListData *pd /*=NULL*/)
{
	BOOL bRet = FALSE;
/*	if(IsSharedFtpSession())
		return FALSE;

	CWaitCursor wcr;


	tm.Start();
	m_timer.Start();
	CString sTime;
	BOOL bRefresh = TRUE;
	CString sToSelect;
	int nToSelect = 0;
	int nNumDirs =0 ,nNumFiles = 0;
	if(pd)
	{
		if(pd->getName() == RET_NAME)
			sToSelect = m_listFiles.GetList()->GetParentName();
		bRet = m_listFiles.Select(pd, bRefresh);
	}
	else if(sPath && _tcslen(sPath))
	{
		if(m_listFiles.GetDir() == sPath)
		{
			nToSelect = m_Grid.GetFocusedRow();
			sToSelect = m_Grid.GetItemText(nToSelect);			
		}
		if (sPath == _T_LNGSTR(9)) //_T_LNGSTR(126)
		{
		}

		bRet = m_listFiles.SetDir(sPath);	
	}
	m_timer.Stop();
	sTime += _T("\nListing: ");
	sTime += m_timer.GetStringTime();

	m_timer.Start();

	//m_timer.Stop(TRUE);
	this->PostMessage(WM_UPDATE_DRIVE_CAPACITY);

	if(bRefresh)
	{
		//m_Grid.RemoveAllRows();
		m_Grid.AllocMem(m_listFiles.GetList()->GetAllCount());
		
		//vector<CString> aColumns;
		//aColumns.resize(5);
		POSITION pos = m_listFiles.GetList()->GetHeadPosition();

		while(pos)
		{
			pd = m_listFiles.GetList()->GetNext(pos);
			m_Grid.AddRow( pd);
			//count number of dirs, to display it on statusbar
			pd->getAttrib() & CFile::directory ? nNumDirs++:nNumFiles++;
		}

		m_timer.Stop();
		sTime += _T("\nAfter Add to grid: ");
		sTime += m_timer.GetStringTime();
		
		m_timer.Start();
		
		m_Grid.Sort();
		m_timer.Stop();
		sTime += _T("\nSort: ");
		sTime += m_timer.GetStringTime();
		m_timer.Start();
		
		m_Grid.RecalcScrollBar();
		if (sPath == _T_LNGSTR(9)) //_T_LNGSTR(126)
		{
			m_pFilesListObject->SetDir(sPath);
		}
		else
		{
			m_pFilesListObject->SetDir(m_listFiles.GetDir());
		}

		if(!sToSelect.IsEmpty() && sToSelect!=RET_NAME)
		{
			BOOL bRet = m_Grid.SelectRow(sToSelect,0,TRUE);
			if(!bRet)
			{
				if(nToSelect>=m_Grid.GetNumRows())
					nToSelect=m_Grid.GetNumRows()-1;
				m_Grid.SelectRow(nToSelect);
			}
		}
		else
			m_Grid.SelectRow(0);

		m_Grid.InvalidateRect(NULL);
		//dsplay info in status bar

//		m_Grid.GetNumMarkedRows()
		//display information in down status bar
		NMarkingChanged();

		
	}
	//AfxMessageBox(m_listFiles.GetDirForParsing());

	m_timer.Stop();
	tm.Stop();
	sTime += _T("\nEnding: ");
	sTime += m_timer.GetStringTime();
	sTime += _T("\nAll: ");
	sTime += tm.GetStringTime();
	sTime += _T("\n");

	TRACE(sTime);*/

	return bRet;
}


BOOL CCoolExplorerView::NSelectRow(int nCol)
{
	ListData *pd = (ListData *)m_Grid.GetUserData(nCol);
	m_pFilesListObject->Select(pd, m_nDataVersion);
	return TRUE;
}

BOOL CCoolExplorerView::NRightClick(int nRow)
{
	CPoint clickpoint;
	GetCursorPos( &clickpoint );

	LISTDataPointers listPointers;

	//if clicked none of items (nRow == -1 - menu for current dir will be displayed - listPointers empty
	m_listFiles.GetList()->LockList();
	if(nRow >=0)
	{
		GetMarkedItems(listPointers);		
	}
	CItemsStorage *pData = m_listFiles.GetList()->SaveItems(listPointers);
	m_listFiles.GetList()->UnlockList();
	if(pData)
	{
		pData->DisplayExplorerMenu(clickpoint);
		delete pData;
		pData = NULL;
	}
  
	//
//	DisplayFiles(m_listFiles.GetDir());
//	m_Grid.FocusRow(nRow);
	return TRUE;
}
LRESULT CCoolExplorerView::RenameItem(WPARAM wparam, LPARAM lparam)
{
	if(m_listFiles.IsCurJobStillActive())
		return 0L;
	
	if(!m_listFiles.GetList()->AreFilesOperationsAvailable())
	{
		AfxMessageBox(_T_LNGSTR(228), MB_ICONERROR);
		return 0;
	}
	m_Grid.EnableRenameMode(0,0);
	return 0;
}
BOOL CCoolExplorerView::NMarkingChanged()
{
	CString str = _T("");	
	CString strFilters = _T("");	

	int nNumMarkedFiles = 0,nNumMarkedDirs = 0;
	int nNumUnMarkedFiles = 0,nNumUnMarkedDirs = 0;
	GetNamesCount(nNumMarkedFiles,nNumUnMarkedFiles,nNumMarkedDirs,nNumUnMarkedDirs);		
	
	if (m_listFiles.HasFilters(strFilters))
	{

		str.Format(_T_LNGSTR(114),nNumMarkedFiles,nNumMarkedFiles+nNumUnMarkedFiles,
				nNumMarkedDirs,nNumUnMarkedDirs+nNumMarkedDirs,strFilters);
	}
	else
	{
		str.Format(_T_LNGSTR(114),nNumMarkedFiles,nNumMarkedFiles+nNumUnMarkedFiles,
				nNumMarkedDirs,nNumUnMarkedDirs+nNumMarkedDirs,_T(""));
	}

	if (((CCoolExplorerFrame*)GetParentFrame()))
	{
		((CCoolExplorerFrame*)GetParentFrame())->m_wndDownStatusBar.SetPaneText(0,str + _T("\t") + m_timer.GetStringTime(),TRUE);
	}
	return TRUE;
}

BOOL CCoolExplorerView::CheckIsEmptyCommandBar()
{
	BOOL bRet = TRUE;
	
	CMainFrame *main_frame = GetMainFrame();
	
	if (main_frame != NULL)
	{
		//check command bar is empty
		bRet = main_frame->IsEmptyCommandBar();
	}
	return bRet;
}

BOOL CCoolExplorerView::RunCommandBar()
{
	BOOL bRet = TRUE;
	CMainFrame *main_frame = GetMainFrame();
	
	if (main_frame != NULL)
	{
		//check command bar is empty
		CString strCommand = main_frame->GetCommandBarText();
		main_frame->EraseCommandBarText();
		bRet = main_frame->SendMessage(WM_DO_COMMAND, 0, (LPARAM)(LPCTSTR)strCommand)!= 0;
	}
	return bRet;
}

BOOL CCoolExplorerView::KeyPressed(UINT nChar, UINT nRepCnt, UINT nFlags, CString str)
{
	BOOL bRet = TRUE;

	if (!str.IsEmpty())
	{
		CMainFrame *main_frame = GetMainFrame();
		
		if (main_frame != NULL)
		{
			main_frame->SetCommandBarText(str,TRUE);
		}
	}
	return bRet;
}

BOOL CCoolExplorerView::NEditChanged(CString &from,CString &to)
{
	if (from == to)
	{
		return FALSE;
	}

	CString dir = m_listFiles.GetDir();
	CString sFrom = dir + from;
	CString sTo = dir + to;

	UINT nBufLen = sFrom.GetLength()+2;
	LPCTSTR szOldBuf = StringToBuf(sFrom);
	LPCTSTR szNewBuf = StringToBuf(sTo);

	
	
	SHFILEOPSTRUCT fop;
	
	
	fop.hwnd = GetParent()->GetSafeHwnd(); 
	fop.wFunc = FO_RENAME; 
	fop.pFrom = szOldBuf;
	fop.pTo = szNewBuf;
	fop.fFlags = 0;
	fop.fAnyOperationsAborted = TRUE; 
	fop.hNameMappings = NULL; 
	fop.lpszProgressTitle = _T(""); 
	
	//m_evNotify.SetEvent();
	
	if (SHFileOperation(&fop))
	{
		DWORD d = GetLastError();
		m_pFilesListObject->SetDir(m_listFiles.GetDir());		
	}
	else
	{
		m_pFilesListObject->SetDir(m_listFiles.GetDir());
		m_sPrevItemToSelect = to;
		//m_Grid.SelectRow(to,0,TRUE);
	}
	
	
	delete [] (LPTSTR) szNewBuf;
	delete [] (LPTSTR) szOldBuf;


	return TRUE;


}
/*
void CCoolExplorerView::PathChanged(LPCTSTR sPath, const void *pData, int nPanel)
{
	BOOL bRet = FALSE;
	
	if(pData == NULL) // not from grid
	{
		bRet = DisplayFiles(sPath);
	
	}
	else
	{
		bRet = DisplayFiles(NULL, (ListData *)pData);
	}
	if (bRet == TRUE)
	{
		CSettings cfg;
		if (cfg.m_settings.bRefreshDirs && m_dir_monitor)
		{
			m_dir_monitor->SetMonitor(CString(m_listFiles.GetDir()));
		}
	}
	

}*/

void CCoolExplorerView::OnAppChanegeVolLabel()
{
	CChangeVolumeLabelDlg m_volDialog;

	//get current driva

	CString sDrive = m_listFiles.GetList()->GetDrive();

	m_volDialog.SetDrive(sDrive);

	if (m_volDialog.DoModal() == IDOK)
	{
		BOOL bResult = SetVolumeLabel(m_volDialog.GetDrive(),m_volDialog.GetNewLabel());

		if (!bResult)
		{
			ASSERT(0);
			CoolTrace(0, TRUE, _T(" CCoolExplorerView::OnAppChanegeVolLabel(), SetVolumeLabel error from drive %s(label:%s) is %d"), 
				m_volDialog.GetDrive(),m_volDialog.GetNewLabel(),GetLastError());
		}

	}
	RefreshDriveCapacityFrame(0,0);

}

void CCoolExplorerView::OnAppFindDialog( )
{
	if (::IsWindow(m_wndMainSrchDlg.GetSafeHwnd()))
	{
		if (m_wndMainSrchDlg.IsWindowVisible())
		{
			m_wndMainSrchDlg.SetFocus();
			m_wndMainSrchDlg.CenterWindow();
		}
		else
		{
			m_wndMainSrchDlg.m_strFindIn = m_listFiles.GetDir();
			m_wndMainSrchDlg.ShowWindow(SW_SHOW);

			m_wndMainSrchDlg.CenterWindow();
		}
	}
	else
	{
		m_wndMainSrchDlg.m_strFindIn = m_listFiles.GetDir();

		m_wndMainSrchDlg.Create(IDD_MAIN_SRCH_DLG,this);

		m_wndMainSrchDlg.ShowWindow(SW_SHOW);

		m_wndMainSrchDlg.CenterWindow();
	
	}

	/*	(DoModal() == IDOK)
	{
		if(!m_wndMainSrchDlg.GetPath().IsEmpty() && !m_wndMainSrchDlg.GetFileName().IsEmpty())
		{
			DisplayFiles( m_wndMainSrchDlg.GetPath());
			m_Grid.SelectRow(m_wndMainSrchDlg.GetFileName(),0,TRUE);
			
		}

	}*/
}

void CCoolExplorerView::DeleteFile(BOOL bFromVirtual, BOOL bUseShell)
{
	if(m_listFiles.IsCurJobStillActive())
		return;

	if(!m_listFiles.GetList()->AreFilesOperationsAvailable())
	{
		AfxMessageBox(_T_LNGSTR(228), MB_ICONERROR);
		return;
	}

	int nItem = -1;
	CStringList lNames;
	//TCHAR buff[_MAX_PATH];

	CString sMes;
	GetMarkedNames(lNames, &sMes);
	
	if(lNames.IsEmpty())
	{
		MessageBox(_T_LNGSTR(33));
		return;
	}

	
	//GetDocument()->DeleteFiles(m_listFiles.GetDir(), lNames);
	// for Virtual Panel if pressed:
	// del -> delete from disk
	// ctrl + del - delete only from panel
	if(bFromVirtual)
	{
		if(MessageBox(_T_LNGSTR(247), NULL, MB_ICONQUESTION | MB_OKCANCEL ) == IDOK)//("Links will be deleted only from virtual panel. Continue?"))
		{
			
			m_listFiles.RemoveFromVirtualList(lNames);
			m_Grid.MarkAllRows(FALSE);
			m_Grid.RedrawWindow();
		}
	}
	else if(bUseShell && m_listFiles.GetList()->GetMode() != Ftp)
	{	
		LPCTSTR sFiles = GetMarkedNamesForShell();
		if(sFiles)
		{
			SHFILEOPSTRUCT shi = {0};
			shi.hwnd = AfxGetMainWnd()->GetSafeHwnd(); 
			shi.wFunc = FO_DELETE; 
			shi.pFrom = sFiles;
			//shi.pTo = sTo;
			shi.fFlags = 0;
			shi.fAnyOperationsAborted = TRUE; 
			shi.hNameMappings = NULL; 
			shi.lpszProgressTitle = _T(""); 

			SHFileOperation(&shi);
			delete [] sFiles;
			PostMessage(WM_REFRESH);
			GetAnotherView()->PostMessage(WM_REFRESH);
		}
	}
	else
	{
		if(m_listFiles.GetList()->GetMode() == Virtual)
		{
			if(MessageBox(_T_LNGSTR(248), NULL, MB_ICONQUESTION | MB_OKCANCEL ) != IDOK)//("Files will be deleted fro DISK. Continue?")
				return;
		}
		CDelDlg *dlg = new CDelDlg(m_listFiles.GetFtpConnection(),
			m_listFiles.GetList()->GetFileFullPath(NULL), &lNames, this, GetAnotherView());
		dlg->Create();
		m_lDelDlg.AddTail(dlg);
		if(CSettings::m_settings.bRefreshDirs && m_dir_monitor)
			m_dir_monitor->DisableRefresh();
	}
	// after cancel on delete dialog
/*	if(!::IsWindow(dlg->GetSafeHwnd()))
		this->m_sSelBeforeDelete = _T("");*/
}

/*void CCoolExplorerView::OnShrecycleDeleteFile() 
{
	SHFILEOPSTRUCT fop;
	LPCTSTR pbuf = GetSelectedItems();
    fop.hwnd = GetParent()->GetSafeHwnd(); 
    fop.wFunc = FO_DELETE ; 
    fop.pFrom = pbuf; 
    fop.pTo = _T(""); 
    fop.fFlags = FOF_ALLOWUNDO; 
    fop.fAnyOperationsAborted = TRUE; 
    fop.hNameMappings = NULL; 
    fop.lpszProgressTitle = _T(""); 

	int nRet = SHFileOperation(&fop);
//	GetDocument()->UpdateAllViews(NULL, 0);
	delete [] (void *)pbuf;
}*/

LPCTSTR CCoolExplorerView::GetMarkedNamesForShell()
{
	int buflen = 0;
	CStringList lNames;
	GetMarkedNames(lNames, NULL, FALSE, TRUE);
	if(!lNames.GetCount())
		return NULL;

	POSITION pos = lNames.GetHeadPosition();

	while(pos)
	{
		buflen += lNames.GetNext(pos).GetLength() + 1;
	}
	buflen += 4;
	TCHAR * buf = new TCHAR[buflen * sizeof(TCHAR)];
	TCHAR * const bufret = buf;
	memset(buf, '\0', buflen * sizeof(TCHAR));

	pos = lNames.GetHeadPosition();

	while(pos)
	{
		_tcscpy(buf, lNames.GetNext(pos));
		buf += _tcslen(buf) + 1;
	}
	return bufret;
}

void CCoolExplorerView::OnCopyMoveFile(BOOL bMove, BOOL bUseShell)
{
	if(m_listFiles.IsCurJobStillActive())
		return;

	CFilesListManager *pSecondList = &(GetAnotherView()->m_listFiles);
	if(!m_listFiles.GetList()->AreFilesOperationsAvailable() || !pSecondList->GetList()->AreFilesOperationsAvailable())
	{
		AfxMessageBox(_T_LNGSTR(227), MB_ICONERROR);
		return;
	}

	if(bMove && pSecondList->IsPureVirtualList())
	{
		AfxMessageBox(_T_LNGSTR(243), MB_ICONINFORMATION);
		return;
	}

	int nItem = -1;
	CStringList lNames;
	CSettings cfg;
	
	//TCHAR buff[_MAX_PATH];

	CString sMes;
	int nCountFiles = 0;
	int nCountDirs = 0;
	GetMarkedNames(lNames, &sMes, FALSE, FALSE, &nCountFiles, &nCountDirs);
	
	if(lNames.IsEmpty())
	{
		AfxMessageBox(_T_LNGSTR(100), MB_ICONINFORMATION);
		return;
	}
/*	else
	{
		CString str;
		str.Format("%d", lNames.GetCount());
	//	MessageBox(str);
	}

*/
	CString sPathFrom =m_listFiles.GetList()->GetFileFullPath(NULL);
	CString sPathTo =pSecondList->GetList()->GetFileFullPath(NULL);
		
	if((m_listFiles.GetList()->GetMode() != Ftp) && (pSecondList->GetList()->GetMode() != Ftp))
	{
		if(pSecondList->IsPureVirtualList())
		{
			{
				LISTData lst;
				CreateList(lst);
				pSecondList->AddToVirtualList(lst);
				m_Grid.MarkAllRows(FALSE);
				m_Grid.RedrawWindow();
			}
		}
		else if(bUseShell)
		{
			LPCTSTR sFiles = GetMarkedNamesForShell();
			if(sFiles)
			{
				int size = sPathTo.GetLength() + 4;
				TCHAR * sTo = new TCHAR[size*2];
				memset(sTo, 0, size * sizeof(_TCHAR));
				_tcscpy(sTo, sPathTo);
				
				SHFILEOPSTRUCT shi = {0};
				//shi.
				shi.hwnd = AfxGetMainWnd()->GetSafeHwnd(); 
				shi.wFunc = bMove ? FO_MOVE :FO_COPY; 
				shi.pFrom = sFiles;
				shi.pTo = sTo;
				shi.fFlags = 0;
				//shi.fAnyOperationsAborted = TRUE; 
				shi.hNameMappings = NULL; 
				shi.lpszProgressTitle = _T(""); 

				SHFileOperation(&shi);
				delete [] sTo;
				delete [] sFiles;
				PostMessage(WM_REFRESH);
				GetAnotherView()->PostMessage(WM_REFRESH);
			}
		}
		else
		{
			//GetDocument()->CopyPath(m_listFiles.GetDir(), lNames);	
			CCopyDlg *pdlg = new CCopyDlg(sPathFrom, sPathTo, &lNames, this, GetAnotherView(), sMes, bMove, nCountFiles, nCountDirs);
			m_lCopyDlg.AddTail(pdlg);
			if(pdlg->OperationStarted())
				m_Grid.MarkAllRows(FALSE);
		}
	}
	else
	{
		CFtpConnection *pFtpThis = NULL;
		CFtpConnection *pFtpSecond = NULL;
		BOOL bFree1ftp = FALSE;
		BOOL bFree2ftp = FALSE;
		if(pSecondList->GetList()->GetMode() == Ftp)
		{
			if(GetAnotherView()->IsSharedFtpSession())
				return;

			pSecondList->CheckAndReconnect();
			if(cfg.m_settings.bCreateFtpConForCopy)
				pFtpSecond = pSecondList->DuplicateFtpConnection();
			if(pFtpSecond == NULL)
				pFtpSecond = pSecondList->GetFtpConnection();
			else
				bFree2ftp = TRUE;

		}
		if(m_listFiles.GetList()->GetMode() == Ftp)
		{
			if(IsSharedFtpSession())
				return;
			if(pSecondList->IsPureVirtualList())
			{
				AfxMessageBox(_T_LNGSTR(238), MB_ICONINFORMATION);
				return;
			}
		
			m_listFiles.CheckAndReconnect();
			if(cfg.m_settings.bCreateFtpConForCopy)
				pFtpThis = m_listFiles.DuplicateFtpConnection();
			if(pFtpThis == NULL)
				pFtpThis = m_listFiles.GetFtpConnection();
			else
				bFree1ftp = TRUE;
		}

		CFtpCopyDlg *pdlg = new CFtpCopyDlg(pFtpThis, pFtpSecond, bFree1ftp, bFree2ftp,sPathFrom, sPathTo, &lNames, this, GetAnotherView(), 
			pSecondList->GetList()->GetMode() == Ftp ? pSecondList->GetFtpProgressInfo() : m_listFiles.GetFtpProgressInfo());
		if(pSecondList->GetList()->GetMode() == Ftp)
			GetAnotherView()->m_lCopyFtpDlg.AddTail(pdlg);
		else
			m_lCopyFtpDlg.AddTail(pdlg);
		if(pdlg->OperationStarted())
			m_Grid.MarkAllRows(FALSE);
	}
	
}

void CCoolExplorerView::OnCopyFile() 
{
	OnCopyMoveFile(FALSE, FALSE);
}

void CCoolExplorerView::OnMoveFile()
{
	OnCopyMoveFile(TRUE, FALSE);
}

void CCoolExplorerView::OnCreateDir()
{
	if(m_listFiles.IsCurJobStillActive())
		return;

	
	if(!m_listFiles.GetList()->AreFilesOperationsAvailable())
	{
		AfxMessageBox(_T_LNGSTR(228), MB_ICONERROR);
		return;
	}

	if(m_listFiles.IsPureVirtualList())
	{
		AfxMessageBox(_T_LNGSTR(240), MB_ICONERROR);
		return;
	}

	CSimpleEdit dlg(_T_LNGSTR(133), _T(""), GetParentFrame());
	if(dlg.DoModal() == IDOK)
	{
		CString sDir = dlg.GetText();
		if(FALSE == m_listFiles.GetList()->CreateDirectory(sDir))
		{
			ErrorMessage((CString)_T_LNGSTR(134) + sDir);

			//tu security
			/*CREDUI_INFO cui;
			TCHAR pszName[CREDUI_MAX_USERNAME_LENGTH+1];
			TCHAR pszPwd[CREDUI_MAX_PASSWORD_LENGTH+1];
			BOOL fSave;
			DWORD dwErr;

			cui.cbSize = sizeof(CREDUI_INFO);
			cui.hwndParent = NULL;
			//  Ensure that MessageText and CaptionText identify what credentials
			//  to use and which application requires them.
			cui.pszMessageText = TEXT("Enter administrator account information");
			cui.pszCaptionText = TEXT("CredUITest");
			cui.hbmBanner = NULL;
			fSave = FALSE;
			SecureZeroMemory(pszName, sizeof(pszName));
			SecureZeroMemory(pszPwd, sizeof(pszPwd));
			dwErr = CredUIPromptForCredentials( 
				&cui,                         // CREDUI_INFO structure
				TEXT("TheServer"),            // Target for credentials
				//   (usually a server)
				NULL,                         // Reserved
				0,                            // Reason
				pszName,                      // User name
				CREDUI_MAX_USERNAME_LENGTH+1, // Max number of char for user name
				pszPwd,                       // Password
				CREDUI_MAX_PASSWORD_LENGTH+1, // Max number of char for password
				&fSave,                       // State of save check box
				CREDUI_FLAGS_GENERIC_CREDENTIALS |  // flags
				CREDUI_FLAGS_ALWAYS_SHOW_UI |
				CREDUI_FLAGS_DO_NOT_PERSIST);  

			if(!dwErr)
			{
				//  Put code that uses the credentials here.

				//  When you have finished using the credentials,
				//  erase them from memory.
				SecureZeroMemory(pszName, sizeof(pszName));
				SecureZeroMemory(pszPwd, sizeof(pszPwd));
			}*/

		}
		else
		{
			
			m_sPrevItemToSelect = sDir;
			m_pFilesListObject->SetDir(m_listFiles.GetDir());			
			//SaveSelection(m_listFiles.GetDir());
			//m_Grid.SelectRow(sDir,0,TRUE);
		}
	}
	
}

void CCoolExplorerView::OnViewFile()
{
	if(m_listFiles.IsCurJobStillActive())
		return;

	CSettings cfg;
	CString sViewer;
	CStringList lNames;
	CString sMes;

	int iRow = m_Grid.GetFocusedRow();

	if(iRow < 0 || iRow >= m_Grid.GetNumRows())
	{
		AfxMessageBox(_T_LNGSTR(125), MB_ICONINFORMATION);//"Please select file!"
		return;
	}

	ListData *pd = (ListData *)m_Grid.GetUserData(iRow);

	CString sFileToView = m_listFiles.GetList()->GetFileFullPath(pd);


	if (cfg.m_settings.bDefaultViewer == TRUE)
	{
		CString param = _T(" -f ");
		param += "\"";
		param += sFileToView;
		param += "\"";
//		CreateProcess(AddPathNames(gsFilesPath,_T("CoolViewer.exe")),param,NULL,NULL,FALSE,)
		CString temp = CString(AddPathNames(gsFilesPath,_T("CoolViewer.exe")+param));
#ifndef UNICODE
		WinExec(temp, SW_SHOW);
#else
		{
			USES_CONVERSION;
			WinExec(W2A(temp), SW_SHOW);
		}
#endif

	}
	else
	{
		sViewer = cfg.m_settings.strViewer;
		OpenSelectedFile(sViewer);
	}

/*	if (!sViewer.IsEmpty())
	{
		//get list of files

		CString sMes;
		int iRow = m_Grid.GetFocusedRow();
		
		if(iRow < 0 || iRow >= m_Grid.GetNumRows())
		{
			MessageBox(_T("Please select file to edit!"));
			return;
		}

		ListData *pd = (ListData *)m_Grid.GetUserData(iRow);

		CString sFileToView = m_listFiles.GetDir() + pd->info.sName;
		if(m_listFiles.GetList()->GetMode() == Ftp)
		{
			CMyFtpFile file(m_listFiles.GetFtpConnection());
			CString sTmp;
			if(!file.GetFileToTemp(sFileToView, sTmp))
			{
				MessageBox(_T_LNGSTR(244), NULL, MB_ICONWARNING);//"Error reading Ftp file"
			}
			sFileToView = sTmp;			
		}

		sViewer += " ";
		sViewer += sFileToView;		

#ifndef UNICODE
			WinExec(sViewer, SW_SHOWMAXIMIZED);
#else
		{
			USES_CONVERSION;
			WinExec(W2A(sViewer), SW_SHOWMAXIMIZED);
		}
#endif

	}*/

}

void CCoolExplorerView::OpenSelectedFile(LPCTSTR sProgram)
{
	CString sCommand = sProgram;
	if (!sCommand.IsEmpty())
	{
		//get list of files

		CString sMes;
		int iRow = m_Grid.GetFocusedRow();
		
		if(iRow < 0 || iRow >= m_Grid.GetNumRows())
		{
			AfxMessageBox(_T_LNGSTR(125), MB_ICONINFORMATION);
			return;
		}

		ListData *pd = (ListData *)m_Grid.GetUserData(iRow);

		if(pd->getName() != RET_NAME && !pd->isDir())
		{
			CString sFileToView = m_listFiles.GetList()->GetFileFullPath(pd);
			if(m_listFiles.GetList()->GetMode() == Ftp)
			{
				CMyFtpFile file(m_listFiles.GetFtpConnection());
				CString sTmp;
				if(!file.GetFileToTemp(sFileToView, sTmp,pd->getSize()))
				{
					MessageBox(_T_LNGSTR(244), NULL, MB_ICONWARNING);//"Error reading Ftp file"
				}
				m_listFiles.CheckAndReconnect();
				sFileToView = sTmp;
			}
			
			sCommand += " \"";
			sCommand += sFileToView;
			sCommand += "\"";

			
#ifndef UNICODE
			//		if(pd->info.lSize < 1000000)
			WinExec(sCommand, SW_SHOWMAXIMIZED);
#else
			{
				USES_CONVERSION;
				WinExec(W2A(sCommand), SW_SHOWMAXIMIZED);
			}
#endif
/*			CCoolViewer viewer;
			char name[255] = {0};
			USES_CONVERSION;
			sprintf(name,"%s",W2A(sFileToView));
			viewer.InitMainViewerWindow(AfxGetInstanceHandle());
			
			
			
			viewer.ViewFile(name);*/

//			TViewer viewer;

//			m_viewer_list.push_back(viewer);


			//m_viewer_list[m_viewer_list.size()-1];//.CreateViewer(AfxGetApp()->m_pMainWnd,sFileToView);

			//m_viewer.CreateViewer(AfxGetApp()->m_pMainWnd,sFileToView);
			
			//TViewer *viewer = new TViewer();
			
			//m_viewer_list.push_back(viewer);


			//m_viewer.CreateViewer(AfxGetApp()->m_pMainWnd,sFileToView);


		}
		else
		{
			MessageBox(_T_LNGSTR(125), NULL, MB_ICONWARNING);
		}

	}
}

void CCoolExplorerView::DefaultViewSelectedFile(LPCTSTR sProgram)
{
    CString sCommand = sProgram;
    if (!sCommand.IsEmpty())
    {
        //get list of files

        CString sMes;
        int iRow = m_Grid.GetFocusedRow();

        if(iRow < 0 || iRow >= m_Grid.GetNumRows())
        {
            MessageBox(_T_LNGSTR(125), NULL, MB_ICONINFORMATION);
            return;
        }

        ListData *pd = (ListData *)m_Grid.GetUserData(iRow);

        if(pd->getName() != RET_NAME && !pd->isDir())
        {
            CString sFileToView = m_listFiles.GetList()->GetFileFullPath(pd);
            if(m_listFiles.GetList()->GetMode() == Ftp)
            {
                CMyFtpFile file(m_listFiles.GetFtpConnection());
                CString sTmp;
                if(!file.GetFileToTemp(sFileToView, sTmp,pd->getSize()))
                {
                    MessageBox(_T_LNGSTR(244), NULL, MB_ICONWARNING);//"Error reading Ftp file"
                }
                m_listFiles.CheckAndReconnect();
                sFileToView = sTmp;
            }

            sCommand += " \"";
            sCommand += sFileToView;
            sCommand += "\"";


        }
        else
        {
            MessageBox(_T_LNGSTR(125), NULL, MB_ICONWARNING);
        }

    }
}

void CCoolExplorerView::OnEditFile()
{
	if(m_listFiles.IsCurJobStillActive())
		return;

	CSettings cfg;
	CString sEditor;
	CStringList lNames;

	sEditor = cfg.m_settings.strEditor;
	OpenSelectedFile(sEditor);
}

void CCoolExplorerView::OnCreateFile()
{
	if(m_listFiles.IsCurJobStillActive())
		return;
	if(m_listFiles.IsPureVirtualList())
		return;
	if(!m_listFiles.GetList()->AreFilesOperationsAvailable())
		return;
	BOOL bStatus = TRUE;
	//TCHAR buff[_MAX_PATH];
	CString sCommand;
	CSettings cfg;
	sCommand = cfg.m_settings.strEditor;
	if(sCommand.IsEmpty())
	{
		MessageBox(_T_LNGSTR(119), NULL, MB_ICONERROR);
		//		((CMainFrame *)GetParent()->GetParent())->OnConf();
		sCommand = cfg.m_settings.strEditor;
	}
	sCommand += " ";

	CString sMes;
	CString filename;

	int iRow = m_Grid.GetFocusedRow();

	if(iRow > 0 || iRow <= m_Grid.GetNumRows())
	{
		ListData *pd = (ListData *)m_Grid.GetUserData(iRow);

		if(pd->getName() != RET_NAME && !pd->isDir())
		{
			filename = pd->getName();
		}
	}

	CSimpleEdit dlg(_T_LNGSTR(120),filename, GetParentFrame());

	if(dlg.DoModal() == IDOK)
	{
		CString sFile = m_listFiles.GetList()->GetFileFullPath(dlg.GetText());
		HANDLE hFile = CreateFile(sFile, 0, 0, 0, OPEN_EXISTING,0,NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
			DWORD d = GetLastError();
			CString mess ;
			mess.Format(_T_LNGSTR(121),sFile);
			if(MessageBox(mess,NULL, MB_YESNO | MB_ICONQUESTION) != IDYES)
				return;
		}

		CFile file;
		if(!file.Open(sFile, CFile::modeCreate))
		{
			MessageBox(_T_LNGSTR(122));
			return;
		}
		file.Close();

		sCommand += '"' + sFile + '"';

		if(TRUE == bStatus)
#ifndef UNICODE
			WinExec(sCommand, SW_SHOWMAXIMIZED);
#else
		{
			USES_CONVERSION;
			WinExec(W2A(sCommand), SW_SHOWMAXIMIZED);
		}
#endif

		//refresh directory
		m_pFilesListObject->SetDir(m_listFiles.GetDir());
	}

}

BOOL CCoolExplorerView::GetMarkedItems(LISTDataPointers &listPointers)
{
	CIntArray arInt;
	this->m_Grid.GetMarkedItems(arInt);

	for(int i = 0; i<arInt.GetSize(); i++)
	{
		ListData *pd = m_Grid.GetUserData(arInt.GetAt(i));
		if(pd)
		{
			if(pd->getName() != RET_NAME)
			{
				listPointers.AddTail(pd);
			}
		}
	}

	return listPointers.GetCount() > 0;
}


BOOL CCoolExplorerView::GetMarkedNames(CStringList &listNames, CString *psMes/*= NULL*/,BOOL bFilesOnly/*= FALSE*/, BOOL bFullPath/*= FALSE*/, int *pnCountFiles/*= NULL*/, int *pnCountDirs/*= NULL*/)
{
	CIntArray arInt;
	this->m_Grid.GetMarkedItems(arInt);

	CString sDir;// = m_listFiles.GetDir();
	int nCountDir = 0;
	int nCountFiles = 0;
	BOOL bAdd = FALSE;

	for(int i = 0; i<arInt.GetSize(); i++)
	{
		bAdd = FALSE;
		ListData *pd = (ListData *)m_Grid.GetUserData(arInt.GetAt(i));
		if(pd)
		{
			if(pd->getName() != RET_NAME)
			{
				if(!pd->isDir())
				{
					nCountFiles++;
					bAdd = TRUE;
					
				}			
				else if(!bFilesOnly)
				{
					nCountDir++;
					bAdd = TRUE;					
				}
				
				if(bAdd)
				{
					if(bFullPath)
						listNames.AddTail(m_listFiles.GetList()->GetFileFullPath(pd));
					else
						listNames.AddTail(pd->getName());
				}
			}
		
		}
		else
		{
			//
		}
	}

	if(psMes)
	{
		if(nCountDir + nCountFiles > 1)
		{
			psMes->Format(_T("%d directiories and %d files"), nCountDir, nCountFiles);
		}
		else if(listNames.GetCount() == 1)
			*psMes = listNames.GetHead();
	}
	if(pnCountFiles)
		*pnCountFiles = nCountFiles;
	if(pnCountDirs)
		*pnCountDirs = nCountDir;
	return nCountFiles + nCountDir > 0;
}
void CCoolExplorerView::GetNamesCount(int &nCountMarkedFiles,int &nCountUnmarkedFiles,int &nCountMarkedDir,int &nCountUnmarkedDir)
{


	for (int i = 0; i < m_Grid.GetNumRows(); i++)
	{
		ListData *pd = (ListData *)m_Grid.GetUserData(i);

		if (pd->getName() == "..")
			continue;

		if (m_Grid.IsMarkedRow(i))
		{
			if (pd->getAttrib() & FILE_ATTRIBUTE_DIRECTORY)
			{
				nCountMarkedDir++;
			}
			else
			{
				nCountMarkedFiles++;
			}
		}
		else
		{
			if (pd->getAttrib() & FILE_ATTRIBUTE_DIRECTORY)
			{
				nCountUnmarkedDir++;
			}
			else
			{
				nCountUnmarkedFiles++;
			}
		}

	}

}

CCoolExplorerView * CCoolExplorerView::GetAnotherView()
{
	CSplitterWnd *pWnd = (CSplitterWnd *)GetParent()->GetParent();
	if(pWnd)
	{
		if(pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
		{
			CFrameWnd *p0 = (CFrameWnd *)pWnd->GetPane(0,0);
			CFrameWnd *p1 = (CFrameWnd *)pWnd->GetPane(0,1);
			if(p0->GetActiveView() != this)
				return (CCoolExplorerView *)p0->GetActiveView();
			if(p1->GetActiveView() != this)
				return (CCoolExplorerView *)p1->GetActiveView();

		}
		else
		{
			ASSERT(0);
			CoolTrace(0, TRUE, _T("ERROR GetAnotherView parent is NOT CSplitterWnd"));
		}
	}
	return NULL;
}


void CCoolExplorerView::RefreshCommandBarPath()
{
		CMainFrame *main_frame = GetMainFrame();

		if (main_frame != NULL)
		{
			//create string which will be inserted there
			CString strPath = m_listFiles.GetDir();
			
			main_frame->SetCommandBarPath(strPath);
		}
}

BOOL CCoolExplorerView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_SYSCHAR && pMsg->hwnd == m_Grid.GetSafeHwnd())
	{
		if (isalpha((int)pMsg->wParam))
		{
			BOOL b_runfast = false;
			switch(CSettings::m_settings.lSearchShortcutStyle)
			{
			case 1: //alt + letter
				{
					if ((!(GetKeyState(VK_CONTROL) & 0x800)))
					{
						b_runfast = true;
					}
				}
				break;

			default:
				break;
			}
			if (b_runfast)
			{
				m_wndFastFileSrchDlg.ShowWindow(SW_SHOW);
				m_wndFastFileSrchDlg.PutLetter((TCHAR)pMsg->wParam);
				return TRUE;
			}

		}

	}
	
	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd == m_Grid.GetSafeHwnd())
	{
		if (isalpha((int)pMsg->wParam))
		{
			BOOL b_runfast = false;
			switch(CSettings::m_settings.lSearchShortcutStyle)
			{
			case 2: //ctrl + alt + letter
				{
					if (((GetKeyState(VK_MENU) & 0x800))&&
						((GetKeyState(VK_CONTROL) & 0x800)))
					{
						b_runfast = true;
					}
				}
				break;
			case 3: //letters
				if ((!(GetKeyState(VK_MENU) & 0x800))&&
					(!(GetKeyState(VK_CONTROL) & 0x800)))
				{
					b_runfast = true;
				}
			    break;
			default:
			    break;
			}
			if (b_runfast)
			{
				m_wndFastFileSrchDlg.ShowWindow(SW_SHOW);
				m_wndFastFileSrchDlg.PutLetter((TCHAR)pMsg->wParam);
				return TRUE;
			}
			
		}


		switch(pMsg->wParam)
		{
			case VK_ESCAPE:
			{
				if(m_listFiles.BreakCurJob())
					return TRUE;
			}
			break;

			case 220:// '\'
			{
				if((GetKeyState(VK_CONTROL) & 0x800))
				{
					OnGotomain();
					return TRUE;
				}
			}
			break;
		case 188:// '<'
			{
				if((GetKeyState(VK_CONTROL) & 0x800))
				{
					OnHistPrev();
					return TRUE;
				}
			}
			break;
		case 190:// '>'
			{
				if((GetKeyState(VK_CONTROL) & 0x800))
				{
					OnHistNext();
					return TRUE;
				}
			}
			break;
		}
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd == m_Grid.GetSafeHwnd())
	{
		switch(pMsg->wParam)
		{
		case VK_ADD:
			{
				CString Name;
				Name = _T_LNGSTR(123);
				CMarkMaskDlg dlg(Name,this);

				if (dlg.DoModal() == IDOK)
				{
					CString strMask = dlg.m_strMask;
					m_Grid.MarkMaskedRows(dlg.m_strMask,TRUE);
				}
				return TRUE;
			}
			break;
		case VK_SUBTRACT:
			{
				CString Name;
				Name = _T_LNGSTR(124);
				CMarkMaskDlg dlg(Name,this);

				if (dlg.DoModal() == IDOK)
				{
					CString strMask = dlg.m_strMask;
					m_Grid.MarkMaskedRows(dlg.m_strMask,FALSE);
				}
				return TRUE;
			}
			break;
		case VK_TAB:
			{
				if ((GetKeyState(VK_CONTROL) & 0x800))
				{
						CCoolExplorerFrame *pFrame = (CCoolExplorerFrame*)GetParentFrame();

						if(pFrame)
						{
							pFrame->m_wndPathBar.MarkComboPath();

						}

				}
				else
				{
					ActivateNextView();
				}

			}
			return TRUE;
			break;
		case VK_F3:
			OnViewFile();
			return TRUE;
			break;
		case VK_F4:
			if ((GetKeyState(VK_SHIFT) & 0x800))
			{
				OnCreateFile();
			}
			else
			{
				OnEditFile();
			}

			
			return TRUE;
			break;
		case VK_F5:
			if ((GetKeyState(VK_CONTROL) & 0x800))
				OnCopyMoveFile(FALSE, TRUE);
			else
				OnCopyFile();
			return TRUE;			
			break;
		case VK_F6:
			// rename or move file
			if ((GetKeyState(VK_SHIFT) & 0x800))
			{
				RenameItem(0,0);
			}
			else if((GetKeyState(VK_CONTROL) & 0x800))
			{
				OnCopyMoveFile(TRUE, TRUE);
			}
			else
			{
				OnMoveFile();
			}
			return TRUE;
			break;
		case VK_F7:
			if ((GetKeyState(VK_RMENU) & 0x800))
			{
				OnAppFindDialog();
			}
			else
			{
				OnCreateDir();
			}
			return TRUE;
			break;
		case VK_F9:
			OnChangeAttrib();
			return TRUE;
		case VK_F11:
			OnFtpDisconect();
			return TRUE;
			break;
		case VK_F12:
			OnFtpConnect();
			return TRUE;
			break;
		case VK_F8:
		case VK_DELETE:
			DeleteFile(m_listFiles.IsPureVirtualList() && !(GetKeyState(VK_CONTROL) & 0x800), (GetKeyState(VK_CONTROL) & 0x800));
			return TRUE;
			break;
		case VK_LEFT:
			if ((GetKeyState(VK_CONTROL) & 0x800))
			{
				if (m_strViewName == _T("CoolExpl_Right"))
				{
					ListData *pd = (ListData *)m_Grid.GetUserData(m_Grid.GetFocusedRow());
					
					if  ( pd )
					{
						CString strPath = _T("");

						if (pd->getName() == _T(".."))
						{
							strPath = m_listFiles.GetDir();
						}
						else
						{
							if(pd->getAttrib() & FILE_ATTRIBUTE_DIRECTORY)
							{
								strPath = m_listFiles.GetList()->GetFileFullPath(pd);;
							}
							else
							{
								strPath = m_listFiles.GetDir();
							}
						}

						if (!strPath.IsEmpty())
						{
							GetAnotherView()->GetFilesList()->SetDir(strPath);
						}
					}
					
				}
				else if (m_strViewName == _T("CoolExpl_Left"))
				{
					CString strPath = GetAnotherView()->m_listFiles.GetDir();
					if (!strPath.IsEmpty())
					{
						m_pFilesListObject->SetDir(strPath);
					}
				}
			}
			break;
		case VK_RIGHT:
			if ((GetKeyState(VK_CONTROL) & 0x800))
			{
				if (m_strViewName == _T("CoolExpl_Left"))
				{
					
					ListData *pd = (ListData *)m_Grid.GetUserData(m_Grid.GetFocusedRow());
					
					if  ( pd )
					{
						CString strPath = _T("");

						if (pd->getName() == _T(".."))
						{
							strPath = m_listFiles.GetDir();
						}
						else
						{
							if(pd->getAttrib() & FILE_ATTRIBUTE_DIRECTORY)
							{
								strPath = m_listFiles.GetList()->GetFileFullPath(pd);
							}
							else
							{
								strPath = m_listFiles.GetDir();
							}
						}

						if (!strPath.IsEmpty())
						{
							GetAnotherView()->GetFilesList()->SetDir(strPath);
						}
					}
				}
				else if (m_strViewName == _T("CoolExpl_Right"))
				{
					CString strPath = GetAnotherView()->m_listFiles.GetDir();
					
					if (!strPath.IsEmpty())
					{
						//DisplayFiles(strPath, NULL);
					}
				}

			}
			break;
		case VK_BACK:
			{
				const ListData * pd = m_listFiles.GetList()->SearchInCurrentDir(RET_NAME);
				if(pd)
				{
					m_pFilesListObject->Select(pd, m_nDataVersion);
					return TRUE;
				}
			}
			break;
		case 'C':			
			if(GetAsyncKeyState(VK_CONTROL) < 0)// check if ctrl is really pressed on the keyboard
			{
				OnEditCopy();
				return TRUE;
			}
			break;
		case 'X':
			if(GetKeyState(VK_CONTROL) < 0)
			{
				OnEditCut();
				return TRUE;
			}
			break;
		case 'L':
			if(GetKeyState(VK_CONTROL) & 0x800)
			{
				OnAppChanegeVolLabel();
				return TRUE;
			}
			break;
		case 'V':
			if((GetKeyState(VK_CONTROL) & 0x800) && !(GetKeyState(VK_SHIFT) & 0x800))
			{
				OnEditPaste();
				return TRUE;
			}
			break;
		case 'T':
			if((GetKeyState(VK_CONTROL) & 0x800)&&(GetKeyState(VK_MENU) & 0x800))
			{
				OnShowThesameFiles();
				return TRUE;
			}
			break;
		case 'D':
			if((GetKeyState(VK_CONTROL) & 0x800)&&(GetKeyState(VK_MENU) & 0x800))
			{
				OnShowDifferentFiles();
				return TRUE;
			}
			else if (GetKeyState(VK_CONTROL) & 0x800)
			{
				CCoolExplorerFrame *pFrame = (CCoolExplorerFrame*)GetParentFrame();

				if(pFrame)
				{
					pFrame->m_wndPathBar.OnFavorities();
					return TRUE;
				}
				
			}
			break;
		case 'F':
			if(GetKeyState(VK_CONTROL) & 0x800)
			{
				SetFilters();
				return TRUE;
			}
			break;
		case 'H':
			{
				if((GetKeyState(VK_CONTROL) & 0x800)&&
					(GetKeyState(VK_MENU) & 0x800))
				{
					//					m.TrackPopupMenu(TPM_CENTERALIGN,0,0,this,NULL);
					
					
					return TRUE;
				}
			}
			break;		
		case VK_ESCAPE:
			{
				CMainFrame *main_frame = GetMainFrame();
				
				if (main_frame != NULL)
				{
					//create commandbar which will be inserted there
					main_frame->EraseCommandBarText();
				}
				
			}
			break;
		case VK_SPACE:
			{
				if((GetKeyState(VK_LSHIFT) & 0x800)&&(GetKeyState(VK_LCONTROL) & 0x800))
				{
					m_listFiles.CountDirsSize(NULL);
				}
				else
				{
					m_listFiles.CountDirsSize(m_Grid.GetUserData(m_Grid.GetFocusedRow()));
				}
			}
			break;
			
		}
		
		if ((GetKeyState(VK_CONTROL) & 0x800)&&(pMsg->wParam == 'A')) 
		{
			if ((GetKeyState(VK_SHIFT) & 0x800))
			{
				m_Grid.MarkAllRows(FALSE);
				m_Grid.RedrawWindow();
			}
			else
			{
				m_Grid.MarkAllRows(TRUE);
				m_Grid.RedrawWindow();
			}
			return TRUE;
		}
		/*if ((GetKeyState(VK_CONTROL) & 0x800)&&(pMsg->wParam == 'C')) 
		{
			int i = 0;
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x800)&&(pMsg->wParam == 'V')) 
		{
			int i = 0;
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x800)&&(pMsg->wParam == 'X')) 
		{
			int i = 0;
			return TRUE;
		}*/
		if ((GetKeyState(VK_CONTROL) & 0x800)&&(pMsg->wParam == 'R')) 
		{
			OnRefresh(0,0);
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x800)&&(pMsg->wParam == 'T')) 
		{
			AddNewTab();
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x800)&&(pMsg->wParam == 'W')) 
		{
			RemoveActiveTab();
			return TRUE;
		}
		if ((GetKeyState(VK_CONTROL) & 0x800)&&
			(GetKeyState(VK_SHIFT) & 0x800)&&(pMsg->wParam == VK_RETURN)) 
		{
			
			int iRow = m_Grid.GetFocusedRow();
			ListData *pd = (ListData *)m_Grid.GetUserData(iRow);
			
			if ((pd->getName() != RET_NAME))
			{
				CString sFileToView = m_listFiles.GetList()->GetFileFullPath(pd);
				
				CMainFrame *main_frame = GetMainFrame();	
				
				if (main_frame != NULL)
				{
					//create string which will be inserted there
					CString strPath = m_listFiles.GetDir();
					main_frame->SetCommandBarText(sFileToView);
				}
				
			}
			
			return TRUE;
		}
		
		if ((GetKeyState(VK_CONTROL) & 0x800)&&(pMsg->wParam == VK_RETURN)) 
		{
			int iRow = m_Grid.GetFocusedRow();
			ListData *pd = (ListData *)m_Grid.GetUserData(iRow);
			
			if ((pd->getName() != _T("..")))
			{
				const CString &sFileToView = pd->getName();
				
				CMainFrame *main_frame = GetMainFrame();
				
				if (main_frame != NULL)
				{
					//create string which will be inserted there
					main_frame->SetCommandBarText(sFileToView);
					
				}
				
			}
			//	OnRefresh(0,0);
			//			AfxMessageBox(m_listFiles);
			return TRUE;
		}
		

		if((int)pMsg->wParam == 191)// VK_DIVIDE does not work??
		{
			//show fast file search window create child window
			if (CSettings::m_settings.lSearchShortcutStyle == 4) //check settings
			{
				m_wndFastFileSrchDlg.ShowWindow(SW_SHOW);	
				return TRUE;
			}
		}


	}
	/*if (pMsg->message == 1101)
	{
		SetTimer(1, 1000, NULL);
	}*/
	return CView::PreTranslateMessage(pMsg);
}

void CCoolExplorerView::ActivateNextView()
{
	GetAnotherView()->SetFocus();
}

void CCoolExplorerView::OnSetFocus(CWnd* pOldWnd) 
{
	CFrameWnd *pFrame = GetParentFrame();
	if(pFrame)
		pFrame->SetActiveView(this);//ccoolexplorerframe
	if(pFrame = pFrame->GetParentFrame())
		pFrame->SetActiveView(this);//CMainFrame*/

	CCoolExplorerView *pView = GetAnotherView();
	if(pView)
		GetMainFrame()->UnRegisterObservers(pView->GetFilesList());
	GetMainFrame()->RegisterObservers(&m_listFiles);

	PostMessage(WM_POST_SETFOCUS);

}

void CCoolExplorerView::OnKillFocus(CWnd* pNewWnd)
{
//	GetMainFrame()->UnRegisterObservers(&m_listFiles);
//	__super::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
}

void CCoolExplorerView::OnFtpConnect() 
{
	if(m_listFiles.IsCurJobStillActive())
		return;

	if(m_listFiles.GetList()->GetMode() == Ftp)
	{
		if(IDOK == MessageBox(_T("Disconnect current FTPsession?"), NULL, MB_OKCANCEL))
			m_listFiles.FtpDisconect();
		else
			return;
	}

//	byte a[1024];
	CConnectFtpDlg dlg(this);
//	byte b[1024];
	BOOL bContinue = TRUE;
	while(bContinue && dlg.DoModal() == IDOK)
	{
		CFtpConSet fcs;
		fcs = dlg.GetSettings();
		if(!m_listFiles.InitInternetConection(&fcs, &((CCoolExplorerFrame *)GetParentFrame())->m_wndFtpStatusBar))
		{
			TRACE(_T("Error init Ftp %s, %s\n"), fcs.m_sServer, fcs.m_sUser);
			//DisplayFiles(m_sBeforeFtp);
			bContinue = TRUE;
		}
		else
		{
			//DisplayFiles(NULL);
			bContinue = FALSE;
		}		
	}

	if(m_listFiles.GetList()->GetMode() != Ftp)
	{
		((CCoolExplorerFrame*)GetParentFrame())->m_wndFtpStatusBar.ShowWindow(SW_HIDE);
		GetParentFrame()->RecalcLayout();
	}
}

void CCoolExplorerView::OnFtpDisconect() 
{
//	if(m_listFiles.GetList()->GetMode() != Ftp)
//		return;
	if(m_listFiles.IsCurJobStillActive())
		return;

	if(IsSharedFtpSession())
		return;

//	if(m_sBeforeFtp.IsEmpty())
//	{
//		MessageBox(_T("You are not connected to Ftp in this panel"));
//		return;
//	}

	m_listFiles.FtpDisconect();
	//m_sPath = m_sBeforeFtp
//	DisplayFiles(m_sBeforeFtp);
//	m_sBeforeFtp = _T("");	
}

void CCoolExplorerView::OnFtpLogin() 
{
	if(IsSharedFtpSession())
		return;

	if(m_listFiles.GetList()->GetMode() == Ftp)
	{
		if(IDOK == MessageBox(_T("Disconnect current FTPsession?"), NULL, MB_OKCANCEL))
			m_listFiles.FtpDisconect();
		else
			return;
	}


	CLoginFtpDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
//		m_sBeforeFtp = m_listFiles.GetDirForParsing();
		CFtpConSet fcs;
		fcs.m_sServer = dlg.m_sFtpAddress;
		fcs.m_sUser = dlg.m_sFtpUser;
		fcs.m_sPassword = dlg.m_sFtpPassword;
		if(!m_listFiles.InitInternetConection(&fcs, &((CCoolExplorerFrame *)GetParentFrame())->m_wndFtpStatusBar))
		{
			MessageBox(_T("Error init Ftp"));
			//DisplayFiles(m_sBeforeFtp);
		}
		else
		{
			DisplayFiles(NULL);
		}
	}

	if(m_listFiles.GetList()->GetMode() != Ftp)
	{
//		if(bRead)
//			DisplayFiles(m_sBeforeFtp);
//		m_sBeforeFtp = _T("");
		((CCoolExplorerFrame*)GetParentFrame())->m_wndFtpStatusBar.ShowWindow(SW_HIDE);
		GetParentFrame()->RecalcLayout();
	}	
}



BOOL CCoolExplorerView::DestroyWindow() 
{		

	return CView::DestroyWindow();
}


void CCoolExplorerView::OnChangeAttrib() 
{
	if(m_listFiles.IsCurJobStillActive())
		return;
	if(m_listFiles.GetList()->GetMode() == Ftp)
	{
		MessageBox(_T("Cannot change attributes in Ftp"));
		return;
	}


	int nItem = -1;
	CStringList lNames;
	
	GetMarkedNames(lNames);
	
	if(lNames.IsEmpty())
	{
		MessageBox(_T("No files selected"));
		return;
	}
 	CChangeAttribDlg dlg(m_listFiles.GetDir(), &lNames);
	if(dlg.DoModal()==IDOK)
	{
		PostMessage(WM_REFRESH);
		if(!m_listFiles.GetDir().CompareNoCase(GetAnotherView()->GetFilesList()->GetDir()))
			GetAnotherView()->PostMessage(WM_REFRESH);
	}
}

BOOL CCoolExplorerView::GoToSimilarItem(int i, CString &sSelectItem)
{
	if (!IsWindow(m_Grid.GetSafeHwnd()))
		return FALSE;
    LPCTSTR strItem = _T("");
    int iCountedItems = 0;
    //add mask letter tos SelectItem - *
    if (m_wndFastFileSrchDlg.m_FullMaskedCheckbox.IsWindowVisible())
    {
        if (m_wndFastFileSrchDlg.m_FullMaskedCheckbox.GetState())
        {
			if (sSelectItem != _T("*"))
				 sSelectItem = _T("*") + sSelectItem + _T("*");
        }
        else
        {
			if (sSelectItem != _T("*"))
				sSelectItem += _T("*");
        }
        
    }
    else
    {
        if (CSettings::m_settings.bFFSerchMask == TRUE)    
        {
			if (sSelectItem != _T("*"))
				sSelectItem += _T("*");
        }
        else
        {
			if (sSelectItem != _T("*"))
			    sSelectItem = _T("*") + sSelectItem + _T("*");
        }
    }
    //count items
/*	for(int nItem = 0; nItem < m_Grid.GetItemCount(); nItem++)
	{
       m_Grid.SetItemState(nItem, ~LVNI_SELECTED, LVNI_SELECTED );
    }*/
	CorrectMask(sSelectItem);
    for(int nItem = 0; nItem < m_Grid.GetItemCount(); nItem++)
	{
        strItem = ((ListData *)m_Grid.GetUserData(nItem))->getName();
		//if ( _tcscmp(strItem, RET_NAME) )
        {
            if ( CompareOne( sSelectItem, strItem ))
            {
				if((_tcschr(sSelectItem,'.')==NULL) || (_tcschr(strItem,'.')!=NULL))
				{
					iCountedItems ++;
					
					if ( iCountedItems == i)
					{
						
						m_Grid.SelectRow(strItem,0,TRUE);
						m_Grid.RedrawWindow();
						return TRUE;
					}
				}
            }
        }
    }
    return FALSE;
}

int CCoolExplorerView::CountSimilarItems( CString sSelectItem )
{
//	if(m_listFiles == NULL )
	if (!IsWindow(m_Grid.GetSafeHwnd()))
		return FALSE;
    LPCTSTR strItem = _T("");
    int iCountedItems = 0;
    //add mask letter tos SelectItem - *
    if (m_wndFastFileSrchDlg.m_FullMaskedCheckbox.IsWindowVisible())
    {
        if (m_wndFastFileSrchDlg.m_FullMaskedCheckbox.GetState())
        {
			if (sSelectItem != _T("*"))
				sSelectItem = _T("*") + sSelectItem + _T("*");
        }
        else
        {
			if (sSelectItem != _T("*"))
				sSelectItem += _T("*");
        }

    }
    else
    {
        if (CSettings::m_settings.bFFSerchMask == TRUE)    
        {
            sSelectItem += _T("*");
        }
        else
        {
            sSelectItem = _T("*") + sSelectItem + _T("*");
        }
    }

	CorrectMask(sSelectItem);
    //count items
	for(int nItem = 0; nItem < m_Grid.GetItemCount(); nItem++)
	{
		
        strItem = ((ListData *)m_Grid.GetUserData(nItem))->getName();
		//if ( _tcscmp(strItem, RET_NAME) )
        {
            if ( CompareOne( sSelectItem, strItem ))
            {
				if((_tcschr(sSelectItem,'.')==NULL) || (_tcschr(strItem,'.')!=NULL))
					iCountedItems ++;
            }
        }
    }
    return iCountedItems;
}

void CCoolExplorerView::OnCopyItems() 
{
/*  COleDataObject DataObject;
  if( DataObject.AttachClipboard() )
  {
    DataObjectToList(&DataObject);
  } */
}
void CCoolExplorerView::OnPasteItems() 
{
}

void CCoolExplorerView::OnCutItems() 
{
}
void DataObjectToList(COleDataObject DataObject)
{

}

LPCTSTR CCoolExplorerView::StringToBuf(LPCTSTR string)
{
	size_t nBufLen = _tcslen(string) + 2;
	LPTSTR szBuf = new TCHAR[nBufLen];
	memset(szBuf, 0, nBufLen * sizeof(TCHAR));
	lstrcpy(szBuf, string);
	return szBuf;
}

LRESULT CCoolExplorerView::OnSearchWindowFinished(WPARAM wparam, LPARAM lparam)
{
	sSearchWinFin *sr = ((sSearchWinFin*)wparam);

	switch (sr->nFinishType)
	{
	case 0:
		break;
	case 1:
		m_listFiles.SetDir(sr->strGoToPath);
		m_sPrevItemToSelect = m_wndMainSrchDlg.GetFileName();
		break;
	case 2:
		m_listFiles.InitVirtualList(&m_wndMainSrchDlg);
		break;

	}
	//free window resources
	m_wndMainSrchDlg.m_finder.Clear();
	return 0;
}

LRESULT CCoolExplorerView::RefreshDriveCapacityFrame(WPARAM, LPARAM)
{
	CString str = _T("");
	if(m_listFiles.GetList()->GetMode() == Ftp)
	{
		const CFtpConSet *ftpset = m_listFiles.GetFtpSet();
		//str.Format(_T("FTP server:%s: Port:%d User:%s Passive mode:%s\t Ftp"),
		//	         ftpset->m_sServer, ftpset->m_nPort, ftpset->m_sUser, ftpset->m_bPassive?_T("On"):_T("Off"));
		str.Format(_T_LNGSTR(204), ftpset->m_sServer, ftpset->m_nPort, ftpset->m_sUser, ftpset->m_bPassive?_T_LNGSTR(205):_T_LNGSTR(206));		
	}
	else
	{
		ULARGE_INTEGER TotalNumberOfBytes,TotalNumberOfFreeBytes,FreeBytesAvailableToCaller;
		TotalNumberOfBytes.QuadPart = 0;
		TotalNumberOfFreeBytes.QuadPart = 0;
		FreeBytesAvailableToCaller.QuadPart = 0;

		CSettings cfg;
		eTypeOfSize ebytes = eMBytes;

		if (cfg.m_settings.lDisplaySizeIn == 0)
		{
			ebytes = eKBytes;
		}

		

		BOOL bValOk = GetDiskFreeSpaceEx( m_listFiles.GetDir(),&FreeBytesAvailableToCaller	,
			&TotalNumberOfBytes,&TotalNumberOfFreeBytes);		
		
		
		//if (bValOk)
		{
			UINT nDrive = GetDriveType(m_listFiles.GetDir());
			CString sDriveType;
			BOOL bCheckLabel = FALSE;

			//
			switch(nDrive)
			{
			case DRIVE_REMOVABLE:
				bCheckLabel = TRUE;
				sDriveType = _T_LNGSTR(104);
				break;
			case DRIVE_FIXED:
				bCheckLabel = TRUE;
				sDriveType = _T_LNGSTR(105);
				break;
			case DRIVE_REMOTE:
				sDriveType = _T_LNGSTR(106);
				break;
			case DRIVE_CDROM:
				bCheckLabel = TRUE;
				sDriveType = _T_LNGSTR(107);
				break;
			case DRIVE_RAMDISK:
				sDriveType = _T_LNGSTR(108);
				break;
			#ifdef _DEBUG
			case DRIVE_UNKNOWN:
				sDriveType = _T_LNGSTR(109);
				break;
			case DRIVE_NO_ROOT_DIR:
				sDriveType = _T_LNGSTR(110);
				break;
			default:
				ASSERT(0);
			#endif
			}

			TCHAR lpVolumeNameBuffer[1024]={0};
			DWORD nVolumeNameSize = sizeof(lpVolumeNameBuffer)/sizeof(TCHAR) -1;
			DWORD lpVolumeSerialNumber;
			DWORD lpMaximumComponentLength;
			DWORD lpFileSystemFlags;
			TCHAR lpFileSystemNameBuffer[1024]={0};
			DWORD nFileSystemNameSize = sizeof(lpFileSystemNameBuffer)/sizeof(TCHAR) -1;
			if (GetVolumeInformation(m_listFiles.GetList()->GetDrivePath(),
				lpVolumeNameBuffer,
				nVolumeNameSize,
				&lpVolumeSerialNumber,
				&lpMaximumComponentLength,
				&lpFileSystemFlags,
				lpFileSystemNameBuffer,
				nFileSystemNameSize
				) == 0)
			{

			}
			//
			if(sDriveType.GetLength() || _tcslen(lpVolumeNameBuffer) )
			{
				CString sLabel;
				if (bCheckLabel)
				{
					if(_tcslen(lpVolumeNameBuffer))
						sLabel.Format(_T("%c%s%c"),'[',lpVolumeNameBuffer,']');

				}
				else
				{
					UNIVERSAL_NAME_INFO *info =(UNIVERSAL_NAME_INFO *) new TCHAR[1024];
					DWORD l=1024;
					DWORD e = WNetGetUniversalName( m_listFiles.GetList()->GetDrivePath(), UNIVERSAL_NAME_INFO_LEVEL, info,&l);
					if(e == NO_ERROR)
					{
						sLabel.Format(_T("( %s )"), info->lpUniversalName);
					}
					delete [] info;
					
				}
			
				str.Format(_T_LNGSTR(113),sDriveType, m_listFiles.GetList()->GetDrive(),sLabel,FormatSize(TotalNumberOfBytes.QuadPart,_T(" "),ebytes)  ,FormatSize(TotalNumberOfFreeBytes.QuadPart,_T(" "),ebytes)  );
				switch(m_listFiles.GetList()->GetMode())
				{
				case Ftp:
					str += _T(" Ftp");
					break;
					case COM:
					str += _T("\tCOM");
					break;
					case Normal:
					str += _T("\tNormal");
					break;
					case Net:
					str += _T("\tNet");
					break;
					case Virtual:
					str += _T("\tVirtual");
					break;					
					default:
					ASSERT(0);
					break;

				}
			}
		}
	}

	if (((CCoolExplorerFrame*)GetParentFrame()))
	{
		((CCoolExplorerFrame*)GetParentFrame())->m_wndUpStatusBar.SetPaneText(0,str,TRUE);
	}
	return 1L;
}

LRESULT CCoolExplorerView::OnSetPMenu(WPARAM wparam, LPARAM pMenu)
{
	m_pContextMenu = (IContextMenu2 *)pMenu;	
	return 1;
}

LRESULT CCoolExplorerView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	//TRACE(_T("DefWindowProc %d\n"), message);
	return CView::DefWindowProc(message, wParam, lParam);
}

LRESULT CCoolExplorerView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WM_MEASUREITEM
		|| message == WM_INITMENUPOPUP
		|| message == WM_MENUCHAR
		|| message == WM_DRAWITEM)
	{
		if(m_pContextMenu)
		{
			//			try
			{
				/*CString str;
				if(message == WM_MEASUREITEM)
					str = "WM_MEASUREITEM";
				else if(message == WM_INITMENUPOPUP)
					str = "WM_INITMENUPOPUP";
				else if(message == WM_MENUCHAR)
					str = "WM_MENUCHAR";
				else if(message == WM_DRAWITEM)
					str = "WM_DRAWITEM";
				
				CString sTmp;
				sTmp.Format(" %d %d", wParam, lParam);
				str += sTmp + "\n";
				TRACE(str);
				*/
				m_pContextMenu->HandleMenuMsg(message, wParam, lParam);
				return TRUE;
			}
			//			catch(...)
			{
				//MessageBox("a");
			}
		}
	}
	
	return CView::WindowProc(message, wParam, lParam);
}

void CCoolExplorerView::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CView::PostNcDestroy();
}

DROPEFFECT CCoolExplorerView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	DROPEFFECT ef=DROPEFFECT_COPY;
	//ASSERT(NULL == m_pDragTarget);
	if(m_pDragTarget)
	{
		delete m_pDragTarget;
		m_pDragTarget = NULL;
	}
	m_Grid.ScreenToClient(&point);
	m_nDropTargetItem = m_Grid.GetMouseSelectedRow(point);
	if(m_nDropTargetItem<0) m_nDropTargetItem = -1;
	TRACE(_T("CCoolExplorerView::OnDragEnter sel:%d\n"), m_nDropTargetItem);
	BOOL bSubFolder = FALSE;

	{
		LISTDataPointers list;
		m_listFiles.GetList()->LockList();
		if(m_nDropTargetItem >= 0)
		{
			ListData *pd = m_Grid.GetUserData(m_nDropTargetItem);
			
			if(pd->isDir() && pd->getName() != RET_NAME)
			{
				TRACE(_T("CCoolExplorerView::OnDragEnter target name:%s\n"), pd->getName());
				list.AddHead(m_Grid.GetUserData(m_nDropTargetItem));
				bSubFolder = TRUE;
			}
			else
			{
				m_nDropTargetItem = -1;
			}
		}
		m_pDragTarget = m_listFiles.GetList()->SaveItems(list);
		m_listFiles.GetList()->UnlockList();
	}

	if(m_nDraggedItem == -1 || (m_nDraggedItem != m_nDropTargetItem && bSubFolder))
		m_dropTarget.SetITarget(m_pDragTarget?m_pDragTarget->createDropTarget():NULL);
	else
		m_dropTarget.SetITarget(NULL);

	if(m_nDropTargetItem >= 0 && bSubFolder)
		m_Grid.FocusRow(m_nDropTargetItem);
	else
		m_Grid.FocusRow(0);

	return ef;
}

DROPEFFECT CCoolExplorerView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	DROPEFFECT ret = DROPEFFECT_COPY;
	//ASSERT(m_pDragTarget);
	if(!m_pDragTarget)
		return ret;

	//TRACE(_T("Drop %d\n"), ret);

	CPoint pt = point;
	m_Grid.ScreenToClient(&pt);
	int nItem = m_Grid.GetMouseSelectedRow(pt);
	if(nItem<0) nItem = -1;
	CString sPath = m_pDragTarget->getDir();

	TRACE(_T("CCoolExplorerView::OnDragOver %d %d\n"), nItem, m_nDropTargetItem);

	if(nItem != m_nDropTargetItem)
	{
		BOOL bSubFolder = FALSE;
		if(nItem >= 0)
		{			
			m_listFiles.GetList()->LockList();
			ListData *pd = m_Grid.GetUserData(nItem);
			bSubFolder = (pd->isDir() && pd->getName() != RET_NAME);
			m_listFiles.GetList()->UnlockList();			
		}

		if(bSubFolder)
		{			
			m_Grid.FocusRow(nItem);//subfolder
			m_Grid.RedrawWindow();
		}
		else
		{
			if(nItem>0)
			{
				m_Grid.FocusRow(nItem);
				m_Grid.RedrawWindow();
			}
			else
			{
				m_Grid.FocusRow(0);
				m_Grid.RedrawWindow();
			}
			nItem = -1;//current folder is target			
		}		
		
		if(nItem != m_nDropTargetItem)
		{
			//it all for change IDropTarget from shell PIDL
			TRACE(_T("CCoolExplorerView::OnDragOver changing target %d %d\n"), nItem, m_nDropTargetItem);
			if(m_dropTarget.GetITarget())
				m_dropTarget.GetITarget()->DragLeave();
			OnDragLeave();
			OnDragEnter(pDataObject, dwKeyState, point);
			ret=DROPEFFECT_NONE;//means target changed
		}
		m_nDropTargetItem = nItem;

	}
	TRACE(_T("CCoolExplorerView::OnDragOver end %d %d\n"), nItem, m_nDropTargetItem);
	return ret;
}

void CCoolExplorerView::OnDragLeave() 
{	
	//ASSERT(m_pDragTarget);
	m_nDropTargetItem = -1;
	if(m_pDragTarget)
	{
		delete m_pDragTarget;
		m_pDragTarget = NULL;

	}
}



BOOL CCoolExplorerView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	ASSERT(m_pDragTarget);
	int nItem = m_Grid.GetMouseSelectedRow(point);

	if(dropEffect != DROPEFFECT_NONE)
		PostMessage(WM_REFRESH);

	if(m_pDragTarget)
	{
		delete m_pDragTarget;
		m_pDragTarget = NULL;
		return TRUE;
	}
	
	return FALSE;	
}

BOOL CCoolExplorerView::OnDropPaste(COleDataObject * pDataObject, DROPEFFECT dropeffect, LPCTSTR sTarget)
{
/*	if(pDataObject->IsDataAvailable(RegisterClipboardFormat(CFSTR_FILENAME)))
	{
		AfxMessageBox(_T(""));
	}

	if(pDataObject->IsDataAvailable(RegisterClipboardFormat(CFSTR_SHELLIDLIST)))
	{
		AfxMessageBox(_T(""));
	}


	if(pDataObject->IsDataAvailable(RegisterClipboardFormat(CFSTR_SHELLIDLISTOFFSET)))
	{
		AfxMessageBox(_T(""));
	}
*/

	if(pDataObject->IsDataAvailable(CF_HDROP))
	{
		if(MessageBox(dropeffect & DROPEFFECT_MOVE ? _T("Move data?") : _T("Copy data?"), NULL, MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
			return FALSE;
		HGLOBAL hdata = pDataObject->GetGlobalData(CF_HDROP);
		LPDROPFILES df = (LPDROPFILES)GlobalLock(hdata);

		LPCTSTR sFiles = (LPCTSTR)((LPCSTR)df + df->pFiles);
		BOOL bDelString = FALSE;
		if(df->fWide)
		{
			sFiles = ConvertUnicodeShellArray((LPCWSTR)sFiles);
			bDelString = TRUE;
		}
				
		SHFILEOPSTRUCT fop = {0};
		fop.hwnd = AfxGetMainWnd()->GetSafeHwnd();
		fop.fFlags = 0;
		fop.pTo = sTarget;
		fop.pFrom = sFiles; 		
		fop.fAnyOperationsAborted = TRUE; 
		fop.hNameMappings = NULL; 
		fop.lpszProgressTitle = _T(""); 
		if(dropeffect & DROPEFFECT_MOVE)
			fop.wFunc = FO_MOVE;
		else
			fop.wFunc = FO_COPY;

		// add "copy of " to name if sSource dir == starget dir
		if(fop.wFunc == FO_COPY)
		{
			CStringList listDest;
			CString sToFile;
			int nSizeAll = 0;
			LPCTSTR sWideChars = sFiles;
			vector<TCHAR> vectDest;
			while(*sWideChars)
			{
				CString sDir,sFile;
				GetDirFile(sDir, sFile, sWideChars);
				if(sDir.Right(1) != '\\')
					sDir += '\\';
				if(sDir == sTarget)
				{
					fop.fFlags = FOF_MULTIDESTFILES;
					sFile = _T("Copy of ") + sFile;				
				}
				listDest.AddTail(AddPathNames(sTarget, sFile));
				
				
				nSizeAll += listDest.GetTail().GetLength() + 1;
				sWideChars += _tcslen(sWideChars) + 1;
			}
			
			if(fop.fFlags == FOF_MULTIDESTFILES )
			{
				nSizeAll += 1;
				LPTSTR sTo = new TCHAR[nSizeAll];
				fop.pTo = sTo; 
				POSITION pos = listDest.GetHeadPosition();
				while(pos)
				{				
					_tcscpy(sTo, listDest.GetNext(pos));
					sTo += _tcslen(sTo) + 1;
				}
				*sTo = 0;
			}
		}
		////
		

		SHFileOperation(&fop);
		if(fop.fFlags == FOF_MULTIDESTFILES )
		{
			delete [] (TCHAR *)fop.pTo;
		}
		GlobalUnlock(hdata);
		//GlobalFree(hdata);
		HGLOBAL hgData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(DROPEFFECT));
		DROPEFFECT *pDropEffect = (DROPEFFECT *)GlobalLock(hgData);
		OpenClipboard();
		if(dropeffect & DROPEFFECT_MOVE)
		{
			*pDropEffect = DROPEFFECT_MOVE;
			EmptyClipboard();
		}
		else
			*pDropEffect = DROPEFFECT_COPY;
		
		GlobalUnlock(hgData);

//		if( ! SetClipboardData( RegisterClipboardFormat(CFSTR_PERFORMEDDROPEFFECT), hgData ) )
//			ErrorMessage();
		CloseClipboard();

		if(bDelString)
		{
#ifndef UNICODE
			delete [] (LPSTR)sFiles;
#endif
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCoolExplorerView::CopyItems(BOOL bDrag, BOOL bCut)
{

	if(m_listFiles.IsCurJobStillActive())
		return FALSE;

	if(!m_listFiles.GetList()->AreFilesOperationsAvailable())
	{
		if(!bDrag)
			AfxMessageBox(_T_LNGSTR(228), MB_ICONERROR);
		return FALSE;
	}

	static BOOL bIn = FALSE;
	int buflen = 0;
	SetItemsState(LVNI_ALL, LVNI_CUT, 0);
	CStringList lNames;
	GetMarkedNames(lNames, NULL, FALSE, TRUE);
	if(bCut)
		SetItemsState(LVNI_SELECTED, LVIS_CUT);
	if(!lNames.GetCount())
		return FALSE;
	//if(!bIn)
	{
	COleDataSource *pSource = new COleDataSource;
	


		POSITION pos = lNames.GetHeadPosition();

		while(pos)
		{
			buflen += lNames.GetNext(pos).GetLength() + 1;
		}
		buflen += 200;
		buflen *= sizeof(TCHAR); // for unicode
//		buflen += 100;

		buflen += sizeof(DROPFILES);
		HGLOBAL hgData=GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT , buflen);
		
		LPTSTR  lpData=(LPTSTR)GlobalLock(hgData);
//		memset(lpData, '\0', buflen);
		DROPFILES *df = (LPDROPFILES) lpData;
		df->pFiles = sizeof(DROPFILES);
		CPoint point;
		GetCursorPos( &point );
		ScreenToClient(&point);
		df->pt = point;
		df->fNC = FALSE;
#ifdef UNICODE
		df->fWide = TRUE;
#else
		df->fWide = FALSE;
#endif
		TCHAR * buf = (LPTSTR)((BYTE *)lpData + df->pFiles);
//		memset(buf, '\0', buflen);
		pos = lNames.GetHeadPosition();
		while(pos)
		{
			//lstrcpy(buf, lNames.GetNext(pos));
			_tcscpy(buf, lNames.GetNext(pos));
			buf += _tcslen(buf) + 1;
		}

		*buf = *(buf+1) = 0;

		GlobalUnlock(hgData);

		STGMEDIUM stgmed;
		
		stgmed.tymed = TYMED_HGLOBAL;
		
		stgmed.hGlobal = hgData;
		
		stgmed.pUnkForRelease = NULL;

		pSource->CacheData(CF_HDROP, &stgmed);

		TRACE(_T("CCoolExplorerView::CopyItems pSource->m_dwRef %d before drop\n"), pSource->m_dwRef);
		if(bDrag)
			pSource->DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE, NULL);
		else
		{
			HGLOBAL hgAddData = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(DROPEFFECT));
			DROPEFFECT *pDropEffect = (DROPEFFECT *)GlobalLock(hgAddData);
			if(bCut)
				*pDropEffect = DROPEFFECT_MOVE;
			else
				*pDropEffect = DROPEFFECT_COPY;
			GlobalUnlock(hgAddData);
			pSource->CacheGlobalData(RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT), hgAddData);
			pSource->SetClipboard();
		}
			
		//CRect(0,0,100,100));

		TRACE(_T("CCoolExplorerView::CopyItems pSource->m_dwRef %d after drop\n"), pSource->m_dwRef);
		if(bDrag)
		{			
			GlobalFree(hgData);
			pSource->Empty();
			delete pSource;
		}

		bIn=FALSE;
	}
		return TRUE;
}

void CCoolExplorerView::OnEditPaste()
{
	if(m_listFiles.IsCurJobStillActive())
		return;

	CItemsStorage *pData = NULL;

	m_listFiles.GetList()->LockList();
	{
		LISTDataPointers listPointers;
		pData = m_listFiles.GetList()->SaveItems(listPointers);
	}
	m_listFiles.GetList()->UnlockList();

	BOOL bRes = TRUE;
	if(pData)
	{
		m_dir_monitor->EnableRefresh();
		bRes = pData->PasteClipboard();
		delete pData;
		pData = NULL;		
	}
	
	if(!bRes)
		AfxMessageBox(_T("Error paste data"), MB_ICONERROR);

	/*if(!m_listFiles.GetList()->AreFilesOperationsAvailable())
	{
		AfxMessageBox(_T_LNGSTR(228), MB_ICONERROR);
		return;
	}

	COleDataObject oleDataObject;
	oleDataObject.AttachClipboard();
	CString sDescDir = m_listFiles.GetList()->GetFileFullPath(NULL);
	DROPEFFECT prefefect = 0;
	if(oleDataObject.IsDataAvailable(RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT)))
	{
		HGLOBAL hdata = oleDataObject.GetGlobalData(RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT));
		DWORD * de = (DWORD *)GlobalLock(hdata);
		prefefect = *de;
		GlobalUnlock(hdata);
		TRACE(prefefect & DROPEFFECT_COPY ? _T("copy\n") : _T("move"));
	}
	else
	{
		prefefect = DROPEFFECT_COPY;
	}
	OnDropPaste(&oleDataObject, prefefect, sDescDir);*/
}

void CCoolExplorerView::OnEditCopy()
{
	TRACE(_T("CCoolExplorerView::OnEditCopy begin\n"));
	if(m_listFiles.IsCurJobStillActive())
		return;
	TRACE(_T("CCoolExplorerView::OnEditCopy After still active\n"));

	LISTDataPointers listPointers;

	//if clicked none of items (nRow == -1 - menu for current dir will be displayed - listPointers empty
	BOOL bRes = FALSE;
	
	CItemsStorage *pData = NULL;
	m_listFiles.GetList()->LockList();
	{
		GetMarkedItems(listPointers);
		TRACE(_T("CCoolExplorerView::OnEditCopy before SaveItems\n"));
		if(!listPointers.IsEmpty())
			pData = m_listFiles.GetList()->SaveItems(listPointers);
		TRACE(_T("CCoolExplorerView::OnEditCopy after SaveItems pData 0x%X\n"), pData);
	}
	m_listFiles.GetList()->UnlockList();
	
	if(pData)
	{
		bRes = pData->CopyClipboard();
		delete pData;
		pData = NULL;
	}
		
	TRACE(_T("CCoolExplorerView::OnEditCopy after copy bres:%d\n"), bRes);
	
	if(listPointers.IsEmpty())
		AfxMessageBox(_T("Nothing to copy"), MB_ICONWARNING);
	else if(!bRes)
		AfxMessageBox(_T("Error copying data"), MB_ICONERROR);

	//CopyItems(FALSE, FALSE);
}

void CCoolExplorerView::OnEditCut()
{
	if(m_listFiles.IsCurJobStillActive())
		return;

	LISTDataPointers listPointers;

	//if clicked none of items (nRow == -1 - menu for current dir will be displayed - listPointers empty
	
	CItemsStorage *pData = NULL;
	m_listFiles.GetList()->LockList();
	{
		GetMarkedItems(listPointers);
		if(!listPointers.IsEmpty())
			pData = m_listFiles.GetList()->SaveItems(listPointers);
	}
	m_listFiles.GetList()->UnlockList();
	
	BOOL bRes = FALSE;
	if(pData)
	{
		bRes = pData->CutClipboard();
		delete pData;
		pData = NULL;
	}

	if(listPointers.IsEmpty())
		AfxMessageBox(_T("Nothing to cut"), MB_ICONWARNING);
	else if(!bRes)
		AfxMessageBox(_T("Error cut data"), MB_ICONERROR);
	//CopyItems(FALSE, TRUE);
}


LPCTSTR CCoolExplorerView::ConvertUnicodeShellArray(LPCWSTR sSource)
{
//	CStringList lFiles;
#ifndef UNICODE
	int nSize = 0, nSizeAll = 0;
	LPCWSTR sWideChars = sSource;
	while(*sWideChars)
	{
		nSize = wcslen(sWideChars);
		nSizeAll += nSize + 1;
		sWideChars += nSize + 1;
	}
	nSizeAll++;
	LPTSTR sTarget = new TCHAR[nSizeAll];
	WideCharToMultiByte(CP_ACP, 0, sSource,nSizeAll,sTarget, nSizeAll, NULL, NULL);
	return sTarget;
#else
	return sSource;
#endif
}

int CCoolExplorerView::SetItemsState(UINT nSearchMask, UINT nSetMask, UINT nValue /* =-1 */)
{
	int nItem = -1;
	int nChangeCount = 0;
	/*while((nItem = GetListCtrl().GetNextItem(nItem, nSearchMask)) != -1)
	{
		if(GetListCtrl().GetItemText(nItem, 0) != "..")
		{
			GetListCtrl().SetItemState(nItem, nValue, nSetMask);
			nChangeCount++;
		}
	}*/
	return nChangeCount++;
}

void CCoolExplorerView::OnMouseMove(UINT nFlags, CPoint point) 
{
	static CPoint pt = point;
	BOOL leftFlags = nFlags & MK_LBUTTON;
	BOOL rightFlags = nFlags & MK_RBUTTON;
	if(!CSettings::m_settings.bDisableDragDrop && (leftFlags || rightFlags))
	{
		//start drag if mouse really moves
		if(pt != point)
		{
			{
				DROPEFFECT bRes = FALSE;
				LISTDataPointers listPointers;
				CItemsStorage *pData = NULL;
				m_listFiles.GetList()->LockList();
				{
					GetMarkedItems(listPointers);
					if(!listPointers.IsEmpty())
					{
						pData = m_listFiles.GetList()->SaveItems(listPointers);				
					}
				}
				m_listFiles.GetList()->UnlockList();
				if(pData)
				{
					//m_nDraggedItem = m_Grid.GetMouseSelectedRow(point);
					m_nDraggedItem = m_Grid.GetFocusedRow();
					bRes = pData->DoDragDrop();
					delete pData;
					pData = NULL;
					m_nDraggedItem = -1;
					if(bRes != DROPEFFECT_NONE)
						PostMessage(WM_REFRESH);
				}
			}
		}
	}
	else
	{	
		CView::OnMouseMove(nFlags, point);
		m_nDraggedItem = -1;
	}
	pt = point;
}

void CCoolExplorerView::OnDestroy()
{
	if(!CSettings::m_settings.bDisableDragDrop)
		HRESULT hr = RevokeDragDrop(this->GetSafeHwnd());
	if(m_listFiles.IsCurJobStillActive())
		m_listFiles.BreakCurJob();
	if(m_pFilesListObject)
		m_pFilesListObject->TurnOff();

	m_listFiles.FtpDisconect(FALSE);	

	CSettings cfg;
	m_listFiles.SaveVirtualList(AddPathNames(gsFilesPath, m_strViewName + _T("VirtPanel") + _T(".cvp")));
	CString sDir = m_listFiles.GetDir();
	//get columns width
	if (m_strViewName == _T("CoolExpl_Left"))
	{
		m_Grid.GetColumnWidth(_T_LNGSTR(0),cfg.m_settings.lLeftColFilenameWidth);
		m_Grid.GetColumnWidth(_T_LNGSTR(1),cfg.m_settings.lLeftColExtensionsWidth);
		m_Grid.GetColumnWidth(_T_LNGSTR(2),cfg.m_settings.lLeftColSizeWidth);
		m_Grid.GetColumnWidth(_T_LNGSTR(3),cfg.m_settings.lLeftColDateWidth);
		m_Grid.GetColumnWidth(_T_LNGSTR(4),cfg.m_settings.lLeftColAttributesWidth);

		m_Grid.GetTabsDirs(cfg.m_settings.sLeftDir);
		if(cfg.m_settings.sLeftDir.size() == 0)
			cfg.m_settings.sLeftDir.push_back(sDir);
	}
	else if(m_strViewName == _T("CoolExpl_Right"))
	{
		m_Grid.GetColumnWidth(_T_LNGSTR(0),cfg.m_settings.lRightColFilenameWidth);
		m_Grid.GetColumnWidth(_T_LNGSTR(1),cfg.m_settings.lRightColExtensionsWidth);
		m_Grid.GetColumnWidth(_T_LNGSTR(2),cfg.m_settings.lRightColSizeWidth);
		m_Grid.GetColumnWidth(_T_LNGSTR(3),cfg.m_settings.lRightColDateWidth);
		m_Grid.GetColumnWidth(_T_LNGSTR(4),cfg.m_settings.lRightColAttributesWidth);

		m_Grid.GetTabsDirs(cfg.m_settings.sRightDir);
		if(cfg.m_settings.sRightDir.size() == 0)
			cfg.m_settings.sRightDir.push_back(sDir);
	}
	

	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

LRESULT CCoolExplorerView::OnRefreshIcon(WPARAM wParam, LPARAM lParam)
{
	m_Grid.InvalidateRect(NULL);
	//m_Grid.RedrawWindow();
	return 0;
}


LRESULT CCoolExplorerView::OnPostSetFocus(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 1)
	{
		CFrameWnd *pFrame = GetParentFrame();
		if(pFrame)
			pFrame->SetActiveView(this);//ccoolexplorerframe
		if(pFrame = pFrame->GetParentFrame())
			pFrame->SetActiveView(this);//CMainFrame*/
	}
	if(GetFocus()->GetSafeHwnd() != m_Grid.GetSafeHwnd())
		m_Grid.SetFocus();
	
	RefreshCommandBarPath();
	//m_Grid.RedrawWindow();
	return 0;
}

void CCoolExplorerView::OnSysCommand(UINT nID, LPARAM lParam) 
{
	CView::OnSysCommand(nID, lParam);
}

BOOL CCoolExplorerView::CanClose()
{
	BOOL bRet = TRUE;
	if(this != NULL)
	{
		POSITION pos;
		pos = m_lCopyFtpDlg.GetHeadPosition();
		while(pos)
		{
			CNewProgressDlg *pWnd = m_lCopyFtpDlg.GetNext(pos);
			if(IsWindow(pWnd->GetSafeHwnd()))
			{
				pWnd->Cancel();
				bRet = FALSE;
				CoolTrace(0, TRUE, _T("CCoolExplorerView::CanClose(), m_lCopyFtpDlg exists! %s"),m_strViewName);
				WriteTraces();
			}
		}
		//
		pos = m_lDelDlg.GetHeadPosition();
		while(pos)
		{
			CNewProgressDlg *pWnd = m_lDelDlg.GetNext(pos);
			if(IsWindow(pWnd->GetSafeHwnd()))
			{
				CoolTrace(0, TRUE, _T("CCoolExplorerView::CanClose(), m_lDelDlg exists! in %s"),m_strViewName);
				pWnd->Cancel();
				bRet = FALSE;
				WriteTraces();
			}
		}
		//
		pos = m_lCopyDlg.GetHeadPosition();
		while(pos)
		{
			CNewProgressDlg *pWnd = m_lCopyDlg.GetNext(pos);
			if(::IsWindow(pWnd->GetSafeHwnd()))
			{
				CoolTrace(0, TRUE, _T("CCoolExplorerView::CanClose(), m_lCopyDlg exists! in %s"),m_strViewName);
				pWnd->Cancel();
				bRet = FALSE;
				WriteTraces();
			}
		}
	}

	if (::IsWindow(m_wndMainSrchDlg.GetSafeHwnd()))
		m_wndMainSrchDlg.DestroyWindow();

	WriteTraces();

	return  bRet;
}

void CCoolExplorerView::OnShowThesameFiles() 
{
	m_Grid.Compare(GetAnotherView()->m_Grid, MARKTHESAME);
}

void CCoolExplorerView::OnShowDifferentFiles() 
{
	m_Grid.Compare(GetAnotherView()->m_Grid, MARKDIFFRENT);
}

BOOL CCoolExplorerView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;//CView::OnEraseBkgnd(pDC);
}

BOOL CCoolExplorerView::IsSharedFtpSession()
{
	BOOL bRet = FALSE;
	if(m_listFiles.GetList()->GetMode() == Ftp)
	{
		CFtpConnection *pFtpCon = m_listFiles.GetFtpConnection();
		if(CheckSharedFtpCon(pFtpCon) || GetAnotherView()->CheckSharedFtpCon(pFtpCon))
			bRet = TRUE;		
	}
	return bRet;
}

BOOL CCoolExplorerView::CheckSharedFtpCon(CFtpConnection *pFtpCon)
{
	BOOL bRet = FALSE;
	POSITION pos;
	pos = m_lCopyFtpDlg.GetHeadPosition();
	while(pos)
	{
		CFtpCopyDlg *pWnd = m_lCopyFtpDlg.GetNext(pos);
		if(pWnd->IsSharedFtpCon(pFtpCon))
		{
			bRet = TRUE;
			CoolTrace(0, TRUE, _T("CCoolExplorerView::CheckSharedFtpSession(), shared! %s"),m_strViewName);
			MessageBox(_T("Ftp connection is used for copying please wait!"), NULL,MB_ICONWARNING);
			return TRUE;
		}
	}

	pos = m_lDelDlg.GetHeadPosition();
	while(pos)
	{
		CDelDlg *pWnd = m_lDelDlg.GetNext(pos);
		pWnd->Create();
		if(IsWindow(pWnd->GetSafeHwnd()) && pWnd->IsSharedFtpCon(pFtpCon))
		{
			bRet = TRUE;
			CoolTrace(0, TRUE, _T("CCoolExplorerView::CheckSharedFtpSession(), shared! %s"),m_strViewName);
			MessageBox(_T("Ftp connection is used for deleting please wait!"), NULL, MB_ICONWARNING);
			return TRUE;
		}
	}	


	return bRet;
}

void CCoolExplorerView::SetFilters()
{
	CSetFiltersDlg dlg(this, m_listFiles.GetShowFilter(), m_listFiles.GetHideFilter());
	if(dlg.DoModal() == IDOK)
	{
		m_listFiles.SetFilters(dlg.m_sShow, dlg.m_sHide);
	}
	OnRefresh(0,0);
}
void CCoolExplorerView::AddNewTab()
{
	CString str = m_listFiles.GetDir();

	if (!str.IsEmpty())
	{
		m_Grid.AddTab(&str);
	}
	OnRefresh(0,0);

}
void CCoolExplorerView::RemoveLastTab()
{
	m_Grid.RemoveLastTab();
}

void CCoolExplorerView::RemoveActiveTab()
{
	m_Grid.RemovActiveTab();
	OnRefresh(0,0);
}


void CCoolExplorerView::SetWaitObject()
{
}

void CCoolExplorerView::OnHistPrev() 
{
	CString sPath = m_pHistory->HistoryPrev();
	if(!sPath.IsEmpty())
		m_pFilesListObject->SetDir(sPath);
}

void CCoolExplorerView::OnHistNext() 
{
	CString sPath = m_pHistory->HistoryNext();
	if(!sPath.IsEmpty())
		m_pFilesListObject->SetDir(sPath);
}

void CCoolExplorerView::OnUpdateHistNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pHistory->HistoryIsNext());
}

void CCoolExplorerView::OnUpdateHistPrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pHistory->HistoryIsPrev());
}

void CCoolExplorerView::OnFavorities() 
{
	CCoolExplorerFrame *pFrame = (CCoolExplorerFrame*)GetParentFrame();

	if(pFrame)
	{
		pFrame->m_wndPathBar.OnFavorities();
	}
}

void CCoolExplorerView::OnGotomain() 
{
	if(m_listFiles.IsCurJobStillActive())
		return;

	if(m_listFiles.GetList()->CanGoToMain())
	{
		m_listFiles.SetDir(m_listFiles.GetList()->GetDrivePath());
	}
	SetFocus();
}

void CCoolExplorerView::OnUpdateGotomain(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_listFiles.GetList()->CanGoToMain());
}


void CCoolExplorerView::EnterSelected()
{
	m_Grid.DoEnterSelected();
}

void CCoolExplorerView::OnCreateBatch() 
{
	if(m_listFiles.GetList()->AreFilesOperationsAvailable())
	{
		CCreateBatchDlg dlg(m_listFiles.GetList()->GetFileFullPath(NULL));
		dlg.DoModal();
	}
}

//extern CMyTimer gTimer;
void CCoolExplorerView::Update(CFilesListObservable *pObject, int nDataVersion, BOOL Result)
{
	m_pFilesListObject = pObject;	

	
	PostMessage(WM_EVENTUPDATE, Result, nDataVersion);
}

LRESULT CCoolExplorerView::OnEventUpdate(WPARAM Result, LPARAM parDataVer)
{
	int DataVer = (int) parDataVer;
	TRACE( _T("OnEventUpdate %d,%s\n"), Result, (LPCTSTR)m_strViewName);
	//gTimer.Start();
	//m_timer .Start();
	CoolTrace(3, TRUE, _T("OnEventUpdate %d"), Result);

	StructSaveSelection sel;
	SaveSelection(sel);

	m_Grid.SendMessage(WM_SETREDRAW, FALSE);
	int nNumDirs =0 ,nNumFiles = 0;
	ListData *pd = NULL;

	m_listFiles.GetList()->LockList();
//	m_dropTarget.SetPath(m_listFiles.GetList()->GetFileFullPath(NULL));
	m_nDataVersion = DataVer;
	m_Grid.SetLockUserDataInterface(&m_listFiles.GetList()->GetLockInterface());
	m_Grid.SetReadIconInterface(m_listFiles.GetList(), DataVer);
	m_Grid.AllocMem(m_listFiles.GetList()->GetAllCount());	
	POSITION pos = m_listFiles.GetList()->GetHeadPosition();
	
	while(pos)
	{
		pd = m_listFiles.GetList()->GetNext(pos);
		m_Grid.AddRow(/*s,pd->nIcon,*/ pd);
		//count number of dirs, to display it on statusbar
		pd->getAttrib() & CFile::directory ? nNumDirs++:nNumFiles++;
	}

//	m_Grid.SetUseUserData(TRUE);
//m_timer .Start();
	m_listFiles.GetList()->FreeOld();
	m_Grid.Sort();
	m_listFiles.GetList()->UnlockList();
	
	//		m_timer.Stop();
	//		sTime += _T("\nAfter Add to grid: ");
	//		sTime += m_timer.GetStringTime();
	//		
	//		m_timer.Start();
	
	
	//m_timer.Stop();
	//		sTime += _T("\nSort: ");
	//		sTime += m_timer.GetStringTime();
	//		m_timer.Start();
	
	RestoreSelection(sel);
	m_Grid.RecalcScrollBar();
	
	
	m_Grid.SendMessage(WM_SETREDRAW, TRUE);
	
	m_Grid.InvalidateRect(NULL);

	
	//m_Grid.SelectRow(0);
	//dsplay info in status bar
	
	//		m_Grid.GetNumMarkedRows()
	//display information in down status bar
	//m_timer .Start();
	//		m_Grid.ChangeActiveTab(pObject->GetDir(), pObject->GetDir());	
	
	//	if (bRet == TRUE)
	{
		CSettings cfg;
		if (cfg.m_settings.bRefreshDirs && m_dir_monitor)
		{
			m_dir_monitor->SetMonitor(CString(m_listFiles.GetDir()));
			m_dir_monitor->DisableRefresh();
			SetTimer(2, 1500, NULL);
		}
	}
	//EndWaitCursor();
	//SendMessage(WM_UPDATE_DRIVE_CAPACITY);
	RefreshDriveCapacityFrame(0,0);
	//m_timer.Stop();
	//CoolTrace(3, TRUE, _T("Setmonitor : %s sec, mode: %d, in %s"), m_timer.GetStringTime(), m_listFiles.GetList()->GetMode(), m_listFiles.GetDir());
	//NMarkingChanged();
	
	m_timer.Stop();
	
	CoolTrace(3, TRUE, _T("Listing files : %s sec, mode: %d, in %s"), m_timer.GetStringTime(), m_listFiles.GetList()->GetMode(), m_listFiles.GetDir());
	NMarkingChanged();
	m_bCurJobStillActive = FALSE;	
	
	//WriteTraces()
	//gTimer.Stop(TRUE);;
	return 1L;
}

void CCoolExplorerView::StopUseObservable(CFilesListObservable *pObject, LPCTSTR sNewPath)
{
	if(CSettings::m_settings.bRefreshDirs && m_dir_monitor)
		m_dir_monitor->DisableRefresh();
	m_sPrevPath = m_listFiles.GetDir();// for restore selection	
	//SaveSelection(sNewPath);

	CString sStatus = _T_LNGSTR(98);
	sStatus += ' ';
	sStatus += sNewPath;
	if (((CCoolExplorerFrame*)GetParentFrame()))
	{
		((CCoolExplorerFrame*)GetParentFrame())->m_wndDownStatusBar.SetPaneText(0,sStatus,TRUE);
	}

	m_timer.Start();
	m_bCurJobStillActive = TRUE;
}

//saves selection if dir was not changed (m_sPrevPath set in StopUseObservable)
void CCoolExplorerView::SaveSelection(StructSaveSelection &sel)
{
	CoolTrace(3, TRUE, _T("SaveSelection"));
	
	if(m_sPrevPath == m_listFiles.GetDir())
	{
		sel.nPrevSelected = m_Grid.GetFocusedRow();
		if(sel.nPrevSelected>=0 && sel.nPrevSelected<m_Grid.GetItemCount())
			sel.sPrevSelected = m_Grid.GetItemText(sel.nPrevSelected, 0);
		else
			sel.sPrevSelected = _T("");
		
		//marked
		CoolTrace(3, TRUE, _T("Focused  row:%d %s "), sel.nPrevSelected, sel.sPrevSelected);
		sel.tabPrevMarked.RemoveAll();
		CIntArray arInt;
		m_Grid.GetMarkedItems(arInt);
		
		for(int i = 0; i<arInt.GetSize(); i++)
		{			
			sel.tabPrevMarked.Add(m_Grid.GetItemText(arInt.GetAt(i),0));		
		}
		if(arInt.GetSize() == 1 && arInt.GetAt(0) == m_Grid.GetFocusedRow())
			sel.tabPrevMarked.RemoveAll();// this is only focused probably

		m_Grid.GetDispRange(sel.nPrevFirst, sel.nPrevLast);
	}
}

void CCoolExplorerView::RestoreSelection(StructSaveSelection &sel)
{
	CoolTrace(3, TRUE, _T("RestoreSelection %d %s %s"),sel.nPrevSelected, (LPCTSTR)sel.sPrevSelected, (LPCTSTR)m_strViewName );
	CString sCurDir = m_listFiles.GetDir();
	TCHAR cSep = m_listFiles.GetList()->GetDirSeparator();

	
	if(!m_sPrevItemToSelect.IsEmpty())
	{
		m_Grid.SelectRow(m_sPrevItemToSelect,0,TRUE);
		m_sPrevItemToSelect = _T("");
	}
	else if(m_sPrevPath == sCurDir) // there was only refresh
	{
		//
		BOOL bRet = m_Grid.SelectRow(sel.sPrevSelected,0,FALSE);
		if(!bRet)
		{
			if(sel.nPrevSelected<0)
				sel.nPrevSelected=0;
			if(sel.nPrevSelected>=m_Grid.GetNumRows())
				sel.nPrevSelected=m_Grid.GetNumRows()-1;
			m_Grid.SelectRow(sel.nPrevSelected);
			CoolTrace(3, TRUE, _T("Select row %d"), sel.nPrevSelected);
		}

		for(int i = 0; i<sel.tabPrevMarked.GetSize(); i++)
		{			
			m_Grid.MarkMaskedRows(sel.tabPrevMarked.GetAt(i));
		}

			//
		if(m_Grid.GetNumRows() < m_Grid.GetCountPerPage())
		{
			sel.nPrevFirst = 0;
			sel.nPrevLast = m_Grid.GetNumRows();
		}
		else
		{
			if(sel.nPrevSelected != -1) //focused was visible
			{
				int nCur = m_Grid.GetFocusedRow();
				int nDiff = sel.nPrevSelected - sel.nPrevFirst;
				sel.nPrevFirst = nCur - nDiff;
			}
			if(sel.nPrevFirst > m_Grid.GetNumRows()-m_Grid.GetCountPerPage())
				sel.nPrevFirst = m_Grid.GetNumRows()-m_Grid.GetCountPerPage();
			
			sel.nPrevLast = sel.nPrevFirst + m_Grid.GetCountPerPage();
		}
		m_Grid.SetDispRange(sel.nPrevFirst, sel.nPrevLast);
		

	}
	else if(!_tcsncmp(m_sPrevPath, sCurDir, sCurDir.GetLength()))// go up one or more levels
	{
		m_sPrevPath = m_sPrevPath.Mid(sCurDir.GetLength());
		m_sPrevPath.TrimLeft(cSep);
		m_sPrevPath.TrimRight(cSep);
		
		int nPos = m_sPrevPath.Find(cSep);
		if(!m_listFiles.IsPureVirtualList() && nPos != -1)
			m_sPrevPath = m_sPrevPath.Left(nPos);
		BOOL bRet = m_Grid.SelectRow(m_sPrevPath,0,TRUE);
		if(!bRet)
			m_Grid.SelectRow(0);
		
	}
	else
	{
		m_Grid.SelectRow(0);
	}

	m_Grid.RecalcScrollBar();

}

BOOL CCoolExplorerView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_bCurJobStillActive)
	{
		SetCursor( ::LoadCursor(NULL, IDC_WAIT) );
		return TRUE;
	}
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CCoolExplorerView::CreateList(LISTData & lst)
{
	LISTDataPointers listPointers;
	m_listFiles.GetList()->LockList();
	{
		GetMarkedItems(listPointers);
		
		POSITION pos = listPointers.GetHeadPosition();
		ListData ld;
		ListData *pld = NULL;
		while(pos)
		{
			pld = listPointers.GetNext(pos);

			ld.Clear();
			ld.setFileInfo(pld->getFileInfo());
			ld.setName(m_listFiles.GetList()->GetFileFullPath(pld));
			ld.setExactIcon(pld->getExactIcon());
			lst.AddTail(ld);
		}
	}
	m_listFiles.GetList()->UnlockList();

	return TRUE;
}

void CCoolExplorerView::OnCreateVirt() 
{
	m_listFiles.InitVirtualList(NULL);	
}

void CCoolExplorerView::OnUpdateCreateVirt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CCoolExplorerView::OnLoadVirtpanel() 
{	
	CFileDialog f(TRUE, _T("cvp"), NULL, OFN_OVERWRITEPROMPT, CString(_T_LNGSTR(241)) + _T("(*.cvp)|*.cvp|") + _T_LNGSTR(242) + _T("(*.*)|*.*||"));
	if(f.DoModal() == IDOK)
	{
		m_listFiles.LoadVirtualList(f.GetFileName());
	}
}

void CCoolExplorerView::OnUpdateLoadVirtpanel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CCoolExplorerView::OnSaveVirtpanel() 
{	
	CFileDialog f(FALSE, _T("cvp"), _T("cool.cvp"), OFN_OVERWRITEPROMPT, CString(_T_LNGSTR(241)) + _T("(*.cvp)|*.cvp|") + _T_LNGSTR(242) + _T("(*.*)|*.*||"));
	
	if(f.DoModal() == IDOK)
	{
		m_listFiles.SaveVirtualList(f.GetFileName());
	}	
}

void CCoolExplorerView::OnUpdateSaveVirtpanel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_listFiles.IsPureVirtualList());	
}

void CCoolExplorerView::OnDelete() 
{
	DeleteFile(FALSE);
}

void CCoolExplorerView::OnDeleteFromvirtual() 
{	
	if(m_listFiles.IsPureVirtualList())
	{
		DeleteFile(TRUE);
	}
}

void CCoolExplorerView::OnTimer(UINT_PTR nIDEvent) 
{
	if(nIDEvent == 1)
	{		
		m_dir_monitor->mutex_data.Lock();
		BOOL bRef = m_dir_monitor->m_Data.bRefresh;
		BOOL bEnable = m_dir_monitor->m_Data.bEnable;
		m_dir_monitor->mutex_data.Unlock();
	
		TRACE(_T("CCoolExplorerView::OnTimer 1, bRef:%d, bEnable:%d, %s\n"), bRef, bEnable,(LPCTSTR)m_strViewName);
		if ( bRef && bEnable)
		{
			OnRefresh(0,0);
		}
		m_dir_monitor->mutex_data.Lock();
		m_dir_monitor->m_Data.bRefresh = FALSE;
		m_dir_monitor->mutex_data.Unlock();

		KillTimer(1);
	}
	if(nIDEvent == 2)
	{
		TRACE(_T("CCoolExplorerView::OnTimer 2,%s\n"), (LPCTSTR)m_strViewName);
		m_dir_monitor->EnableRefresh();
		KillTimer(2);
	}
	CView::OnTimer(nIDEvent);
}

void CCoolExplorerView::OnUpdateStartipc(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

void CCoolExplorerView::OnStartipc() 
{
	((CCoolExplorerFrame*)GetParentFrame())->OnStartIPC();	
}
