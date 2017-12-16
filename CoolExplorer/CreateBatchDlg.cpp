// SmartCopyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "coolexplorer.h"
#include "TooltipDlg.h"
#include "CreateBatchDlg.h"
#include "UnicodeFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateBatchDlg dialog
extern CString gsFilesPath;

CCreateBatchDlg::CCreateBatchDlg(LPCTSTR sFromDir, CWnd* pParent /*=NULL*/)
	: CTooltipDlg(CCreateBatchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateBatchDlg)
	m_sComAndDir = AddPathNames(sFromDir, TAG_RELATIVE_PATH);
	m_sBatchFile = AddPathNames(gsFilesPath, _T("batch.bat"));
	m_bRunCommand = FALSE;
	m_sDestFile = TAG_FULL_NAME;
	m_sDir = sFromDir;
	m_sMask = _T("*.*");
	//}}AFX_DATA_INIT
}


void CCreateBatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CTooltipDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateBatchDlg)
	DDX_Text(pDX, IDC_COM_AND_DIR, m_sComAndDir);
	DDX_Text(pDX, IDC_BATCH_FILE, m_sBatchFile);
	DDX_Check(pDX, IDC_CHECK_RUN_COMMAND, m_bRunCommand);
	DDX_Text(pDX, IDC_DEST_FILE, m_sDestFile);
	DDX_Text(pDX, IDC_FROM_DIR, m_sDir);
	DDX_Text(pDX, IDC_MASK, m_sMask);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateBatchDlg, CTooltipDlg)
	//{{AFX_MSG_MAP(CCreateBatchDlg)	
	ON_BN_CLICKED(IDC_CREATE_BATCH, OnCreateBatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateBatchDlg message handlers

void CCreateBatchDlg::OnOK() 
{
	CTooltipDlg::OnOK();
}

BOOL CCreateBatchDlg::OnInitDialog() 
{
	CTooltipDlg::OnInitDialog();
	
	AddToolTip(IDC_COM_AND_DIR, CSmartNameManager::GetHelp());
	AddToolTip(IDC_DEST_FILE, CSmartNameManager::GetHelp());

	SetWindowText(_T_LNGSTR(213));
	_DLG_ITEM_LANGSTR(207,IDC_SC_DIR_STATIC);
	_DLG_ITEM_LANGSTR(208,IDC_SC_MASK_STATIC);
	_DLG_ITEM_LANGSTR(209,IDC_SC_COM_STATIC);
	_DLG_ITEM_LANGSTR(210,IDC_SC_DEST_STATIC);
	_DLG_ITEM_LANGSTR(211,IDC_SC_BATCH_STATIC);
	_DLG_ITEM_LANGSTR(212,IDC_CHECK_RUN_COMMAND);
	_DLG_ITEM_LANGSTR(213,IDC_CREATE_BATCH);
	_DLG_ITEM_LANGSTR(32,IDCANCEL);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCreateBatchDlg::OnCreateBatch() 
{
	UpdateData();
	CString sBatch;
	CSmartNameManager info;
	if(info.Init(m_sDir, m_sDestFile, m_sComAndDir))
	{
		info.CreateBatch(sBatch, m_sDir, m_sMask, m_bRunCommand);
		if(!CUnicodeFile::WriteToFile(m_sBatchFile, sBatch, TRUE))
		{
			CString sMes;
			sMes.Format(_T("Cannot open file: %s"), m_sBatchFile);
			ErrorMessage(sMes);
		}
		m_sBatchFile = _T("notepad.exe ") + m_sBatchFile;
		_tsystem(m_sBatchFile);
	}
}
