// FtpConSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoolExplorer.h"
#include "FtpConSetDlg.h"


// CFtpConSetDlg dialog

IMPLEMENT_DYNAMIC(CFtpConSetDlg, CDialog)
CFtpConSetDlg::CFtpConSetDlg(CWnd* pParent /*=NULL*/, const CFtpConSet* pSet /*=NULL*/)
	: CDialog(CFtpConSetDlg::IDD, pParent)	
{
	ASSERT(pSet); //should be pass to constructor
	m_recSet = *pSet;
}

CFtpConSetDlg::~CFtpConSetDlg()
{
}

void CFtpConSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FCS_NAME, m_recSet.m_sName);
	DDX_Text(pDX, IDC_FCS_SERVERNAME, m_recSet.m_sServer);
	DDX_Text(pDX, IDC_FCS_USER, m_recSet.m_sUser);
	DDX_Text(pDX, IDC_FCS_PASSWORD, m_recSet.m_sPassword);
	DDX_Text(pDX, IDC_FCS_REMOTEDIR, m_recSet.m_sRemoteDir);
	
	int n = m_recSet.m_nPort;
	DDX_Text(pDX, IDC_FCS_PORT, n);
	m_recSet.m_nPort = n;
	DDX_Check(pDX, IDC_FCS_PASSIVE, m_recSet.m_bPassive);
	DDX_Check(pDX, IDC_FCS_SSL, m_recSet.m_bSSL);
}


BEGIN_MESSAGE_MAP(CFtpConSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CFtpConSetDlg message handlers

BOOL CFtpConSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(_T_LNGSTR(229));
	_DLG_ITEM_LANGSTR(230,IDC_SFCS_NAME);
	_DLG_ITEM_LANGSTR(231,IDC_SFCS_SERVERNAME);
	_DLG_ITEM_LANGSTR(232,IDC_SFCS_USER);
	_DLG_ITEM_LANGSTR(233,IDC_SFCS_PASSWORD);
	_DLG_ITEM_LANGSTR(234,IDC_SFCS_REMOTEDIR);
	_DLG_ITEM_LANGSTR(235,IDC_SFCS_PORT);
	_DLG_ITEM_LANGSTR(236,IDC_FCS_PASSIVE);
	_DLG_ITEM_LANGSTR(237,IDC_FCS_SSL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFtpConSetDlg::OnBnClickedOk()
{
	OnOK();
}

BOOL CFtpConSetDlg::ValidData(void)
{
	BOOL bRet = TRUE;

	if(m_recSet.m_sName.IsEmpty()
		|| m_recSet.m_sPassword.IsEmpty()
		|| m_recSet.m_sServer.IsEmpty()
		|| m_recSet.m_sUser.IsEmpty()
		)
	{
		if(AfxMessageBox(_T("Not all data enterd continue?"), MB_ICONQUESTION | MB_YESNO) == IDNO)
			bRet = FALSE;
	}

	return bRet;
}

void CFtpConSetDlg::OnOK()
{
	UpdateData();
	if(ValidData())
		CDialog::OnOK();
}
