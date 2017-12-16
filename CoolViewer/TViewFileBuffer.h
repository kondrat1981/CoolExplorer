#pragma once

class TViewFileBuffer
{
	void *m_ptrToFile;
public:
	TViewFileBuffer(void);
	virtual ~TViewFileBuffer(void);
	
	BOOL BoundFilePtr(void *ptrFile);


};
