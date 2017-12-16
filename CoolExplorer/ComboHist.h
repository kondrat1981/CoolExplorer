#if !defined(AFX_COMBOHIST_H__11855D3B_D36A_47E0_9C54_335AE289DD04__INCLUDED_)
#define AFX_COMBOHIST_H__11855D3B_D36A_47E0_9C54_335AE289DD04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboHist.h : header file
//
#define     COMBO_HIST_FILENAME     "ContHistory.xml"

#include "markup.h"
/////////////////////////////////////////////////////////////////////////////
// CComboHist window

typedef std::list<CString> LISTSTR;

class CComboHist : public CComboBox
{
// Construction
public:
	CComboHist();

// Attributes
public:

    int m_iMaxNumOfHist;        //max number of items in history
	int m_iControllID;          //id of correspondent control
	LISTSTR m_sHistItems;
    static CMarkup m_xml_file;
	void GetWindowText(CString &str,BOOL bAddToHistory);
	void FillControl();			//get data from list and fill control
	void FillStruct();			//get data from control and fill list
	void SetUniqueID(int nID){ ASSERT(m_iControllID==-1);m_iControllID = nID;}

private:
	static CMap<int, int, int, int> m_IDs;
	BOOL CheckUniqueID(int nID);
	BOOL RefreshXmlFile();

	static BOOL m_bRefreshXmlFile;
//	static BOOL m_bSaveXmlFile;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboHist)
	public:
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	
// Implementation
public:
	virtual ~CComboHist();
	BOOL ReadControlHistory();
	BOOL SaveControlHistory();

	void AddItem(CString strItem);
	void AddToHistory( CString strItem );

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboHist)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOHIST_H__11855D3B_D36A_47E0_9C54_335AE289DD04__INCLUDED_)
