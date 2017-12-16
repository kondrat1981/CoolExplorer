// FastFileSrchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FastFileSrchDlg.h"
#include "CoolExplorerDoc.h"
#include "CoolExplorerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFastFileSrchDlg dialog


CFastFileSrchDlg::CFastFileSrchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFastFileSrchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFastFileSrchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFastFileSrchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFastFileSrchDlg)
	DDX_Control(pDX, IDC_MASKED, m_FullMaskedCheckbox);
	DDX_Control(pDX, IDC_FASTFILESRCH_EDIT, m_FileNameFind_Edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFastFileSrchDlg, CDialog)
	//{{AFX_MSG_MAP(CFastFileSrchDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEWHEEL()
	ON_EN_KILLFOCUS(IDC_FASTFILESRCH_EDIT, OnKillfocusFastfilesrchEdit)
	ON_EN_CHANGE(IDC_FASTFILESRCH_EDIT, OnChangeFastfilesrchEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastFileSrchDlg message handlers

BOOL CFastFileSrchDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CFastFileSrchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		//MessageBox("key down PreTranslateMessage");
		switch(pMsg->wParam)
		{
            case VK_F1:
                {
                    if (m_FullMaskedCheckbox.IsWindowVisible())
                    {
                        m_FullMaskedCheckbox.SetCheck(!m_FullMaskedCheckbox.GetCheck());
                    }
					ChangeFastfilesrchEdit();
                }
                break;
        
            case VK_ESCAPE:
				{
					//this->ShowWindow(SW_HIDE);
					this->SendMessage(WM_KILLFOCUS);

					m_pView->SetFocus();
				}

                break;
			case VK_RETURN:
				{

					//this->ShowWindow(SW_HIDE);
					this->SendMessage(WM_KILLFOCUS);

					m_pView->SetFocus();
					if ( CSettings::m_settings.bRunOnEnter)
					{
						m_pView->EnterSelected();
					}
				}

                break;
            case VK_DOWN:
                {
                    CString strItem = _T("");
                    m_FileNameFind_Edit.GetWindowText( strItem );
                    
                    if (strItem != _T(""))
                    {
                        if (m_selected_item == m_found_items )
                        {
                            m_selected_item = 1;
                        }
                        else
                        {
                            m_selected_item++;
                        }
                        m_pView->GoToSimilarItem( m_selected_item, strItem );
						m_FileNameFind_Edit.SetSel(strItem.GetLength(),strItem.GetLength());
                    }
                }
                return FALSE;
                break;
            case VK_UP:
                {
                    CString strItem = _T("");
                    m_FileNameFind_Edit.GetWindowText( strItem );
                    
                    if (strItem != _T(""))
                    {
                        if (m_selected_item == 1 )
                        {
                            m_selected_item = m_found_items;
                        }
                        else
                        {
                            m_selected_item--;
                        }
                        m_pView->GoToSimilarItem( m_selected_item, strItem );
						m_FileNameFind_Edit.SetSel(strItem.GetLength(),strItem.GetLength());
					}
                return FALSE;
                }
                break;
            case 191:
                {
                    this->SendMessage(WM_KILLFOCUS);
                    m_pView->SetFocus();
                    return FALSE;
                }
                break;

            default:
    			break;
				return TRUE;
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

void CFastFileSrchDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);



	if (bShow == 1)
	{
        if ( CSettings::m_settings.bFullMasking)
        {
            if ( !m_FullMaskedCheckbox.IsWindowVisible() )
                m_FullMaskedCheckbox.ShowWindow(SW_SHOW);
            //set state from settings
            m_FullMaskedCheckbox.SetCheck(!CSettings::m_settings.bFFSerchMask);

        }
        else
        {
            if ( !m_FullMaskedCheckbox.IsWindowVisible() )
                m_FullMaskedCheckbox.ShowWindow(SW_HIDE);

        }

        RECT rect;
        m_found_items = 0;

       
/*        DWORD dwStyle = GetWindowLong(m_pView->GetListCtrl().GetSafeHwnd(), GWL_STYLE);
        dwStyle |= LVS_SHOWSELALWAYS  ;
        SetWindowLong(m_pView->GetListCtrl().GetSafeHwnd(), GWL_STYLE,dwStyle);
*/		

		m_pView->GetWindowRect(&rect);

        rect.top = rect.bottom;
        rect.bottom+= 30;
		GetParentFrame()->ScreenToClient(&rect);
        		
        this->MoveWindow(&rect,TRUE);

//		SetWindowPos(GetParent()->GetParentFrame(), 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE);
//		SetParent(GetParent()->GetParentFrame());
		//SetWindowPos(&(GetParentFrame()->wndTopMost),rect.left,rect.top,100,100,WS_EX_TOPMOST);
        
        m_FileNameFind_Edit.SetWindowText(_T(""));
		m_FileNameFind_Edit.SetFocus();
	}
    else
    {
/*     DWORD dwStyle = GetWindowLong(m_pView->GetListCtrl().GetSafeHwnd(), GWL_STYLE);
     if ( dwStyle & LVS_SHOWSELALWAYS)
     {
         dwStyle &= ~LVS_SHOWSELALWAYS  ;
         SetWindowLong(m_pView->GetListCtrl().GetSafeHwnd(), GWL_STYLE,dwStyle);
     }

*/
  //      pView->GetListCtrl().SetFocus();
    }
	
	// TODO: Add your message handler code here
	
}

void CFastFileSrchDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
   
// TODO: Add your message handler code here
   this->ShowWindow( SW_HIDE );
}

BOOL CFastFileSrchDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
    return TRUE;
}

void CFastFileSrchDlg::OnKillfocusFastfilesrchEdit() 
{
	// TODO: Add your control notification handler code here
	 this->ShowWindow( SW_HIDE );
}

void CFastFileSrchDlg::OnChangeFastfilesrchEdit() 
{
	ChangeFastfilesrchEdit();
}

void CFastFileSrchDlg::SetPointers( CCoolExplorerView* pView )
{
	m_pView = pView;
}

void CFastFileSrchDlg::ChangeFastfilesrchEdit()
{

		// TODO: Add your control notification handler code here
    CString str = _T("");
    m_selected_item = 0;
    m_FileNameFind_Edit.GetWindowText(str);

    if (str != _T(""))
    {
		if ( str == _T("**"))
		{
			str.Delete(str.GetLength()-1,1);
			m_FileNameFind_Edit.SetWindowText( str );
		}
		else
		{
			
			int i = m_pView->CountSimilarItems( str );
			
			if (i > 0)
			{
				m_found_items = i;
				m_pView->GoToSimilarItem( 1, str );
				m_selected_item = 1;
				
			}
			else
			{
				str.Delete(str.GetLength()-1,1);
				m_FileNameFind_Edit.SetWindowText( str );
			}
        }
        m_FileNameFind_Edit.SetSel(str.GetLength(),str.GetLength());
    
    }

}

BOOL CFastFileSrchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	_DLG_ITEM_LANGSTR(14,IDC_STATIC_SEARCH);
	_DLG_ITEM_LANGSTR(15,IDC_MASKED);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFastFileSrchDlg::PutLetter(TCHAR letter)
{
	m_FileNameFind_Edit.SetWindowText(CString(TCHAR(towlower(letter))));
}