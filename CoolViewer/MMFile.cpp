#include "stdafx.h"
#include "MMFile.h"

CMMFile::CMMFile(void)
{
	filehandle = NULL;
	filemapping = NULL;
	//dwSizeLow = 0;
	//dwSizeHigh = 0;
	llFileSize = 0;

	ptrFile = NULL;
	bFileOpened = FALSE;
}

CMMFile::~CMMFile(void)
{
	if (bFileOpened)
	{
		CloseFile();
	}

}

BOOL CMMFile::GetFilePtr(LPCTSTR pFilename , void *ptr)
{
	ASSERT(pFilename);
	BOOL bRet = FALSE;

	if (pFilename == NULL)
		return bRet;

	filehandle = CreateFile(pFilename,FILE_READ_DATA,NULL,NULL,OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE|FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_READONLY|
		FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_TEMPORARY,NULL);
	
	if (filehandle != NULL)
	{
		filemapping = CreateFileMapping(filehandle, NULL, PAGE_READONLY,
			0,0,_T("ViewerLoad"));
				//if ok, create a file view
		if (filemapping != NULL)
		{
			ptr = MapViewOfFile(filemapping,FILE_MAP_READ,0,0,0);

			if (ptr != NULL)
			{
				bFileOpened = TRUE;
				//try to find substring in file
				// if Match Case is enabled, use strstr.
				ULARGE_INTEGER fsize;
				fsize.QuadPart = 0;
				fsize.LowPart = GetFileSize (filehandle, &fsize.HighPart) ;
				llFileSize = fsize.QuadPart;

				ptrFile = ptr;
				bRet = TRUE;
			}
		}
	}
	return bRet;
}

void CMMFile::CloseFile()
{
	if (ptrFile != NULL)
	{
		if (UnmapViewOfFile(ptrFile)) 
		{   
			ptrFile = NULL;

			if (CloseHandle(filemapping))
			{
				filemapping = NULL;
				if (CloseHandle(filehandle))
				{
					filehandle = NULL;
					bFileOpened = FALSE;
				}
			}

		}
	}
	

}