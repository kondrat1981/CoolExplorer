#if !defined(AFX_DRIVECOMBO_H__65499D0A_2141_450D_900E_37C8E6E7D527__INCLUDED_)
#define AFX_DRIVECOMBO_H__65499D0A_2141_450D_900E_37C8E6E7D527__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DriveCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDriveCombo window
enum eDriveType
{
	eNone = 0,
	eFIXED,
	eCDROM,
	eREMOVABLE,
	eREMOTE,
	eFTP,
	eNetResource,
	eDesktop

};

class _DriveItem
{
public:
	CString strDrive;
	eDriveType eType;
	CString strLabel;
};

class CDriveCombo : public CComboBox
{
private:
	std::vector <_DriveItem> m_Drives;
// Construction
public:
	CDriveCombo();

// Attributes
public:
	CString m_strPrevDriveValue;

// Operations
public:

private:
	void RefreshSystemDrives();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDriveCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDriveCombo();
	BOOL RefreshDrives();
	BOOL UpdateDriveCombo();
	void SetActiveDrive();
	void SetDriveFromPath(const CString &strPath);
	void ShowDropDownWindow();
	// Generated message map functions
protected:
	//{{AFX_MSG(CDriveCombo)
	afx_msg void OnSelchange();
	afx_msg void OnDropdown();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRIVECOMBO_H__65499D0A_2141_450D_900E_37C8E6E7D527__INCLUDED_)
