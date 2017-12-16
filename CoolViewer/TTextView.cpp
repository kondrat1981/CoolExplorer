// TTextView.cpp : implementation file
//

#include "stdafx.h"
#include "CoolViewer.h"
#include "TTextView.h"


// TTextView

IMPLEMENT_DYNAMIC(TTextView, CWnd)

TTextView::TTextView()
{
	m_bBound = FALSE;
	m_pBoundFile = FALSE;
	m_size = 0;

	bWrap = FALSE;

	m_prevNumberOfSigns = 0;
	m_iNumRightScrolled = 0; 
	


}

TTextView::~TTextView()
{

}


BEGIN_MESSAGE_MAP(TTextView, CWnd)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// TTextView message handlers



void TTextView::OnPaint()
{
	CPaintDC dc(this);

	if((HDC)m_MemDC==NULL)
		RecreateBitmapAndDC();
	
	m_MemDC.FillSolidRect(&m_rectView, RGB(255,255,255));

	if (m_mode == eTextNormal)
	{
		DrawLines(&m_MemDC);
	}
	dc.BitBlt(m_rectView.left,m_rectView.top,m_rectView.right - m_rectView.left,m_rectView.bottom - m_rectView.top,&m_MemDC,m_rectView.left,m_rectView.top,SRCCOPY);

}
void TTextView::DrawLines(CDC *pDC)
{
	if (m_bBound)
	{
		CString s ;
		int sign = 0;
		BOOL bNext = FALSE;

		if (m_screen_size == 0) //check, how meny lines can we draw
		{
		
			//max lines to draw
			TEXTMETRIC Metrics;
			if (m_MemDC)
			{	
				m_MemDC.GetTextMetrics(&Metrics);
				m_iNumLines = (m_rectView.Height() / Metrics.tmHeight);
				m_iNumColumns = (m_rectView.Width() / Metrics.tmAveCharWidth );
			}
			
			//current lines to draw
			m_screen_size = m_iNumLines;


		}
		int iColumn = 0;

		//predict number of signs to draw
/*
		for (int i = 0; i < m_screen_size ; i++) //number of lines to draw
		{
			iColumn = 0;
			bNext = FALSE;


			while (!bNext)
			{
				char t = ((char*)m_screen)[sign];
				switch(t)
				{
				case 9: //TAB
					s += "    ";
					break; 
				case 13: //windows[-CL-]
					{
						if (((char*)m_screen)[sign+1] == 10)//[-RF-]
						{
							bNext = TRUE;

						}
						sign++;
					}
					break;
				case 10: //unixs[-RF-]
					{
						bNext = TRUE;
					}
				break;
				default:
					s += t;
					break;
				}
				
				iColumn++;
				sign++;
				if (((char*)m_screen+sign) >= m_ptrbuffer + m_BufferUsed)
				{
					bNext = TRUE;
					i = m_screen_size;
				}
			}
			s+= "\n";
		}*/
		
		pDC->DrawText(s,s.GetLength(),m_rectView,DT_LEFT);
		//m_prevNumberOfSigns = sign;
	}


}

BOOL TTextView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;//CWnd::OnEraseBkgnd(pDC);
}

void TTextView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonDblClk(nFlags, point);
}

BOOL TTextView::Create(  DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	CString strMyClass;
	CString strWindowName = _T("CoolViewer");
	
	try
	{
		strMyClass = AfxRegisterWndClass(
			CS_VREDRAW | CS_HREDRAW|CS_DBLCLKS,
			::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH) ::GetStockObject(LTGRAY_BRUSH),
			::LoadIcon(NULL, IDI_APPLICATION));
	}
	catch (CResourceException* pEx)
	{
		pEx->Delete();
	}

	m_rect = rect;
	m_rectView = m_rect;
	m_rectView.right -= 15;
	m_rectView.bottom -= 15;

	if (CWnd::Create(strMyClass, strWindowName, dwStyle, rect, pParentWnd, nID, pContext))
	{
		//Right scrollbar
		RECT scr_rect;
		scr_rect.right = m_rect.right;
		scr_rect.left = m_rect.right - 15;
		scr_rect.top = 0;
		scr_rect.bottom = m_rect.bottom - m_rect.top - 15;

		m_RScrollbar.Create(SBS_VERT,scr_rect,this,1113);
		m_RScrollbar.ShowWindow(SW_SHOW);
		
		//down scrollbar
		scr_rect.right = m_rect.right - 15;
		scr_rect.left = m_rect.left;
		scr_rect.top = m_rect.bottom - 15;
		scr_rect.bottom = m_rect.bottom;

		m_DScrollbar.Create(SBS_HORZ,scr_rect,this,1114);
		m_DScrollbar.ShowWindow(SW_SHOW);

			//max lines to draw
			TEXTMETRIC Metrics;
			if (m_MemDC)
			{	
				m_MemDC.GetTextMetrics(&Metrics);
				m_iNumLines = (m_rectView.Height() / Metrics.tmHeight);
				m_iNumColumns = (m_rectView.Width() / Metrics.tmAveCharWidth );
			}
			

		return TRUE;
	}
	return FALSE;

}

void TTextView::RecreateBitmapAndDC()
{
	m_MemDC.DeleteDC();
	m_Bitmap.DeleteObject();

	CDC * pDC = GetDC();
	m_MemDC.CreateCompatibleDC(pDC);
		
	m_Bitmap.CreateCompatibleBitmap(pDC,m_rectView.Width(),m_rectView.Height());
	ReleaseDC(pDC);
	m_MemDC.SelectObject(&m_Bitmap);
	m_MemDC.SetBkMode(TRANSPARENT);

	//calculate number of signs possible to draw
	TEXTMETRIC Metrics;
	m_MemDC.GetTextMetrics(&Metrics);
	m_iNumLines = (m_rectView.Height() / Metrics.tmHeight);
	m_iNumColumns = (m_rectView.Width() / Metrics.tmAveCharWidth );

	RecalculateScrollbars();

}

void TTextView::RecalculateRightScrollbar(int change)
{
	if (::IsWindow(m_RScrollbar.GetSafeHwnd()))
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL|SIF_PAGE|SIF_POS | SIF_TRACKPOS | SIF_RANGE;
		m_RScrollbar.GetScrollInfo(&si);

		// set info for scrollbar
		si.nMin = 0;
		si.nMax = m_iTotalLines-m_iNumLines;
		if (si.nMax < 0)
			si.nMax = 1;
		si.nPage = 1;
		int nCurSel = 0;
		if (nCurSel == LB_ERR || nCurSel < 0)
			nCurSel = 0;
		si.nPos += change;

		if (si.nPos < 0)
			si.nPos = 0;
		m_RScrollbar.SetScrollInfo(&si);
		m_RScrollbar.SetScrollPos(si.nPos, TRUE);
	}
}

void TTextView::RecalculateDownScrollbar(int change)
{
	if (::IsWindow(m_DScrollbar.GetSafeHwnd()))
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL|SIF_PAGE|SIF_POS | SIF_TRACKPOS | SIF_RANGE;
		m_DScrollbar.GetScrollInfo(&si);

		// set info for scrollbar
		si.nMin = 0;
		si.nMax = 0;
		if (si.nMax < 0)
			si.nMax = 1;
		si.nPage = 5;
		int nCurSel = 0;
		if (nCurSel == LB_ERR || nCurSel < 0)
			nCurSel = 0;
		si.nPos = nCurSel;

		if (si.nPos < 0)
			si.nPos = 0;
		m_DScrollbar.SetScrollInfo(&si);
		m_DScrollbar.SetScrollPos(si.nPos, TRUE);
	}
}

void TTextView::RecalculateScrollbars(int change)
{
	RecalculateRightScrollbar();
	RecalculateDownScrollbar();
}

int TTextView::SetText(LPCTSTR buffer)
{
	return 0;
}

int TTextView::SetTextFile(LPCTSTR buffer)
{
	return 0;
}

BOOL TTextView::SetTextBinding(void *ptr,LONGLONG size)
{
	BOOL bRet = FALSE;
	ASSERT(ptr);

	if (ptr != NULL)
	{
		m_TBuffer = new TViewFileBuffer();
		m_TBuffer->BoundFilePtr(ptr);
/*		m_pBoundFile = ptr;
		m_size = size;
		bRet = TRUE;
		m_bBound = TRUE;
		PrepareTextBuffer();
		//load data to buffer
		int i = 0;
		m_BufferUsed = i;
		//set pointers
		m_buffer_to_ptrfile = (char*)ptr; //file
		m_screen = m_ptrbuffer; 	//we will draw from
		m_screen_size = 0;				//we will draw 0 bytes for now*/
		RecognizeFileEncoding();
		AnalyzeMode();
	}
	return bRet;
}
void TTextView::ReadToBuffer(CDC *pDC)
{


	
   
}
void TTextView::Resize( int cx, int cy )
{

	BOOL bSizeChanged = m_rectView.Width() != cx -15|| m_rectView.Height() != cy -15;
	
	/*m_WholeRect.bottom = cy;
	m_WholeRect.right = cx;*/

	if(bSizeChanged)
	{
		m_rect.bottom = cy;
		m_rect.right = cx ;
		m_rectView.bottom = cy - 15;
		m_rectView.right = cx -15;
		RecreateBitmapAndDC();
		MoveWindow(&m_rect,TRUE);
//move scrollbars
		RECT scr_rect;
		scr_rect.right = m_rect.right;
		scr_rect.left = m_rect.right - 15;
		scr_rect.top = 0;
		scr_rect.bottom = m_rect.bottom - m_rect.top - 15;
		//scr_rect.left = scr_rect.right - 10;

		m_RScrollbar.MoveWindow(&scr_rect);
		
		//down scrollbar


		scr_rect.right = m_rect.right - 15;
		scr_rect.left = m_rect.left;
		scr_rect.top = m_rect.bottom - 15;
		scr_rect.bottom = m_rect.bottom;

		m_DScrollbar.MoveWindow(&scr_rect);

	}


	

}
void TTextView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

void TTextView::PrepareTextBuffer()
{

}

/*
UTF-16 LE (Little Endian). Used for Windows operating systems. Typically called "Unicode".
Signature = 2 bytes: 0xFF 0xFE
followed by 2 byte pairs. xx 00 xx 00 xx 00 for normal 0-127 ASCII chars.

UTF-16 BE (Big Endian). This is used for Macintosh operating systems.
Signature = 2 bytes: 0xFE 0xFF
followed by 2 byte pairs. 00 xx 00 xx 00 xx for normal 0-127 ASCII chars.
ie. So same as Windows UTF-16 LE but the word bytes are flipped.

UTF-8
Signature = 3 bytes: 0xEF 0xBB 0xBF
UTF-8 is the 8-bit form of Unicode.
*/
void TTextView::RecognizeFileEncoding()
{
	if (m_pBoundFile == NULL || m_size == 0 )
		return;

	BOOL bFound = FALSE;
	char id[4] = {0};
	const unsigned char utf8[4] = {0xEF,0xBB,0xBF,0x00};
	const unsigned char utf16be[3] = {0xFE,0xFF,0x00};
	unsigned char utf16le[3] = {0xFF,0xFE,0x00};

	if (m_size >= 2) //more than 2 bytes
	{
		if (m_size >= 3) //more than 3 bytes
		{
			//check if its UTF-8 
			//m_texttype = eUTF_8;
			memcpy((void *)id, m_pBoundFile,sizeof(char)*3);
			if(!(strcmp((char*)id,(char*)utf8)))
			{
				m_texttype = eUTF_8;
				bFound = TRUE;
			}
		}
		
		if (!bFound)
		{
			//UTF-16 BE or UTF-16 LE
			//m_texttype = eUNI16_BE ;
			memset((void *)id,'\0',sizeof(char)*3);
			memcpy((void *)id, m_pBoundFile,sizeof(char)*2);

			if(!(strcmp((char*)id,(char*)utf16be)))
			{
				m_texttype = eUNI16_BE;
				bFound = TRUE;
			}
			else if(!(strcmp((char*)id,(char*)utf16le)))
			{
				m_texttype = eUNI16_LE;
				bFound = TRUE;
			}
			
		}
	}

	if (!bFound)
	{
		//normal text
		m_texttype = eASCII;
	}

}

void TTextView::AnalyzeMode()
{
	if (!m_bBound)
		return;
	


	switch(m_mode)
	{
	case eTextNormal:
		{
			//analyze text file type


			//total number of rows, used
			m_iTotalLines = 0;
			for (LONGLONG i = 0; i < m_size ; i++)
			{
				if (((char*)m_pBoundFile)[i] == 10)
				{
					m_iTotalLines ++;
				}
			}
		}
		break;
	case eHex:

		break;
	case eBinary:
		break;

	}
	//set sliders
	RecalculateScrollbars();

}


void TTextView::MoveToNextLine( BOOL bRecalc )
{

	if (bRecalc)
	{
		RedrawWindow();
		RecalculateScrollbars();
	}

}
void TTextView::MoveToPrevLine( BOOL bRecalc )
{


	if (bRecalc)
	{
		RedrawWindow();
		RecalculateScrollbars();
	}

}

void TTextView::MoveToNextPage()
{
	for (int i = 0; i < m_iNumLines ; i++)
	{
		MoveToNextLine();
		RecalculateScrollbars(1);
	}
	RedrawWindow();
	
}

void TTextView::MoveToPrevPage()
{
	for (int i = 0; i < m_iNumLines ; i++)
	{
		MoveToPrevLine();
		RecalculateScrollbars(-1);
	}
	RedrawWindow();
	

}
void TTextView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (!(::IsWindow(pScrollBar->GetSafeHwnd())))
	{
		return;
	}
	SCROLLINFO info;

	info.fMask = SIF_ALL | SIF_DISABLENOSCROLL; 
	info.cbSize = sizeof(SCROLLINFO);


	pScrollBar->GetScrollInfo(&info);
	
	if ((info.nPos < info.nMin)||(info.nPos > info.nMax))
		return;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		{
			
			if (info.nPos < info.nMax)
			{
				MoveToNextLine();
				info.nPos += 1;
			}
		}
		break;

	case SB_LINEUP:
		{
			MoveToPrevLine();
			if (info.nPos > 0)
			{
				info.nPos -=1;
			}

		}
		break;
	case SB_PAGEUP:
		{
			MoveToPrevPage();
			/*if (info.nPos > 0)
			{
				info.nPos -=1;
			}*/
		}
		break;
	case SB_PAGEDOWN:
		{
			MoveToNextPage();
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:

	break;
	}


	pScrollBar->SetScrollInfo(&info);
	
	RedrawWindow();

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void TTextView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (!(::IsWindow(pScrollBar->GetSafeHwnd())))
	{
		return;
	}
	SCROLLINFO info;

	info.fMask = SIF_ALL | SIF_DISABLENOSCROLL; 
	info.cbSize = sizeof(SCROLLINFO);


	pScrollBar->GetScrollInfo(&info);
	
	if ((info.nPos < info.nMin)||(info.nPos > info.nMax))
		return;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		m_iNumRightScrolled++;

		break;
	case SB_LINEUP:
		if (m_iNumRightScrolled >0)
		m_iNumRightScrolled--;
		break;

	case SB_PAGEUP:
		break;
	case SB_PAGEDOWN:
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
	break;
	}


	pScrollBar->SetScrollInfo(&info);
	
	RedrawWindow();

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void TTextView::Focus(BOOL bRedraw)
{
	ASSERT(::IsWindow(m_hWnd));

	CWnd::FromHandle(::SetFocus(m_hWnd));

	if (bRedraw)
	{
		RedrawWindow();
	}
}

BOOL TTextView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::PreTranslateMessage(pMsg);
}
