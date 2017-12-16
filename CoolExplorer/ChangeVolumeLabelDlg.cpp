// ChangeVolumeLabelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "coolexplorer.h"
#include "ChangeVolumeLabelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeVolumeLabelDlg dialog


CChangeVolumeLabelDlg::CChangeVolumeLabelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeVolumeLabelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangeVolumeLabelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CChangeVolumeLabelDlg::SetDrive(LPCTSTR sDrive)
{
	m_Drive = sDrive;
}

CString CChangeVolumeLabelDlg::GetDrive()
{
	return m_Drive;
}

CString CChangeVolumeLabelDlg::GetNewLabel()
{
	return m_sNewLabel;
}


void CChangeVolumeLabelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeVolumeLabelDlg)
	DDX_Control(pDX, IDC_VLABEL_EDIT, m_EditVolumeLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeVolumeLabelDlg, CDialog)
	//{{AFX_MSG_MAP(CChangeVolumeLabelDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeVolumeLabelDlg message handlers

BOOL CChangeVolumeLabelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!m_Drive.IsEmpty())
	{
		m_Drive += _T(":\\");
		TCHAR lpVolumeNameBuffer[1024]={0};
		DWORD nVolumeNameSize = sizeof(lpVolumeNameBuffer)/sizeof(TCHAR) -1;
		DWORD lpVolumeSerialNumber;
		DWORD lpMaximumComponentLength;
		DWORD lpFileSystemFlags;
		TCHAR lpFileSystemNameBuffer[1024]={0};
		DWORD nFileSystemNameSize = sizeof(lpFileSystemNameBuffer)/sizeof(TCHAR) -1;
		
		BOOL bRet = GetVolumeInformation(m_Drive,
					lpVolumeNameBuffer,
					nVolumeNameSize,
					&lpVolumeSerialNumber,
					&lpMaximumComponentLength,
					&lpFileSystemFlags,
					lpFileSystemNameBuffer,
					nFileSystemNameSize	);
		if (bRet)
		{
			//put data to edit
			m_EditVolumeLabel.SetWindowText(lpVolumeNameBuffer);

			//mark selection
			m_EditVolumeLabel.SetSel(0,-1);

		}
		else
		{
			CoolTrace(0, TRUE, _T("CChangeVolumeLabelDlg::OnInitDialog, GetVolumeInformation error from drive %s is %d"), m_Drive,GetLastError());
			EndDialog(IDCANCEL);
		}

	}
	else
	{
		EndDialog(IDCANCEL);
	}

	CString label;
	label.Format(_T_LNGSTR(89),m_Drive);

	SetWindowText(_T_LNGSTR(88));

	_DLG_STR_LANGSTR(label,IDC_VLABEL_EDIT_STATIC);
	_DLG_ITEM_LANGSTR(26,IDCANCEL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChangeVolumeLabelDlg::OnOK() 
{
	// TODO: Add extra validation here
	//get data from edit, put it to m_sNewLabel string
	m_EditVolumeLabel.GetWindowText(m_sNewLabel);

	CDialog::OnOK();
}
