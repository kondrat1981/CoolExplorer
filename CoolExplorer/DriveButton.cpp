// DriveButton.cpp : implementation file
//

#include "stdafx.h"
#include "CoolExplorer.h"
#include "DriveButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDriveButton
IMPLEMENT_DYNCREATE(CDriveButton, CFlatButton)

CDriveButton::CDriveButton()
{
}

CDriveButton::~CDriveButton()
{
}


BEGIN_MESSAGE_MAP(CDriveButton, CFlatButton)
	//{{AFX_MSG_MAP(CDriveButton)
	ON_WM_CREATE()
//	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
//	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDriveButton message handlers

int CDriveButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFlatButton::OnCreate(lpCreateStruct) == -1)
		return -1;	

//	EnableToolTips(TRUE);
	
	return 0;
}

BOOL CDriveButton::Create(LPCTSTR lpszWindowName, LPCTSTR lpszDriveName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	m_sDriveName = lpszDriveName;
	
	BOOL bRet = CFlatButton::Create(lpszWindowName, WS_CHILD|WS_VISIBLE/*|BS_BITMAP| BS_RIGHT | BS_TEXT */| BS_CHECKBOX /*| BS_PUSHLIKE*/, rect, pParentWnd, nID);

	SHFILEINFO shinfo;
	memset(&shinfo, 0, sizeof(shinfo));
	MySHGetFileInfo( m_sDriveName, FILE_ATTRIBUTE_NORMAL, 
					   &shinfo, 
					   sizeof(shinfo), 
					   SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES );
	m_hIcon = shinfo.hIcon;

	/*SHFILEINFO shinfo;
	MySHGetFileInfo( lpszDriveName, FILE_ATTRIBUTE_NORMAL, 
					   &shinfo, 
					   sizeof(shinfo), 
					   SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES );

	CDC memdc;
	memdc.CreateCompatibleDC(NULL);
	memdc.SetMapMode( MM_TEXT    );

	BYTE * pBuffer = NULL;

	BITMAPINFO DIBinfo;
	BITMAPINFOHEADER& bih = DIBinfo.bmiHeader;
			bih.biSize          = sizeof(BITMAPINFOHEADER);
			bih.biWidth         = rect.right - rect.left;
			bih.biHeight        = rect.bottom - rect.top;
			bih.biPlanes        = 1;                // Must always be 1 according to docs
			bih.biBitCount      = 24;
			bih.biCompression   = BI_RGB;
			bih.biSizeImage     = 0;
			bih.biXPelsPerMeter = 0;
			bih.biYPelsPerMeter = 0;
			bih.biClrUsed       = 0;
			bih.biClrImportant  = 0;

	HBITMAP hBmp = ::CreateDIBSection(memdc.GetSafeHdc(),&DIBinfo,DIB_RGB_COLORS,(void **)&pBuffer,NULL,0);//CreateBitmapIndirect(&bm);
	
	memdc.SelectObject(hBmp);
	
	memdc.SelectObject(GetSysColorBrush(COLOR_BTNFACE));
	memdc.Rectangle(-1,-1,rect.right - rect.left + 1,rect.bottom - rect.top + 1);
	DrawIconEx(memdc.GetSafeHdc(),8,2,shinfo.hIcon,0,0,0,0,DI_NORMAL);
	memdc.SetBkMode(TRANSPARENT);

	CString str = lpszWindowName;
	str.MakeLower();
	if(str[1] ==':')
		str = str.Left(1);

	memdc.TextOut(24,2,str);

	SetBitmap(hBmp);*/

	return bRet;
}



CString CDriveButton::GetDriveName()
{
	return m_sDriveName;
}

void CDriveButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	BOOL bPressed = m_bSelected;//(lpDIS->itemState & ODS_SELECTED);
	COLORREF rgbBkg = bPressed ? RGB(210, 210, 210) : GetSysColor(COLOR_3DFACE);
	CRect rcItem(lpDIS->rcItem);

	if(m_bMouseIn && !bPressed)
		rgbBkg = RGB(230, 230, 230);

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	CBrush brBkg(rgbBkg);

	pDC->FillRect(rcItem, &brBkg);
	//pDC->Draw3dRect(rcItem, GetSysColor(bPressed ? COLOR_3DSHADOW : COLOR_3DHILIGHT), GetSysColor(bPressed ? COLOR_3DHILIGHT : COLOR_3DSHADOW));
	
	if(bPressed)
		rcItem.OffsetRect(1, 1);

	DrawIconEx(pDC->GetSafeHdc(),3,2,m_hIcon,0,0,0,0,DI_NORMAL);
	
	//CString sText;
	//GetWindowText(sText);
	//CFont font;
	//pDC->SelectObject(GetFont());
	pDC->SetBkColor(rgbBkg);
	pDC->DrawText(m_sDriveName.Left(1)+" ", rcItem, DT_VCENTER|DT_RIGHT|DT_SINGLELINE);

	pDC->Draw3dRect(rcItem, GetSysColor(bPressed ? COLOR_3DSHADOW : COLOR_3DHILIGHT), GetSysColor(bPressed ? COLOR_3DHILIGHT : COLOR_3DSHADOW));

	brBkg.DeleteObject();

}

BOOL CDriveButton::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT_PTR nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
	   pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
	   if(!::IsWindowEnabled((HWND)nID))
		   return FALSE;

	   // idFrom is actually the HWND of the tool
	   //nID = ::GetDlgCtrlID((HWND)nID);	  
   }
   
   CDriveButton * wnd = NULL;
   wnd = (CDriveButton *)CWnd::FromHandle((HWND)nID);
   if (wnd) //<- tego nie bylo i byl crash
			{
	   if(wnd->IsKindOf(RUNTIME_CLASS(CDriveButton)))
	   {
		   strTipText = wnd->GetDriveName();
	   }
   }
   
   
   


#ifdef _UNICODE
   _tcscpy(pTTTW->szText, strTipText);
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
   else
      _mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
#endif

   *pResult = 0;

   return TRUE;    // message was handled
}
