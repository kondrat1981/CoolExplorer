#include "StdAfx.h"
#include "DirMonitor.h"


void CDirMonitor::SetMonitor(const CString &dir)
{
	if (m_thread)
	{
		DWORD dw = m_thread->m_nThreadID;
		CoolTrace(0, TRUE, _T("Begin SetMonitor for threadID %d, dir:%s"),dw,dir);
	}
	else
	{
		CoolTrace(0, TRUE, _T("Begin SetMonitor, dir:%s"),dir);
	}
	
	HWND hwnd = NULL;
	CString old_dir;
	
	mutex_data.Lock();
		hwnd = m_Data.m_hwnd;
//		msg = m_Data.m_msg;
//		m_Data.m_strDir = dir;
		old_dir = m_Data.m_strDir;
	mutex_data.Unlock();
	if ((old_dir == dir)&&(dir != _T(""))&&(dir != _T("!")))
	{
		CoolTrace(0, TRUE, _T("Begin SetMonitor, same dir as previous, dir:%s, exiting!"),dir);
		return;
	}
	if (hwnd != NULL)
	{
		BOOL bCreate = TRUE;
		BOOL bDisable = FALSE;
		UINT nDriveType = GetDriveType(dir);
		if (nDriveType != DRIVE_FIXED && nDriveType != DRIVE_REMOVABLE) // !!TODO!!
		{
			//disable monitor
			bDisable = TRUE;
			CoolTrace(0, TRUE, _T("   SetMonitor, dir:%s; disable monitor  == TRUE, not fixed drive"),dir);
		}
		else
		{
			CoolTrace(0, TRUE, _T("   SetMonitor, dir:%s; disable monitor  == FALSE, fixed drive"),dir);
			mutex_data.Lock();
			m_Data.m_strDir = dir;
			m_Data.bRefresh = FALSE;
			mutex_data.Unlock();
		}
		
		DWORD dwCode;
		if (m_thread)
		{
			if (GetExitCodeThread(m_thread->m_hThread,&dwCode))
			{
				if (dwCode == STILL_ACTIVE)
				{
					bCreate = FALSE;

				}
			}
		}
		if (bCreate)
		{
			//set change data/stop event
				CoolTrace(0, TRUE, _T("   SetMonitor, dir:%s; bCreate == TRUE"),dir);
				mutex_data.Lock();
					m_Data.notify_handle[1] = (HANDLE)m_stop_event;
				mutex_data.Unlock();

				if(m_thread)
				{
					delete m_thread;
					m_thread = NULL;
				}
		
				m_thread = AfxBeginThread(DirNotifyThread,this,THREAD_PRIORITY_IDLE, 0, CREATE_SUSPENDED);
				TRACE(_T("CDirMonitor start thread: 0x%X\n"), m_thread);

				if (m_thread == NULL)
				{
					//AfxMessageBox(_T("Error Creating watching thread."));
					CoolTrace(0, TRUE, _T("         SetMonitor,Error Creating watching thread. dir:%s; bCreate == FALSE"),dir);
				}
				else
				{
					m_thread->m_bAutoDelete = FALSE;
					m_thread->ResumeThread();
				}
		}
		else
		{
			CoolTrace(0, TRUE, _T("   SetMonitor, dir:%s; bCreate == FALSE"),dir);
			//change data in thread, do event to change dir
			mutex_data.Lock();
				if (bDisable)
				{
					if (dir == _T("!"))
					{
						m_Data.m_strDir = _T("!");

					}
					else
					{
						m_Data.m_strDir = _T("");
					}
					
				}
				else
				{
					m_Data.m_strDir = dir;
				}
				CString strDir = m_Data.m_strDir;
				
			mutex_data.Unlock();
			CoolTrace(0, TRUE, _T("     SetMonitor, dir:%s"),strDir);
			m_stop_event.SetEvent();
		}

	}
	CoolTrace(0, TRUE, _T("End SetMonitor, dir:"));

}

CDirMonitor::CDirMonitor(void):m_thread(NULL)
{
	CoolTrace(0, TRUE, _T("CDirMonitor::CDirMonitor"));
}

CDirMonitor::~CDirMonitor(void)
{
	CoolTrace(0, TRUE, _T("CDirMonitor::~CDirMonitor - begin"));
	SetMonitor(_T("!"));
	if (m_thread)
	{
		DWORD dw = m_thread->m_nThreadID;
		CoolTrace(0, TRUE, _T("    CDirMonitor::~CDirMonitor - Wait until TID %d ends"),dw);
		DWORD dwCode = STILL_ACTIVE;
		while (dwCode == STILL_ACTIVE)
		{
			m_stop_event.SetEvent();
			GetExitCodeThread(m_thread->m_hThread,&dwCode);
			Sleep(20);
		}
		delete m_thread;
		m_thread = NULL;
		CoolTrace(0, TRUE, _T("CDirMonitor::~CDirMonitor - Closing TID %d "),dw);

	}
CoolTrace(0, TRUE, _T("CDirMonitor::~CDirMonitor - end"));
}
UINT __cdecl DirNotifyThread( LPVOID pParam )
{
	DWORD handle = GetCurrentThreadId();
	CoolTrace(0, TRUE, _T("    -BEGIN---DirNotifyThread for TId %d---------------"),handle);
	CDirMonitor *_ptr = (CDirMonitor*)pParam;
	DWORD dwWaitStatus = 0;
	BOOL bContinue = TRUE;
	_data_Monitor data;

	_ptr->mutex_data.Lock();
	data = _ptr->m_Data;
	_ptr->mutex_data.Unlock();
	CoolTrace(0, TRUE, _T("    ---------DirNotifyThread for TId %d--dir:%s-------"),handle,data.m_strDir);
	if (!data.m_strDir.IsEmpty())
	{
		data.notify_handle[0] = FindFirstChangeNotification(data.m_strDir,FALSE,DEF_WATCH_PARAMS);
		
		if (data.notify_handle[0] == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox(_T("Error creating notification Handle"));
		}
		else
		{
			while (bContinue) 
			{
				ResetEvent(data.notify_handle[1]);
				dwWaitStatus = WaitForMultipleObjects(2,data.notify_handle,FALSE,INFINITE);
				
				
				switch (dwWaitStatus) 
				{
				case WAIT_OBJECT_0:
					//send message
					if (data.m_hwnd != NULL)
					{
							_ptr->mutex_data.Lock();
							BOOL bRefresh = _ptr->m_Data.bRefresh;
							BOOL bEnable = _ptr->m_Data.bEnable;
							_ptr->mutex_data.Unlock();
							if (bEnable && !bRefresh)
							{
								//Sleep(100);
								_ptr->mutex_data.Lock();
								_ptr->m_Data.bRefresh = TRUE;
								_ptr->mutex_data.Unlock();								
								VERIFY(PostMessage(data.m_hwnd,WM_REFRESH,1,0));
								TRACE(_T("PostMessage from DirNotifyThread %s\n"), data.m_strDir);
								
							}
					}
					
					if (!FindNextChangeNotification(data.notify_handle[0]))
					{
						CoolTrace(0, TRUE, _T("    ---------DirNotifyThread for TId %d--dir:%s-------"),handle,data.m_strDir);
						//AfxMessageBox(_T("Error Waiting for event"));
					}
					
					break;
				case WAIT_OBJECT_0 + 1:
					if (FindCloseChangeNotification( data.notify_handle[0]))
					{
						_ptr->mutex_data.Lock();
							data = _ptr->m_Data;
						_ptr->mutex_data.Unlock();
						CoolTrace(0, TRUE, _T("    ---------DirNotifyThread for TId %d--dir:%s-------"),handle,data.m_strDir);
						if ((data.m_strDir != _T(""))&&(data.m_strDir != _T("!")))
						{
							data.notify_handle[0] = FindFirstChangeNotification(data.m_strDir,FALSE,DEF_WATCH_PARAMS);
							if (data.notify_handle[0] == INVALID_HANDLE_VALUE)
							{
								AfxMessageBox(_T("Error creating notification Handle"));
							}
						}
						else
						{
							bContinue = FALSE;
						}
						
					}
					
					break;
					
				case WAIT_TIMEOUT:
					break;
				case WAIT_FAILED:
					DWORD d = GetLastError();
					AfxMessageBox(_T("Error Waiting for event"));
					bContinue = FALSE;
					break;
				}
				
				
				
			}
		}
	}
	_ptr->e.Unlock();
	CoolTrace(0, TRUE, _T("    -END---DirNotifyThread for TId %i---------------"),handle);
	return 0;
}
void CDirMonitor::SetCALLBACK(HWND hwnd)
{
	mutex_data.Lock();
		m_Data.m_hwnd = hwnd;
//		m_Data.m_msg = msg;
	mutex_data.Unlock();

}
void CDirMonitor::Pulse(const CString &dir)
{
	//m_stop_event.ResetEvent();
	//m_stop_event.PulseEvent();
	m_stop_event.SetEvent();
}
