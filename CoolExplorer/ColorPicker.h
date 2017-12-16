#if !defined(AFX_COLORPICKER_H__10A0DBF5_80CE_40B8_BF90_D960EDCF2F23__INCLUDED_)
#define AFX_COLORPICKER_H__10A0DBF5_80CE_40B8_BF90_D960EDCF2F23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorPicker.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorPicker window

class CColorPicker : public CStatic
{
// Construction
public:
	CColorPicker();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorPicker)
	//}}AFX_VIRTUAL

// Implementation
public:
	COLORREF m_rgbColor;
	void SetColor(COLORREF rgb);
    COLORREF GetColor();
	virtual ~CColorPicker();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorPicker)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORPICKER_H__10A0DBF5_80CE_40B8_BF90_D960EDCF2F23__INCLUDED_)
