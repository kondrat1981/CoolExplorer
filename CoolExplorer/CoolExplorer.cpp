// CoolExplorer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CoolExplorer.h"

#include "MainFrm.h"
#include "CoolExplorerDoc.h"
#include "CoolExplorerView.h"
#include "UnicodeFile.h"
#include "ListData.h"
#include "afxcmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerApp

BEGIN_MESSAGE_MAP(CCoolExplorerApp, CWinApp)
	//{{AFX_MSG_MAP(CCoolExplorerApp)
	ON_COMMAND(ID_HELP_HOMEPAGE, OnAppAbout)
	ON_COMMAND(ID_HELP_ONLINEDOCUMENTATION, OnAppAbout)
	ON_COMMAND(ID_HELP_CHECKFORNEWVERSION, OnAppAbout)
	ON_COMMAND(ID_HELP_SENDPROBLEMS, OnAppAbout)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_F1, OnAppAbout)

		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_CONFIGURATION_SETTINGS, OnAppConfiguration)
	// Standard print setup command
	//ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerApp construction

extern CString gsFilesPath;
extern CString gsStartPath;
extern LPCTSTR gsConfigFileName;

CCoolExplorerApp::CCoolExplorerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CCoolExplorerApp::~CCoolExplorerApp()
{
	if (m_configuration!= NULL)
	{
		delete m_configuration;
		m_configuration = NULL;
	}


}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCoolExplorerApp object

CCoolExplorerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerApp initialization
void CCoolExplorerApp::ReplaceStrings()
{

}

BOOL CCoolExplorerApp::InitInstance()
{	
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	TCHAR sTmp[MAX_PATH*4] = {0};
	GetCurrentDirectory(sizeof(sTmp)-1, sTmp);
	gsStartPath = gsFilesPath = sTmp;


	//if config file exist in the start directory - use current directory for settings files - otherwise use AppData directory
	HANDLE hFile1 = CreateFile(AddPathNames(gsStartPath, gsConfigFileName), GENERIC_WRITE, 0, 0, OPEN_EXISTING,0,NULL);
	if(hFile1 != INVALID_HANDLE_VALUE)
		CloseHandle(hFile1);
	else
	{
		TCHAR szPath[MAX_PATH*4] = {0};

		if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA |CSIDL_FLAG_CREATE, NULL, 0, szPath)))
		{
			CString sPath = AddPathNames(szPath, _T("CoolExplorer"));
			if(ExistsPath(sPath) || MyCreateDirectory(sPath, NULL))
				gsFilesPath = sPath;
		}
	}


	CoolTrace(0, TRUE, _T("********************CoolExplorer started********************"));

	m_configuration = new CSettings;

	if (m_configuration->bNoLangFiles)
	{
		//AfxMessageBox(_T("No Language Files installed! Exiting CoolExplorer!"));
		//return FALSE;
	}

	gLanguage.SetCurrentLanguage();
	
	AfxEnableControlContainer();
	

	if (!AfxOleInit ())
    {
		AfxMessageBox (_T("Ole init failed"));
		return FALSE;
    }


#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


	
	// load configuration
//	m_configuration.SetConfigFile();

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	HWND hWnd = m_pMainWnd->GetSafeHwnd();


	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCoolExplorerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CCoolExplorerView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	ReplaceStrings();

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


	

	m_bFullFunct = TRUE;

	m_ptr_NetServerEnum_Fun = NULL;
	m_ptr_NetServerDiskEnum_Fun = NULL;
	m_ptr_NetApiBufferFree_Fun = NULL;

	m_winVer = eReturnWinVersion();



	if (( m_winVer > eWinUnknown) && (m_winVer<= eWinMe))
	{
		//lower version 
		m_bFullFunct = FALSE;
	}
	else if (((m_winVer >= eWinNT_3_51) && (m_winVer <= eWinXP))||(m_winVer == eWinUnknown))
	{
		m_bFullFunct = TRUE;
		//try to load NETAPI32.DLL and localize needed functions

	}

	if (m_bFullFunct)
	{
		m_hLibNetApi32 = NULL;

		m_hLibNetApi32 = LoadLibrary(_T("NETAPI32.DLL"));

		if (m_hLibNetApi32)
		{
			m_ptr_NetServerEnum_Fun = (NetServerEnum_Fun) GetProcAddress(m_hLibNetApi32,"NetServerEnum");

			m_ptr_NetServerDiskEnum_Fun = (NetServerDiskEnum_Fun) GetProcAddress(m_hLibNetApi32,"NetServerDiskEnum");

			m_ptr_NetApiBufferFree_Fun = (NetApiBufferFree_Fun) GetProcAddress(m_hLibNetApi32,"NetApiBufferFree");

			m_ptr_NetShareEnum_Fun = (NetShareEnum_Fun) GetProcAddress(m_hLibNetApi32,"NetShareEnum");
			
			if ((m_ptr_NetServerEnum_Fun)&&(m_ptr_NetServerDiskEnum_Fun)
				&&(m_ptr_NetApiBufferFree_Fun)&&(m_ptr_NetShareEnum_Fun))
			{
				m_bFullFunct = TRUE;
			}
		}
		else
		{
			m_bFullFunct = FALSE;
		}
	}
	//set language
	LCID  lc = GetThreadLocale();

	AfxInitRichEdit();
	m_pMainWnd->ShowWindow(SW_HIDE);
	ReloadMainMenuItems();

	
	m_pMainWnd->ShowWindow(SW_SHOW);

	m_pMainWnd->UpdateWindow();
	
	TRACE(_T("System Detected %d, new functions are %d\n"),m_winVer,m_bFullFunct);
	return TRUE;
}

void CCoolExplorerApp::ReloadMainMenuItems()
{
	CMenu *gmenu = m_pMainWnd->GetMenu();
	//gmenu->DestroyMenu();

	int length = gmenu->GetMenuItemCount();

	for (int i = 0; i< length;i++) 
	{

		UINT id = gmenu->GetMenuItemID(i);

		//modify main string
		gmenu->ModifyMenu(i, MF_BYPOSITION|MF_STRING, id, gLanguage.GetMenuItem(i));

		//try to get popup menu and modify content

		CMenu *submenu = gmenu->GetSubMenu(i);

		if (submenu != NULL)
		{
			int iOmmit = 0;
			int count = submenu->GetMenuItemCount();
			for (int j = 0; j < (int)submenu->GetMenuItemCount(); j++)
			{
				CString s;
				MENUITEMINFO mi;
				mi.cbSize = sizeof(MENUITEMINFO); 
				mi.fMask = MIIM_TYPE; 
				mi.cch = 0;  

				submenu->GetMenuItemInfo(j,&mi,TRUE);

				if (!(MFT_SEPARATOR && mi.fType))
				{
					submenu->GetMenuString(j,s,MF_BYPOSITION);

					submenu->ModifyMenu(j, MF_BYPOSITION|MF_STRING, 
						submenu->GetMenuItemID(j), gLanguage.GetMenuItem(i,j));
				}
				else
				{
					iOmmit++;
				}


			}

		}

	}

}
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_sTest;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnStaticCool();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CRichEditCtrl m_help_text;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_sTest = _T("³³óóó¹¹¹œœ");
	//}}AFX_DATA_INIT
}
	
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//	DDX_Text(pDX, IDC_EDIT2, m_sTest);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RICHEDIT_HELP, m_help_text);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_STATIC_COOL, OnStaticCool)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CCoolExplorerApp::OnAppHomePage()
{
}

void CCoolExplorerApp::OnAppOnlineDocumentation()
{
}

void CCoolExplorerApp::OnAppCheckForNewRelease()
{
}

void CCoolExplorerApp::OnAppReportProblem()
{
}


void CCoolExplorerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



/////////////////////////////////////////////////////////////////////////////
// CCoolExplorerApp message handlers


int CCoolExplorerApp::ExitInstance() 
{

	CSettings::SaveConfiguration();	
	
	if (m_hLibNetApi32)
	{
		FreeLibrary(m_hLibNetApi32);
	}

	RemoveTempDir();
	CoolTrace(0, TRUE, _T("********************CoolExplorer exited normally*******************"));

	WriteTraces();

	return CWinApp::ExitInstance();
}

void CCoolExplorerApp::OnAppConfiguration()
 {
	CSettingsMainDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString sLangData;
		CSettings::m_settings = dlg.m_cfg;
		
		
		CSettings settings;

		settings.SaveConfiguration();
		
		BOOL bResult = TRUE;//CSettings::CreateDefaultLanguageFile(settings.m_settings.sLangFile);
		
		if (bResult)
		{// read default file
			CString strExt = AddPathNames(gsFilesPath,_T("lang"));
			//CString strName = AddPathNames(strExt, m_settings.sLangFile);
			CString strName = AddPathNames(strExt, settings.m_settings.sLangFile);
			strName = _T("") + strName + _T("");
			bResult = CUnicodeFile::ReadFromFile(strName ,sLangData);
		}
		if (bResult)//file is loaded
		{
			settings.LoadAndSetLanguageData(sLangData);
		}

		ReloadMainMenuItems();


		((CMainFrame*)AfxGetMainWnd())->RefreshConfiguration();

/*		((CMainFrame*)AfxGetMainWnd())->GetWindowRect(&r);
		((CMainFrame*)AfxGetMainWnd())->CalcWindowRect( &r);
		((CMainFrame*)AfxGetMainWnd())->MoveWindow(&r);*/
		((CMainFrame*)AfxGetMainWnd())->RecalcFrames();
		m_pMainWnd->ShowWindow(SW_HIDE);
		m_pMainWnd->ShowWindow(SW_SHOW);
			


	}

}


BOOL CCoolExplorerApp::PreTranslateMessage(MSG* pMsg) 
{
	
	return CWinApp::PreTranslateMessage(pMsg);
}

void CAboutDlg::OnStaticCool() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//set version
	//set version

	CString ver,newver;

	if (GetDlgItem(IDC_STATIC_VERSION) != NULL) 
	{
		GetDlgItem(IDC_STATIC_VERSION)->GetWindowText(ver);

		newver.Format(ver,COOL_VERSION);

		GetDlgItem(IDC_STATIC_VERSION)->SetWindowText(newver);
	}
	
	CString textHelp;
	
	m_help_text.SetReadOnly(TRUE);

	if (CUnicodeFile::ReadFromFile(_T("readme.rtf"),textHelp))
	{
		m_help_text.SetWindowText(textHelp);
	}
	else
	{
		m_help_text.SetWindowText(_T("Unable to find Readme.rtf file."));

	}

	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
