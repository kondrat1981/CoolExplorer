// ComboHist.cpp : implementation file
//

#include "stdafx.h"
#include "UnicodeFile.h"
//#include "winexplorer.h"
#include "ComboHist.h"
#include ".\combohist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboHist

CMarkup CComboHist::m_xml_file;
BOOL CComboHist::m_bRefreshXmlFile;
CMap<int, int,int,int> CComboHist::m_IDs;
extern CString gsFilesPath;

CComboHist::CComboHist()
{
	m_iControllID = -1; 
    m_iMaxNumOfHist = 10;
	m_bRefreshXmlFile = TRUE;

}

CComboHist::~CComboHist()
{
	SaveControlHistory();

}


BEGIN_MESSAGE_MAP(CComboHist, CComboBox)
	//{{AFX_MSG_MAP(CComboHist)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboHist message handlers



void CComboHist::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
//	ModifyStyle(0,CBS_AUTOHSCROLL);
	CComboBox::PreSubclassWindow();
	

	if(m_iControllID==-1)
		SetUniqueID(GetDlgCtrlID());

#ifdef _DEBUG
	CheckUniqueID(m_iControllID);
	m_IDs.SetAt(m_iControllID, m_iControllID);
#endif

//  read history for dialog
	ReadControlHistory();

	//fill control with history
	FillControl();

}

void CComboHist::FillControl()
{
	LISTSTR::iterator i;
	CString strOldValue = _T("");

	CComboBox::GetWindowText(strOldValue);

	ResetContent();
	int n = 0;
	for (i = m_sHistItems.begin(); i != m_sHistItems.end(); i++)
	{
		//AddString(*i);
		InsertString(n++,*i);
	}
	CComboBox::SetWindowText(strOldValue);
}

void CComboHist::AddItem(CString strItem)
{
	if (strItem != _T(""))
	{
		m_sHistItems.push_front(strItem);
//		AddString( strItem );
	}

}
void CComboHist::AddToHistory( CString strItem )
{
	if (strItem != _T(""))
	{
		// check, if element is on the list
		BOOL bIsElement = FALSE;
		LISTSTR::iterator i;

		for (i = m_sHistItems.begin(); i != m_sHistItems.end(); i++)
		{
			if (*i == strItem)
			{
				m_sHistItems.erase(i);
				bIsElement = TRUE;
				break;
			}
		}

		//add to control history stored
		if (m_sHistItems.size() == m_iMaxNumOfHist)
		{
			//remove last item from back
			m_sHistItems.pop_back();

		}
		//add new item to front
		AddItem(strItem);
		if (!bIsElement)
		{
			AddString( strItem );
		}
		SetCurSel(FindStringExact(-1, strItem));
	}
}


BOOL CComboHist::ReadControlHistory()
{
    BOOL bResult = FALSE;
	CString stmp = _T("");

	if (m_bRefreshXmlFile)
	{
		if (RefreshXmlFile())
		{
			m_bRefreshXmlFile = FALSE;
		}
	}

	try
	{
        m_xml_file.ResetPos();

        bResult = m_xml_file.FindElem(_T("ControlsHistory"));

        if (bResult)
        {
            CString strTmp = _T("");
            //find our control ID tag
            strTmp.Format(_T("C%d"),m_iControllID);
            
            bResult = m_xml_file.FindChildElem(strTmp);

            if (bResult)
            {
				//find all subitems
				m_xml_file.IntoElem();
				m_sHistItems.erase(m_sHistItems.begin(),m_sHistItems.end());
				
					
				while (m_xml_file.FindChildElem(_T("ITEM")))
				{
					stmp = m_xml_file.GetChildData();
					m_sHistItems.push_back(stmp);
				}
				
				m_xml_file.OutOfElem();

            }
        }
	}
	catch (...)
	{
	}
    FillControl();
	return bResult;
}


BOOL CComboHist::SaveControlHistory()
{
    BOOL bResult = FALSE;

	CString strAttrData = m_xml_file.GetDoc();

	bResult = CUnicodeFile::WriteToFile(AddPathNames(gsFilesPath, _T(COMBO_HIST_FILENAME)), strAttrData);

	return bResult;
}


BOOL CComboHist::RefreshXmlFile()
{
	BOOL bResult = FALSE;
	CString strAttrData = _T("");
	
	bResult = CUnicodeFile::ReadFromFile(AddPathNames(gsFilesPath, _T(COMBO_HIST_FILENAME)), strAttrData);
	
	if (bResult)//file is loaded
	{
		bResult = m_xml_file.SetDoc(strAttrData);
	}
	else
	{
		strAttrData=_T("<ControlsHistory></ControlsHistory>");
		bResult = m_xml_file.SetDoc(strAttrData);
	}
	
	return bResult;
}

void CComboHist::OnDestroy() 
{
	ASSERT(m_IDs.RemoveKey(m_iControllID));

	CComboBox::OnDestroy();
	
	//save data from controll to xml file
	LISTSTR::iterator i;
	BOOL bResult = FALSE;
	try
	{
        m_xml_file.ResetPos();

        bResult = m_xml_file.FindElem(_T("ControlsHistory"));

        if (bResult)
        {
            CString strTmp = _T("");
            //find our control ID tag
            strTmp.Format(_T("C%d"),m_iControllID);
            
            bResult = m_xml_file.FindChildElem(strTmp);

            if (bResult)  //there is control id inside
            {
				//find all subitems and erase them
				m_xml_file.IntoElem();
					
				while (m_xml_file.FindChildElem(_T("ITEM")))
				{
					m_xml_file.RemoveChildElem();
				}

			}
			else
			{
				//there is no control id inside
				//add tag
				bResult = m_xml_file.AddChildElem( strTmp );
				m_xml_file.IntoElem();

            }
			//add items from inside vector
				for (i = m_sHistItems.begin(); i != m_sHistItems.end(); i++)
				{
					m_xml_file.AddChildElem(_T("ITEM"),*i);
				}

				m_xml_file.OutOfElem();

            }
        }
	catch (...)
	{
	}

}

void CComboHist::GetWindowText(CString &str,BOOL bAddToHistory)
{
	CComboBox::GetWindowText(str);

	if ((str != _T(""))&&(bAddToHistory))
	{

		// check, if element is on the list
		BOOL bIsElement = FALSE;
		LISTSTR::iterator i;

		for (i = m_sHistItems.begin(); i != m_sHistItems.end(); i++)
		{
			if (*i == str)
			{
				m_sHistItems.erase(i);
				break;
			}
		}
		

			//add to control history stored
			if (m_sHistItems.size() == m_iMaxNumOfHist)
			{
				//remove last item from back
				m_sHistItems.pop_back();
				
			}
			//add new item to front
			AddItem(str);
			//m_sHistItems.push_front(str);
			FillControl();
		}

}

HBRUSH CComboHist::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


BOOL CComboHist::CheckUniqueID(int nID)
{
	BOOL bRet = TRUE;
	int i=0;
	if(m_IDs.Lookup(nID,i))
	{
		ASSERT(0);
		AfxMessageBox(_T("CComboHist ID is duplicated"));
	}

	return bRet;
}
