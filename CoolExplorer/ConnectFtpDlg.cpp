// ConnectFtpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoolExplorer.h"
#include "FtpConSetDlg.h"
#include "ConnectFtpDlg.h"
#include "Markup.h"
#include "UnicodeFile.h"
#include "ARACrypt.h"

// CConnectFtpDlg dialog
IMPLEMENT_DYNAMIC(CConnectFtpDlg, CDialog)

const char *sKey = "dfdgttrere#%^&q";
extern CString gsFilesPath;

CConnectFtpDlg::CConnectFtpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectFtpDlg::IDD, pParent)
	, m_nPos(0)
{
	m_sXmlFile = AddPathNames(gsFilesPath, _T("ftpset.xml"));
}

CConnectFtpDlg::~CConnectFtpDlg()
{
}

void CConnectFtpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CF_LIST, m_ListBox);
	DDX_Control(pDX, IDC_CF_EDIT, m_butEdit);
	DDX_Control(pDX, IDC_CF_DELETE, m_butDelete);

	if(pDX->m_bSaveAndValidate == FALSE)
		FillList();
}


BEGIN_MESSAGE_MAP(CConnectFtpDlg, CDialog)
	ON_LBN_DBLCLK(IDC_CF_LIST, OnDblclkCfList)
	ON_BN_CLICKED(IDC_CF_CONNECT, OnBnClickedCfConnect)
	ON_BN_CLICKED(IDC_CF_NEW_CONNECT, OnBnClickedCfNewConnect)
	ON_BN_CLICKED(IDC_CF_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_CF_DELETE, OnBnClickedCfDelete)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CConnectFtpDlg message handlers

void CConnectFtpDlg::OnBnClickedCfConnect()
{	
	OnOK();
}

BOOL CConnectFtpDlg::SaveSettings(void)
{
	CString sPas;
	CString sSSL;

	ARACrypt obCrypt;
	CMarkup xml;
	xml.AddElem( _T("CConnectFtpDlg") );
	xml.IntoElem();
	xml.AddElem( _T("Settings") );
	xml.IntoElem();
	for(int i=0; i<m_aSettings.GetSize(); i++)
	{
		xml.AddElem( _T("ELEM") );
		xml.IntoElem();

		xml.AddElem( _T("m_sName"), m_aSettings[i].m_sName );
		xml.AddElem( _T("m_sServer"), m_aSettings[i].m_sServer );
		xml.AddElem( _T("m_sUser"), m_aSettings[i].m_sUser );
		xml.AddElem( _T("m_sRemoteDir"), m_aSettings[i].m_sRemoteDir );
		
		sPas = m_aSettings[i].m_sPassword;
		obCrypt.TransformString(sKey, sPas);
		xml.AddElem( _T("sPas"), sPas);

		sPas.Format(_T("%d"), m_aSettings[i].m_nPort);
		xml.AddElem( _T("m_nPort"), sPas);
		sPas.Format(_T("%d"), m_aSettings[i].m_bPassive);
		xml.AddElem( _T("m_bPassive"), sPas);

		sSSL.Format(_T("%d"), m_aSettings[i].m_bSSL);
		xml.AddElem( _T("m_bSSL"), sSSL);

		xml.OutOfElem();
	}
	CString sTmp; sTmp.Format(_T("%d"), m_nPos);
	xml.AddElem(_T("m_nPos"), sTmp);
	xml.OutOfElem();
	
	CString sXml = xml.GetDoc();
	BOOL bRet = CUnicodeFile::WriteToFileBinary(m_sXmlFile, sXml, TRUE);
	return bRet;
}

BOOL CConnectFtpDlg::ReadSettings(void)
{
	BOOL bRet = TRUE;
    CString sXml;
	ARACrypt obCrypt;
	CMarkup xml;
	bRet = CUnicodeFile::ReadFromFileBinary(m_sXmlFile, sXml);
	m_aSettings.RemoveAll();
	if(bRet)
	{
		if(!xml.SetDoc(sXml))
			return FALSE;

		if(!xml.FindElem(_T("CConnectFtpDlg")))
			return FALSE;
		xml.IntoElem();
		if(!xml.FindElem(_T("Settings")))
			return FALSE;

		xml.IntoElem();
		while ( xml.FindElem(_T("ELEM")) )
		{
			CFtpConSet elem;
			
			if (xml.FindChildElem( _T("m_sName") ))
			{
				elem.m_sName = xml.GetChildData();
			}
			
			if (xml.FindChildElem( _T("m_sServer")))
			{
				elem.m_sServer = xml.GetChildData();
			}
			
			if (xml.FindChildElem( _T("m_sUser")))
			{
				elem.m_sUser = xml.GetChildData();
			}

			if (xml.FindChildElem( _T("m_sRemoteDir") ))
			{
				elem.m_sRemoteDir = xml.GetChildData();
			}

			if (xml.FindChildElem( _T("sPas")))
			{
				elem.m_sPassword = xml.GetChildData();
				obCrypt.TransformString(sKey, elem.m_sPassword);
			}

			if (xml.FindChildElem( _T("m_nPort")))
			{
				elem.m_nPort = (INTERNET_PORT)_ttol(xml.GetChildData());
			}

			if (xml.FindChildElem( _T("m_bPassive")))
			{
				elem.m_bPassive = _ttol(xml.GetChildData());
			}
			if (xml.FindChildElem( _T("m_bSSL")))
			{
				elem.m_bSSL = _ttol(xml.GetChildData());
			}


			m_aSettings.Add(elem);
		}

		if(xml.FindElem(_T("m_nPos")))
		{
			m_nPos = _ttoi(xml.GetData());
			if(m_nPos > 0 && m_nPos < m_aSettings.GetSize())
			{
				//move last used connection to begin of list
				CFtpConSet elem = m_aSettings.GetAt(m_nPos);
				m_aSettings.RemoveAt(m_nPos);
				m_aSettings.InsertAt(0, elem);
			}
			m_nPos = 0;
		}
	}
	return bRet;	
}

void CConnectFtpDlg::FillList(void)
{
	m_ListBox.ResetContent();
    for(int i=0; i<m_aSettings.GetSize(); i++)
	{
		m_ListBox.InsertString(i, m_aSettings[i].m_sName);
		//m_ListBox.SetItemData(i, &m_aSettings[i]);		
	}
	if(m_aSettings.GetSize())
		m_ListBox.SetCurSel(0);

	m_butEdit.EnableWindow(m_aSettings.GetSize() != 0);
	m_butDelete.EnableWindow(m_aSettings.GetSize() != 0);
}

BOOL CConnectFtpDlg::OnInitDialog()
{
	ReadSettings();
	CDialog::OnInitDialog();
	
	SetWindowText(_T_LNGSTR(27));
	_DLG_ITEM_LANGSTR(28,IDC_CF_CONNECT);
	_DLG_ITEM_LANGSTR(29,IDC_CF_NEW_CONNECT);
	_DLG_ITEM_LANGSTR(30,IDC_CF_EDIT);
	_DLG_ITEM_LANGSTR(31,IDC_CF_DELETE);
	_DLG_ITEM_LANGSTR(32,IDCANCEL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CFtpConSet CConnectFtpDlg::GetSettings(void)
{
	if(m_nPos >= 0 && m_nPos < m_aSettings.GetSize())
		return m_aSettings[m_nPos];
	return CFtpConSet();
}
void CConnectFtpDlg::OnEdit()
{
	m_nPos = m_ListBox.GetCurSel();
	if(m_nPos == LB_ERR)
		m_nPos = 0;

	if(m_nPos >=0 && m_nPos < m_aSettings.GetSize())
	{
		CFtpConSet elem = m_aSettings[m_nPos];
		CFtpConSetDlg dlgSet(this, &elem);
		if(dlgSet.DoModal() == IDOK)
		{
			elem = dlgSet.GetSettings();
			m_aSettings.SetAt(m_nPos, elem);
			FillList();
			m_ListBox.SetCurSel(m_nPos);
		}
	}
	else
	{
		AfxMessageBox(_T_LNGSTR(34));
	}

}

void CConnectFtpDlg::OnBnClickedCfNewConnect()
{
	CFtpConSet elem;
	CFtpConSetDlg dlgSet(this, &elem);
	if(dlgSet.DoModal() == IDOK)
	{
		elem = dlgSet.GetSettings();
		m_aSettings.Add(elem);
		FillList();
		m_ListBox.SetCurSel((int)m_aSettings.GetSize()-1);
	}
}

void CConnectFtpDlg::OnBnClickedCfDelete()
{
	m_nPos = m_ListBox.GetCurSel();
	if(m_nPos == LB_ERR)
		m_nPos = 0;

	if(m_nPos >=0 && m_nPos < m_aSettings.GetSize())
	{
		m_aSettings.RemoveAt(m_nPos--);
		FillList();
		if(m_nPos>=0 && m_nPos<m_ListBox.GetCount())
			m_ListBox.SetCurSel(m_nPos);		
	}
	else
	{
		AfxMessageBox(_T_LNGSTR(33));
	}
}

void CConnectFtpDlg::OnOK()
{
	m_nPos = m_ListBox.GetCurSel();
	SaveSettings();
	if(m_nPos == LB_ERR)
		m_nPos = 0;

	CDialog::OnOK();
}

void CConnectFtpDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CConnectFtpDlg::OnCancel()
{
	SaveSettings();
	CDialog::OnCancel();
}

void CConnectFtpDlg::OnDblclkCfList() 
{
	OnOK();
}
