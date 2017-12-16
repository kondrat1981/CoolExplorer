#include "stdafx.h"
#include "TViewFileBuffer.h"

TViewFileBuffer::TViewFileBuffer(void)
{
	m_ptrToFile = NULL; 
}


TViewFileBuffer::~TViewFileBuffer(void)
{

}


BOOL TViewFileBuffer::BoundFilePtr(void *ptrFile)
{
	BOOL bRet = TRUE;
	if (ptrFile != NULL)
	{
		m_ptrToFile = ptrFile;
	}
	else
	{
		bRet = TRUE;
	}
	return bRet;
}