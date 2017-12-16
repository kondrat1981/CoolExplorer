#ifndef _FTP_CON_SET_D_H_INLCUDED
#define _FTP_CON_SET_D_H_INLCUDED


// CFtpConSetDlg dialog


class CFtpConSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CFtpConSetDlg)

public:
	CFtpConSetDlg(CWnd* pParent = NULL, const CFtpConSet* pSet = NULL);   // standard constructor
	virtual ~CFtpConSetDlg();

// Dialog Data
	enum { IDD = IDD_FTP_CON_SETTINGS };


	CFtpConSet GetSettings() const {return m_recSet;};
protected:
	CFtpConSet m_recSet;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
private:
	BOOL ValidData(void);
protected:
	virtual void OnOK();
};

#endif //_FTP_CON_SET_D_H_INLCUDED