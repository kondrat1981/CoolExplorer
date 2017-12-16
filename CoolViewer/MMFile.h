#pragma once

class CMMFile
{
protected:
	HANDLE filehandle;
	HANDLE filemapping;
	const char *strSubstringg;
	//DWORD dwSizeLow;
	//DWORD dwSizeHigh;
	LONGLONG llFileSize;
	void *ptrFile;

	BOOL bFileOpened;
public:
	CMMFile(void);
	virtual ~CMMFile(void);

	void *GetPtr()
	{
		return ptrFile;
	}
	LONGLONG GetSize()
	{
		return llFileSize;
	}
	BOOL GetFilePtr(LPCTSTR pFilename , void *ptrFile);
	void CloseFile();
	
};
