#if !defined(AFX_CHANGEVOLUMELABELDLG_H__25A8F95F_057A_4FC0_B363_438919AA7F8E__INCLUDED_)
#define AFX_CHANGEVOLUMELABELDLG_H__25A8F95F_057A_4FC0_B363_438919AA7F8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangeVolumeLabelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChangeVolumeLabelDlg dialog

class CChangeVolumeLabelDlg : public CDialog
{
	CString m_Drive;
	CString m_sNewLabel;
// Construction
public:
	CChangeVolumeLabelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangeVolumeLabelDlg)
	enum { IDD = IDD_VOLUME_LABEL };
	CEdit	m_EditVolumeLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangeVolumeLabelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangeVolumeLabelDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void SetDrive(LPCTSTR sDrive);
	CString GetDrive();
	CString GetNewLabel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEVOLUMELABELDLG_H__25A8F95F_057A_4FC0_B363_438919AA7F8E__INCLUDED_)
