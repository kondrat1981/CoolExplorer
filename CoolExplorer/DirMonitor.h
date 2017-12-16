#include "afxmt.h"
#define DEF_WATCH_PARAMS		FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME|FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_LAST_WRITE
#pragma once



UINT __cdecl DirNotifyThread( LPVOID pParam );

struct _data_Monitor
{
	_data_Monitor():bRefresh(FALSE),bEnable(TRUE),m_hwnd(NULL){};
	CString m_strDir;
	HANDLE notify_handle[2]; 
	BOOL bRefresh;
	BOOL bEnable;
	HWND m_hwnd;
	//UINT m_msg;
};

class CDirMonitor
{
protected:
	CEvent m_stop_event;
	CWinThread *m_thread;
	CMutex mutex_refresh;
	

	void Pulse(const CString &dir);	
public:
	CEvent e;
	CMutex mutex_data;
	_data_Monitor m_Data;
	friend UINT __cdecl DirNotifyThread( LPVOID pParam );
	CDirMonitor *getPtr()
	{
		return this;
	}
	void SetMonitor(const CString &dir);
	void SetCALLBACK(HWND hwnd);
	CDirMonitor(void);
	void DisableRefresh()
	{
		TRACE(_T("CDirMonitor::DisableRefresh\n"));
		mutex_data.Lock();
		m_Data.bEnable = FALSE;
		mutex_data.Unlock();		
	};
	void EnableRefresh()
	{
		TRACE(_T("CDirMonitor::EnableRefresh\n"));
		mutex_data.Lock();
		m_Data.bEnable = TRUE;
		//m_Data.bRefresh = FALSE;
		mutex_data.Unlock();		
	};
public:
	BOOL SetChangeThread();
	virtual ~CDirMonitor(void);
};
