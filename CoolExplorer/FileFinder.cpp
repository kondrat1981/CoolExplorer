#include "stdafx.h"
#include "MainSearchDlg.h"
#include "FileFinder.h"
#include "CoolExplorer.h"



#pragma hdrstop


CFileFinder::CFileFinder()
{
    m_bSInProgress = TRUE;

}

void CFileFinder::SetSrchMask( _ff_mask mask )
{
    m_SrchMask = mask;
}

void CFileFinder::GetSrchMask( _ff_mask &mask )
{
    mask = m_SrchMask;
}

void CFileFinder::Clear()
{
	m_result.clear();
	if (m_result.capacity() > 50)
	{
		m_result.reserve(50);

	}

	m_SrchMask.Clear();
}
/*
void FindFiles( CString strDirectory )
{
    HANDLE fHandle;
    BOOL bFind = TRUE;
    WIN32_FIND_DATA _file_data_info;

    //create directory path
    CString dir =  strDirectory +"\\"+ "*" ;
    fHandle = FindFirstFile(dir,&_file_data_info);

    if ( fHandle != INVALID_HANDLE_VALUE )
    {

        //if it is a directory
        if ((_file_data_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )&&
             (strcmp(_file_data_info.cFileName,".") !=0 ))
        {
            //create new directory to search into
            CString newDir = strDirectory + "\\" + _file_data_info.cFileName;
            FindFiles(newDir);

        }
        while ((bFind)&&(m_bSInProgress))
        {

            bFind = FindNextFile( fHandle , &_file_data_info);

            if (bFind)
            {
                if ((_file_data_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )&&
                    (strcmp(_file_data_info.cFileName,"..") !=0 ))
                {
                    
                    CString newDir = strDirectory + "\\" + _file_data_info.cFileName;
                    FindFiles(newDir);
                    
                }
                
                if (!strcmp(_file_data_info.cFileName,m_SrchMask._filename_mask))
                {
                    CString result = strDirectory + _file_data_info.cFileName;

                    m_UpdateListCritSection.Lock();
                    m_result.Add( result );
                    m_UpdateListCritSection.Unlock();

                    //send message do update files list
                    
                }
            }
 
       
        }

    }
}*/