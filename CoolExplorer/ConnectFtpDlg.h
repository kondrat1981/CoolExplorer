#ifndef CONNECT_FTP_DLG_H_INC
#define CONNECT_FTP_DLG_H_INC

// CConnectFtpDlg dialog

class CConnectFtpDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectFtpDlg)

public:
	CConnectFtpDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConnectFtpDlg();

// Dialog Data
	enum { IDD = IDD_CONNECT_FTP };
protected:

	BOOL SaveSettings(void);
	BOOL ReadSettings(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnDblclkCfList();
	afx_msg void OnBnClickedCfConnect();
private:
	CString m_sXmlFile;
	//char a[1024*10*10];
	CFtpConSetArray m_aSettings;
	//char b[1024*10*10];

	int m_nPos;
	CButton m_butDelete;
	CButton m_butEdit;
	CListBox m_ListBox;
public:
	
	void FillList(void);
	afx_msg void OnBnClickedCfNewConnect();
	CFtpConSet GetSettings(void);
	virtual BOOL OnInitDialog();
private:
	
public:
	afx_msg void OnEdit();	
	afx_msg void OnBnClickedCfDelete();
protected:
	
	virtual void OnOK();
public:
	afx_msg void OnBnClickedCancel();
protected:
	virtual void OnCancel();
};

#endif //CONNECT_FTP_DLG_H_INC