
#define COOL_VERSION _T("0.7 [build 248]")

#define RET_NAME _T("..")
#define VIRT_NAME _T("<Virt>")

#ifdef _DEBUG
#define DEBUG_MESSAGE(x) AfxMessageBox(x);
#define DTIMER_START CMyTimer tm; tm.Start();
#define DTIMER_STOP(x) tm.Stop(FALSE); \
	CString sTrace; \
	sTrace.Format(_T("%s %s s\n"), x, tm.GetStringTime());\
TRACE(sTrace);


#else	//release
#define DEBUG_MESSAGE(x)
#define DTIMER_START
#define DTIMER_STOP(x)

#endif

#define WM_PROGRESS					WM_USER+6
#define WM_RENAME					WM_USER+9

#define WM_SETPMENU					WM_USER+10
#define WM_CLOSE_INFO				WM_USER+12
#define WM_REFRESHICON				WM_USER+13

#define WM_REFRESH					WM_USER+11

//search screen
#define WM_UPDATE_WIN_FRAME			WM_USER+14
#define WM_REFRESH_FFILES			WM_USER+15
#define WM_SEARCH					WM_USER+16
#define WM_UPDATE_DRIVE_CAPACITY	WM_USER+18
#define WM_SEARCH_WINDOW_FINISHED   WM_USER + 19     // user closed search window

//command
#define WM_DO_COMMAND				WM_USER + 20

#define WM_TRAY_ICON_NOTIFY_MESSAGE WM_USER + 21    //message to process try icon actions

#define WM_POST_SETFOCUS			WM_USER + 22    //

#define WM_MYCLOSE					WM_USER + 23

#define WM_FOCUSFROMVIEW			WM_USER + 24

#define WM_DO_FOCUS					WM_USER + 25

#define WM_EVENTUPDATE				WM_USER + 26


#define RETURN_ICONINDEX			-2
#define DELETED_ICONINDEX			-3


#define SHGFI_OVERLAYINDEX      0x000000040
//
//#define WM_UPDATEPATH       WM_USER+14
#define FROM_PATHBAR		1
//

#define	cn_name  0
#define	cn_ext  1
#define	cn_size  2
#define	cn_date  3
#define	cn_attrib  4

#define SKIP					0x01
#define SKIP_ALL				0x02
#define SKIP_ALL_THIS_TYPE		0x04
#define YES						0x08
#define YES_TO_ALL				0x10
#define YES_TO_ALL_THIS_TYPE	0x20
#define ABORT					0x40
#define SKIP_ALL_READ_ONLY		0x80
#define SKIP_ALL_SYSTEM			0x100
#define SKIP_ALL_HIDDEN			0x200
#define SKIP_ALL_SHARED			0x400
#define SKIP_ALL_EXISTING		0x800
#define YES_TO_ALL_READ_ONLY	0x1000
#define YES_TO_ALL_SYSTEM		0x2000
#define YES_TO_ALL_HIDDEN		0x4000


#define PROGRESS_FROM 1
#define PROGRESS_TO 2
#define NAME_TO 3
#define NAME_FROM 4
#define PROGRESS_RANGE 6
#define PROGRESS_RANGE_ONE 7
#define PROGRESS_NEW 8
#define PROGRESS_DIRNAME 9
#define PROGRESS_WINNAME 10
#define PROGRESS_POS 11
#define PROGRESS_STAT 12
#define PROGRESS_AND_NAME 13

#define MARKTHESAME 1
#define MARKDIFFRENT 2


struct CDirFile;
//struct DelInfo;
struct CopyInfo;
//typedef CArray<LPCITEMIDLIST, LPCITEMIDLIST> ITEMIDARRAY;
typedef void (*PFILEFUN)(CDirFile *file, LPCTSTR sDesc);
typedef void (*PDIRFUN)(LPCTSTR sDirName, LPCTSTR sDesc);
//typedef void (*PDELPROGRESSFUN)(DelInfo *pDelInfo);

//struct CopyInfo;
typedef void (*PCOPYPROGRESSFUN)(CopyInfo *pCopyInfo);

typedef CArray<int, int> CIntArray;