#pragma once


#define WM_GRID_EDIT_END	WM_USER+115

// CGridEdit

class CGridEdit : public CEdit
{
	DECLARE_DYNAMIC(CGridEdit)
	CWnd *m_parent;
	int m_Item;
	CString m_OriginalText;
public:
	CGridEdit(CWnd *parent,int nItem);
	virtual ~CGridEdit();
	int GetEditedItem();
	CString GetEditedText();
	CString GetOriginalText(){ return m_OriginalText;}
	void SetEditedText(CString &text);


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


