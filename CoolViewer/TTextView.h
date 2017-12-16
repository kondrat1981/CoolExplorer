#include <vector>
#include <string>
#include <list>
#include "TViewFileBuffer.h"
#pragma once

enum eViewMode
{	
	eEmptyView = 0,
	eTextNormal,
	eBinary,
	eHex
};

enum eTextFileType
{
	eTYPE_UNKNOWN = 0,
	eASCII = 1,
	eUTF_8,
	eUNI16_BE,
	eUNI16_LE
};


class TTextView : public CWnd
{
	DECLARE_DYNAMIC(TTextView)
protected:
	CRect m_rect;
	CRect m_rectView;
	CScrollBar		m_RScrollbar;
	CScrollBar		m_DScrollbar;
	eViewMode m_mode;	 //viewer mode (text, hex, bin)?
	int m_iNumLines;     //number of lines  on screen
	int m_iNumColumns;   //number of columns on screen

//text files begin
	eTextFileType m_texttype; //text file type
	BOOL bWrap;			 //wrap lines in text mode?
	int m_iNumRightScrolled;
//text files end
	int m_iTotalLines;
	int m_prevNumberOfSigns;

//---------------------------------------
	//file//data to bind with File
	BOOL m_bBound;				//bound to file
	void *m_pBoundFile;			//pointer to file
	LONGLONG m_size;            //filesize
	

	int  m_screen_size;

	TViewFileBuffer *m_TBuffer;
//---------------------------------------
protected: 
	 


	// text buffor
	void ReadToBuffer( CDC *pDC );

	//
	CDC				m_MemDC;
	CBitmap			m_Bitmap;
	void RecreateBitmapAndDC();
	void DrawLines(CDC *pDC);
	void PrepareTextBuffer();
	void RecalculateScrollbars(int change = 0);
	void RecalculateRightScrollbar(int change = 0);
	void RecalculateDownScrollbar(int change = 0);

	
public:
	void MoveToNextLine( BOOL bRecalc = FALSE);
	void MoveToPrevLine( BOOL bRecalc = FALSE );

	void MoveToPrevPage();
	void MoveToNextPage();
	
public:
	TTextView();
	virtual ~TTextView();
	
	int SetText(LPCTSTR buffer);
	int SetTextFile(LPCTSTR buffer);
	BOOL SetTextBinding(void *ptr,LONGLONG size);
	void SetMode( eViewMode mode )
	{
		m_mode = mode;
	}
	void AnalyzeMode();
	void RecognizeFileEncoding();
	int GetTotalLines()
	{
		return m_iTotalLines;
	}
	void Resize( int cx, int cy );
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Focus(BOOL bRedraw);
};


