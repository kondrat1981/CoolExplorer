// CoolExplorerView.h : interface of the CCoolExplorerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COOLEXPLORERVIEW_H__0F8F8090_1481_4573_B490_C29F257A7397__INCLUDED_)
#define AFX_COOLEXPLORERVIEW_H__0F8F8090_1481_4573_B490_C29F257A7397__INCLUDED_
#include "FastFileSrchDlg.h"
#include "FilesListManager.h"	// Added by ClassView
#include "TMyGrid.h"
#include "TViewer.h"
#include "MyTimer.h"
#include "DropTarget.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDirMonitor;
class CFtpCopyDlg;
class CDelDlg;
class CCopyDlg;
class CMainFrame;


class CPathHistory;
class CCoolExplorerView : public CView, public CFilesListObserverInterface, public ListDataCreator
{

protected: // create from serialization only
	CCoolExplorerView();
	friend class CCoolExplorerFrame;
	DECLARE_DYNCREATE(CCoolExplorerView)

protected:
	CString	m_strViewName;
	TMyGrid	m_Grid;
// Attributes
public:
	virtual void Update(CFilesListObservable *, int nDataVersion, BOOL Result);
	virtual void StopUseObservable(CFilesListObservable *, LPCTSTR sNewPath);

	//ListDataCreator interface
	virtual BOOL CreateList(LISTData & lst);
	//



	LPCTSTR StringToBuf(LPCTSTR string);
	
	/*-------------------------- events from grid ----------------------------*/
	BOOL NSelectRow(int nRow);
	BOOL NRightClick(int nRow);
	BOOL NMarkingChanged();
	BOOL NEditChanged(CString &from,CString &to);
	BOOL CheckIsEmptyCommandBar();
	BOOL RunCommandBar();
	BOOL KeyPressed(UINT nChar, UINT nRepCnt, UINT nFlags, CString str);
	/*------------------------------------------------------------------------*/


	CCoolExplorerDoc* GetDocument();
	CMainFrame *GetMainFrame();
	
	
	void SetWaitObject();

	void SetPathBarPtr(CWnd *pPathBar){m_pPathBar = pPathBar;}
	void RefreshConfiguration();
	afx_msg LRESULT RenameItem(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT RefreshDriveCapacityFrame(WPARAM, LPARAM);
	afx_msg LRESULT OnSearchWindowFinished(WPARAM wparam, LPARAM lparam);
private:
	IContextMenu2 * m_pContextMenu;
	CMenu m_hist_menu;

// Operations
public:
	
	//Drag & Drop
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	BOOL CopyItems(BOOL bDrag, BOOL bCut);
	BOOL OnDropPaste(COleDataObject *pDataObject, DROPEFFECT dropeffect, LPCTSTR sTarget);
	
	int SetItemsState(UINT nSearchMask, UINT nSetMask, UINT nValue = -1);
	void OnEditPaste();
	void OnEditCut();
	void OnEditCopy();
	void RefreshCommandBarPath();
	void AddNewTab();	
	void RemoveLastTab();
	void RemoveActiveTab();
	//
	void EnterSelected();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolExplorerView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnAppFindDialog( );
	virtual void OnAppChanegeVolLabel();
	//virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
//    virtual void OnDragLeave();
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCoolExplorerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
//	virtual void PathChanged(LPCTSTR sPath, const void *pData, int nPanel);

// Generated message map functions
protected:
	void DeleteFile(BOOL bFromVirtual = FALSE, BOOL bUseShell = FALSE);
	void SetFilters();
	BOOL CheckSharedFtpCon(CFtpConnection *pFtpCon);
	BOOL IsSharedFtpSession();
	void OnCopyMoveFile(BOOL bMove, BOOL bUseShell);
//	vector<CMainSearchDlg> m_wndMainSrchDlg;
	static CMainSearchDlg m_wndMainSrchDlg;
	LPCTSTR ConvertUnicodeShellArray(LPCWSTR sSource);
//	COleDropTarget m_DropTarget;
	int m_nDropTargetItem;
//	DROPEFFECT m_DropEffect;
	int m_nDraggedItem;

	void				ActivateNextView();
	CWnd				*m_pPathBar;
	CFilesListManager		m_listFiles;
	CList<CFtpCopyDlg *, CFtpCopyDlg*> m_lCopyFtpDlg;
	CList<CDelDlg *, CDelDlg*> m_lDelDlg;
	CList<CCopyDlg *, CCopyDlg*> m_lCopyDlg;
//	CString		m_sBeforeFtp;
	void		OnCopyItems();
	void		OnPasteItems(); 
	void		OnCutItems();
	void		OpenSelectedFile(LPCTSTR sProgram);
    void        DefaultViewSelectedFile(LPCTSTR sProgram);
	//{{AFX_MSG(CCoolExplorerView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCopyFile();
	afx_msg void OnMoveFile();	
	afx_msg void OnCreateDir();
	afx_msg void OnViewFile();
	afx_msg void OnEditFile();
	afx_msg void OnCreateFile();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFtpConnect();
	afx_msg void OnFtpDisconect();
	afx_msg void OnFtpLogin();
	afx_msg void OnChangeAttrib();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnShowThesameFiles();
	afx_msg void OnShowDifferentFiles();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHistPrev();
	afx_msg void OnHistNext();
	afx_msg void OnUpdateHistNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHistPrev(CCmdUI* pCmdUI);
	afx_msg void OnFavorities();
	afx_msg void OnGotomain();
	afx_msg void OnUpdateGotomain(CCmdUI* pCmdUI);
	afx_msg void OnCreateBatch();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnCreateVirt();
	afx_msg void OnUpdateCreateVirt(CCmdUI* pCmdUI);
	afx_msg void OnLoadVirtpanel();
	afx_msg void OnUpdateLoadVirtpanel(CCmdUI* pCmdUI);
	afx_msg void OnSaveVirtpanel();
	afx_msg void OnUpdateSaveVirtpanel(CCmdUI* pCmdUI);
	afx_msg void OnDelete();
	afx_msg void OnDeleteFromvirtual();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnUpdateStartipc(CCmdUI* pCmdUI);
	afx_msg void OnStartipc();
	//}}AFX_MSG
	afx_msg LRESULT OnRefreshIcon(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetPMenu(WPARAM, LPARAM);
	afx_msg LRESULT OnPostSetFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEventUpdate(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
public:
	BOOL CanClose();
	CFilesListManager	*GetFilesList(){return &m_listFiles;};
	BOOL				GetMarkedItems(LISTDataPointers &listPointers);
	BOOL				GetMarkedNames(CStringList &listNames, CString *psMes = NULL,BOOL bFilesOnly = FALSE, BOOL bFullPath = FALSE, int *pnCountFiles = NULL, int *pnCountDirs = NULL);
	void				GetNamesCount(int &nCountMarkedFiles,int &nCountUnmarkedFiles,int &nCountMarkedDir,int &nCountUnmarkedDir);
	CFastFileSrchDlg	m_wndFastFileSrchDlg;
	int					CountSimilarItems( CString sSelectItem );
	BOOL				GoToSimilarItem(int i, CString &sSelectItem);
	BOOL				DisplayFiles(LPCTSTR sPath, ListData *pd = NULL);
	afx_msg void		OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void		OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT		OnRefresh(WPARAM, LPARAM);
	LPCTSTR				GetMarkedNamesForShell();
	void				OnShrecycleDeleteFile() ;
	CCoolExplorerView * GetAnotherView();
//----------------------viewer members and interface------------------------
private:	
	TViewer m_viewer;
	std::vector <TViewer*> m_viewer_list;

	CDirMonitor *m_dir_monitor; //monitoring directory changes
	CMyTimer m_timer;			//performance timer
	CFilesListObservable *m_pFilesListObject;
	int m_nDataVersion;
	
	//save and restore selection
	struct StructSaveSelection
	{
		StructSaveSelection() : nPrevSelected(0), nPrevFirst(0), nPrevLast(0){;}
		int nPrevSelected;
		int nPrevFirst;
		int nPrevLast;
		CString sPrevSelected;	
		CStringArray tabPrevMarked;
	};
	void RestoreSelection(StructSaveSelection &sel);
	void SaveSelection(StructSaveSelection &sel);
	CString m_sPrevPath;
	CString m_sPrevItemToSelect;//will be selected even dir was changed (eg. after search dialog, after create dir)
	//save and restore selection END
	CPathHistory *m_pHistory;
	BOOL m_bCurJobStillActive;
	CDropTarget m_dropTarget;
	//MyDragInfo m_dragInfo;
	CItemsStorage *m_pDragTarget;


public:
	
	

//----------------------viewer members and interface------------------------

};


#ifndef _DEBUG  // debug version in CoolExplorerView.cpp
inline CCoolExplorerDoc* CCoolExplorerView::GetDocument()
   { return (CCoolExplorerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLEXPLORERVIEW_H__0F8F8090_1481_4573_B490_C29F257A7397__INCLUDED_)
