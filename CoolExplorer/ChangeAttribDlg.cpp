// ChangeAttribDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoolExplorer.h"
#include "ChangeAttribDlg.h"
#include "FilesTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeAttribDlg dialog


CChangeAttribDlg::CChangeAttribDlg(LPCTSTR sPath, CStringList *pFiles, CWnd* pParent /*=NULL*/)
	: CMoreDialog(CChangeAttribDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangeAttribDlg)
	m_sMask = _T("*.*");

	//}}AFX_DATA_INIT

/*	m_nArchive = BST_INDETERMINATE;
	m_bCreation = FALSE;
	m_nSystem = BST_INDETERMINATE;
	m_bModyfication = FALSE;
	m_bLastAccess = FALSE;
	m_nHidden = BST_INDETERMINATE;
	m_nReadOnly = BST_INDETERMINATE;
*/


	m_sPath = sPath;
	m_lFiles.AddTail(pFiles);

	m_Info.nArchive = BST_INDETERMINATE;
	m_Info.nSystem = BST_INDETERMINATE;
	m_Info.nReadOnly = BST_INDETERMINATE;
	m_Info.nHidden = BST_INDETERMINATE;
	m_Info.bCreation = BST_UNCHECKED;
	m_Info.bModified = BST_UNCHECKED;
	m_Info.bLastAccess = BST_UNCHECKED;
}


void CChangeAttribDlg::DoDataExchange(CDataExchange* pDX)
{
	CMoreDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeAttribDlg)
	DDX_Control(pDX, IDC_MODYFICATION_DATE, m_ModyficationDate);
	DDX_Control(pDX, IDC_CREATION_DATE, m_CreationDate);
	DDX_Control(pDX, IDC_LASTACCESS_DATE, m_LastAccessDate);
	DDX_Control(pDX, IDC_CREATION_TIME, m_CreationTime);
	DDX_Control(pDX, IDC_LASTACCESS_TIME, m_LastAccessTime);
	DDX_Control(pDX, IDC_MODYFICATION_TIME, m_ModyficationTime);
	DDX_Text(pDX, IDC_EDIT_MASK, m_sMask);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_ARCHIVE_CHECK, m_Info.nArchive);
	DDX_Check(pDX, IDC_CREATION_CHECK, m_Info.bCreation);
	DDX_Check(pDX, IDC_SYSTEM_CHECK, m_Info.nSystem);
	DDX_Check(pDX, IDC_MODYFICATION_CHECK, m_Info.bModified);
	DDX_Check(pDX, IDC_LASTACCESS_CHECK, m_Info.bLastAccess);
	DDX_Check(pDX, IDC_HIDDEN_CHECK, m_Info.nHidden);
	DDX_Check(pDX, IDC_READONLY_CHECK, m_Info.nReadOnly);
}


BEGIN_MESSAGE_MAP(CChangeAttribDlg, CMoreDialog)
	//{{AFX_MSG_MAP(CChangeAttribDlg)
	ON_BN_CLICKED(IDC_MODYFICATION_CHECK, OnModyficationCheck)
	ON_BN_CLICKED(IDC_CREATION_CHECK, OnCreationCheck)
	ON_BN_CLICKED(IDC_LASTACCESS_CHECK, OnLastmodyficationCheck)
//	ON_BN_CLICKED(IDC_ADVANCED, OnAdvanced)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeAttribDlg message handlers

BOOL CChangeAttribDlg::OnInitDialog() 
{
	CMoreDialog::OnInitDialog();

	COleDateTime time;
	time = COleDateTime::GetCurrentTime();

	m_ModyficationDate.SetDate(time);
	m_CreationDate.SetDate(time);
	m_LastAccessDate.SetDate(time);
	m_ModyficationTime.SetTime(time);
	m_CreationTime.SetTime(time);
	m_LastAccessTime.SetTime(time);

//	OnAdvanced();
	SetWindowText(_T_LNGSTR(20));
	_DLG_ITEM_LANGSTR(16,IDC_STATIC_CA1);
	_DLG_ITEM_LANGSTR(17,IDC_STATIC_CA2);
	_DLG_ITEM_LANGSTR(18,IDC_STATIC_CA3);
	_DLG_ITEM_LANGSTR(19,IDC_STATIC_CA4);
	_DLG_ITEM_LANGSTR(21,IDC_STATIC_CA5);
	_DLG_ITEM_LANGSTR(22,IDC_ARCHIVE_CHECK);
	_DLG_ITEM_LANGSTR(23,IDC_READONLY_CHECK);
	_DLG_ITEM_LANGSTR(24,IDC_HIDDEN_CHECK);
	_DLG_ITEM_LANGSTR(25,IDC_SYSTEM_CHECK);
	_DLG_ITEM_LANGSTR(26,IDCANCEL);
//	m_ModyficationDate.SetTime();
/*	CreateDateTimeCtrl(&m_ModyficationDate, IDC_MODYFICATION_DATE, TRUE);
	CreateDateTimeCtrl(&m_CreationDate, IDC_CREATION_DATE, TRUE);
	CreateDateTimeCtrl(&m_LastAccessDate, IDC_LASTACCESS_DATE, TRUE);
	CreateDateTimeCtrl(&m_ModyficationTime, IDC_MODYFICATION_TIME, FALSE);
	CreateDateTimeCtrl(&m_CreationTime, IDC_CREATION_TIME, FALSE);
	CreateDateTimeCtrl(&m_LastAccessTime, IDC_LASTACCESS_TIME, FALSE);
*/	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*
BOOL CChangeAttribDlg::CreateDateTimeCtrl(CDateTimeCtrl *pCtrl, UINT nID, BOOL bDate)
{
	CWnd *pStatic = GetDlgItem(nID);
	RECT rect;
	pStatic->GetWindowRect(&rect);
	ScreenToClient(&rect);
	pStatic->DestroyWindow();
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_TABSTOP | DTS_SHOWNONE;
	if(bDate)
		dwStyle |= DTS_SHORTDATEFORMAT;
	else
		dwStyle |= DTS_TIMEFORMAT;
	return pCtrl->Create(dwStyle, rect, this, nID);
}
*/

void CChangeAttribDlg::OnModyficationCheck() 
{
	if(((CButton *)GetDlgItem(IDC_MODYFICATION_CHECK))->GetCheck() == BST_CHECKED)
	{
		m_ModyficationDate.EnableWindow(TRUE);
		m_ModyficationTime.EnableWindow(TRUE);
	}
	else
	{
		m_ModyficationDate.EnableWindow(FALSE);
		m_ModyficationTime.EnableWindow(FALSE);
	}
}

void CChangeAttribDlg::OnCreationCheck() 
{
	if(((CButton *)GetDlgItem(IDC_CREATION_CHECK))->GetCheck() == BST_CHECKED)
	{
		m_CreationDate.EnableWindow(TRUE);
		m_CreationTime.EnableWindow(TRUE);
	}
	else
	{
		m_CreationDate.EnableWindow(FALSE);
		m_CreationTime.EnableWindow(FALSE);
	}	
}

void CChangeAttribDlg::OnLastmodyficationCheck() 
{
	if(((CButton *)GetDlgItem(IDC_LASTACCESS_CHECK))->GetCheck() == BST_CHECKED)
	{
		m_LastAccessDate.EnableWindow(TRUE);
		m_LastAccessTime.EnableWindow(TRUE);
	}
	else
	{
		m_LastAccessDate.EnableWindow(FALSE);
		m_LastAccessTime.EnableWindow(FALSE);
	}
}

void CChangeAttribDlg::OnOK() 
{
	UpdateData();
	COleDateTime date;

	if(m_Info.bCreation)
	{
		date = m_CreationDate.GetDate() + m_CreationTime.GetTime();
		if(!OleTimeToUTCFileTime(m_Info.tmCreation, date))
		{
			MessageBox(_T_LNGSTR(5));
			return;
		}
	}

	if(m_Info.bModified)
	{
		date = m_ModyficationDate.GetDate() + m_ModyficationTime.GetTime();
		if(!OleTimeToUTCFileTime(m_Info.tmModified, date))
		{
			MessageBox(_T_LNGSTR(6));
			return;
		}
	}

	if(m_Info.bLastAccess)
	{
		date = m_LastAccessDate.GetDate() + m_LastAccessTime.GetTime();
		if(!OleTimeToUTCFileTime(m_Info.tmLastAccess, date))
		{
			MessageBox(_T_LNGSTR(7));
			return;
		}
	}
	
	
//time = m_CreationTime.GetTime();

//	m_Info.tmCreation = CTime(date.GetYear(), date.GetMonth(), date.GetDay(),
//								time.GetHour(), time.GetMinute(), time.GetSecond());
//	m_ModyficationDate.GetTime(date);
//	m_ModyficationTime.GetTime(time);
//	m_Info.tmModified = CTime(date.GetYear(), date.GetMonth(), date.GetDay(),
//								time.GetHour(), time.GetMinute(), time.GetSecond());

//	m_LastAccessDate.GetTime(date);
//	m_LastAccessTime.GetTime(time);
//	m_Info.tmLastAccess = CTime(date.GetYear(), date.GetMonth(), date.GetDay(),
//								time.GetHour(), time.GetMinute(), time.GetSecond());
	
	CDirTree dt;
	if(m_sMask.IsEmpty())
		m_sMask = _T("*");
	if(m_sPath.Right(1) == '\\')
		m_sPath.Delete(m_sPath.GetLength()-1);
	dt.Init(m_sPath, m_sMask, m_lFiles, NULL);
	dt.SetStatus(&m_Info, m_sPath);
	CMoreDialog::OnOK();
}

/*
void CChangeAttribDlg::OnAdvanced() 
{
	CRect rect, rectline;
	GetWindowRect(rect);
	GetDlgItem(IDC_LINE)->GetWindowRect(rectline);
	if(rect.bottom != rectline.bottom)
	{
		rect.bottom = rectline.bottom;
		MoveWindow(rect);
		GetDlgItem(IDC_ADVANCED)->SetWindowText(_T("<< Advanced"));
	}
	else
	{
		GetDlgItem(IDC_LINE2)->GetWindowRect(rectline);
		rect.bottom = rectline.bottom;
		MoveWindow(rect);
		GetDlgItem(IDC_ADVANCED)->SetWindowText(_T(">> Advanced"));
	}
}
*/
