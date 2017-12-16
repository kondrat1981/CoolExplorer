// DriveCombo.cpp : implementation file
//

#include "stdafx.h"
#include "coolexplorer.h"
#include "DriveCombo.h"
#include "Shell.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDriveCombo

CDriveCombo::CDriveCombo()
{
	m_strPrevDriveValue = _T("");
}

CDriveCombo::~CDriveCombo()
{
}

BOOL CDriveCombo::RefreshDrives()
{
	RefreshSystemDrives();

	UpdateDriveCombo();

return TRUE;
}

BOOL CDriveCombo::UpdateDriveCombo()
{
	this->ResetContent( );
	
	for (int i = 0; i < (int)m_Drives.size(); i++)
	{
		this->AddString(m_Drives[i].strDrive);
	}
	
	return TRUE;
}

void CDriveCombo::RefreshSystemDrives()
{
	_DriveItem tmpItem;
    DWORD nBufferLength = 100;
    TCHAR *s = new TCHAR [nBufferLength];
    TCHAR *s_start = s;
    // TODO: Add extra initialization here
    m_Drives.clear();
    eDriveType eDType = eNone;
    
    //	GetLogicalDriveStrings( 0, drives );
    GetLogicalDriveStrings( nBufferLength, s );
    
    BOOL bEnd = FALSE;
    CString sDrive;
    CString sDriveType;
    
    int i = 0;
    while ( !bEnd )
    {
        tmpItem.eType = eNone;
        sDrive = _T("");
        sDriveType = _T("");
        if (*s == '\0')
        {
            bEnd = TRUE;
            
        }
        else
        {
//			tmpItem.Clear();
            tmpItem.strDrive = s;
            int iDriveType = GetDriveType( s );
            
            switch(iDriveType)
            {
                
            case DRIVE_NO_ROOT_DIR: 
                break;
                
            case DRIVE_REMOVABLE: 
                tmpItem.eType = eREMOVABLE;
				tmpItem.strLabel = _T("Removable");
                break;
                
            case DRIVE_FIXED: 
                tmpItem.eType = eFIXED;
                break;
                
            case DRIVE_REMOTE:
                tmpItem.eType = eREMOTE;
                break;
                
            case DRIVE_CDROM: 
                tmpItem.eType = eCDROM;
				tmpItem.strLabel = _T("CDROM");
                break;
                
            case DRIVE_RAMDISK: 
                break;
                //ExtractShellIcon				
            case DRIVE_UNKNOWN: 
            default:
                
                break;
            }
			m_Drives.push_back(tmpItem);
       //     AddString(sDrive);
//            AddDrive( s, eDType,"");
            s += tmpItem.strDrive.GetLength() + 1;
            i++;
        }
        
        
    }
    s = s_start;
    
    delete []s;


	// add desktop
	tmpItem.strDrive = _T_LNGSTR(9);
	tmpItem.strLabel = _T_LNGSTR(9);
	tmpItem.eType = eDesktop;

	m_Drives.push_back(tmpItem);
	// add desktop
	tmpItem.strDrive = _T_LNGSTR(126);
	tmpItem.strLabel = _T_LNGSTR(126);
	tmpItem.eType = eDesktop;

	m_Drives.push_back(tmpItem);

}


BEGIN_MESSAGE_MAP(CDriveCombo, CComboBox)
	//{{AFX_MSG_MAP(CDriveCombo)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDriveCombo message handlers

void CDriveCombo::OnSelchange() 
{
/*	CString str = _T("");
	if (GetParent())
	{
		
		GetLBText( GetCurSel(), str);
		if (!(str.IsEmpty())&&(str != m_strPrevDriveValue))
		{
			((CPathBar*)GetParent())->ComboChangedDrive(str);

		}
	}
	
*/	
}

void CDriveCombo::OnDropdown() 
{


	
}

void CDriveCombo::SetActiveDrive()
{
	int item = FindString( -1, m_strPrevDriveValue );

	if (item >= 0)
	{
		SetCurSel(item);
	}

}

void CDriveCombo::SetDriveFromPath(const CString &strPath)
{
	CString sPath = strPath;
	CString sDrive = _T("");

	sPath.MakeUpper();

	if (!sPath.IsEmpty())
	{
		for (int i = 0; i < (int)m_Drives.size(); i++)
		{
			if (sPath.Find(m_Drives[i].strDrive) != -1)
			{
				
				sDrive = m_Drives[i].strDrive;
				break;
			}
		}
		if (!sDrive.IsEmpty())
		{
			int item = FindString( -1, sDrive );
			
			if (item >= 0)
			{
				SetCurSel(item);
				m_strPrevDriveValue = sDrive;
			}
		}
		else
		{
			if ((sPath[0] == '\\')&&(sPath[1] == '\\'))
			{
				//network
			int item = FindString( -1, _T_LNGSTR(126) );
			
			if (item >= 0)
			{
				SetCurSel(item);
				m_strPrevDriveValue = sDrive;
			}

			}
		}

	}

}

void CDriveCombo::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CComboBox::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CDriveCombo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
		//closing dropdown after first key if not up and down keys
	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);

	if ((nChar !=VK_UP) && (nChar !=VK_DOWN) &&(GetDroppedState()))
	{
		ShowDropDown(FALSE);
	}


}

void CDriveCombo::ShowDropDownWindow()
{
	ShowDropDown(TRUE);
}

void CDriveCombo::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	RefreshDrives();
	CPathBar* PathBar = (CPathBar*)this->GetParent();
	if (PathBar)
	{
		CString sPath = PathBar->m_sPath;
		if (!sPath.IsEmpty())
		{
			SetDriveFromPath(sPath);
		}
	}
	CComboBox::PreSubclassWindow();
}


BOOL CDriveCombo::PreTranslateMessage(MSG* pMsg) 
{
	
	return CComboBox::PreTranslateMessage(pMsg);
}

void CDriveCombo::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	
	CComboBox::OnRButtonDown(nFlags, point);
}

void CDriveCombo::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CString str;
	GetWindowText(str);
	if ((GetDriveType(str) == DRIVE_FIXED)||
		(GetDriveType(str) == DRIVE_REMOVABLE)||
		(GetDriveType(str) == DRIVE_REMOTE)||
		(GetDriveType(str) == DRIVE_CDROM))
	{
		CStringList list;
		list.AddTail(str);
		if(IsWindow(GetParentFrame()->GetSafeHwnd()) &&IsWindow(GetParentFrame()->GetActiveView()->GetSafeHwnd()) )
		{
			CShell::DisplayExplorerMenu (GetParentFrame()->GetActiveView()->GetSafeHwnd(), _T(""), list, point);
		}
		

	}
}
