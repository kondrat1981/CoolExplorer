#include "stdafx.h"
#include "FilesListBase.h"
#include "ListData.h"

int CALLBACK comp_names (const void *, const void *, LPARAM Crit);
int comp_names (const void *pEl1, const void *pEl2)
{
	return comp_names(pEl1, pEl2, cn_name + 1) < 0 ? TRUE : FALSE;
}

int comp_dates(const void *pEl1, const void *pEl2)
{
	return comp_names(pEl1, pEl2, cn_date + 1) < 0 ? TRUE : FALSE;
}

int comp_sizes(const void *pEl1, const void *pEl2)
{
	return comp_names(pEl1, pEl2, cn_size + 1) < 0 ? TRUE : FALSE;
}

int comp_names_desc (const void *pEl1, const void *pEl2)
{
	return comp_names(pEl1, pEl2, -cn_name - 1) < 0 ? TRUE : FALSE;
}

int comp_dates_desc (const void *pEl1, const void *pEl2)
{
/*	TRACE("%s %s\t\t\t%s %s\t\t\t", ((ListData *) pEl1)->info.sName, ((ListData *) pEl2)->info.sName,
			((ListData *) pEl1)->info.tmCreat.Format("%d/%m/%Y %H:%M:%S"),
			((ListData *) pEl2)->info.tmCreat.Format("%d/%m/%Y %H:%M:%S"));
*/
	return comp_names(pEl1, pEl2, -cn_date - 1) < 0 ? TRUE : FALSE;
}
int comp_sizes_desc (const void *pEl1, const void *pEl2)
{
	return comp_names(pEl1, pEl2, -cn_size - 1) < 0 ? TRUE : FALSE;
}



int CALLBACK comp_names (const void * pEl1, const void * pEl2, int Crit)
{	
	int nRet = 0;
	ListData * pld1 = (ListData *) pEl1;
	ListData * pld2 = (ListData *) pEl2;
	LPCTSTR s1 = pld1->getName();
	LPCTSTR s2 = pld2->getName();

	if(_tcscmp(s1, RET_NAME) == 0)
		return -1;

	if(_tcscmp(s2, RET_NAME) == 0)
		return 1;

	int iWsp = Crit;
	

	BOOL bDir1 = pld1->isDir();
	BOOL bDir2 = pld2->isDir();

	if(abs(Crit) == cn_date + 1
		&& ((bDir1 && bDir2) || (!bDir1 && !bDir2)))
	{
		//const CTime &t1 = pld1->getTime();
		//const CTime &t2 = pld2->getTime();
		//if(t1 > t2 /*|| bDir2*/)
		return iWsp * CompareFileTime(&pld1->getFileTime(), &pld2->getFileTime());
		
		//if(t1 < t2 /*|| bDir1*/)
		//	return -iWsp;
		
		//return 0;			
	}
	else
	{
		if(bDir1 && bDir2)
		{
			return iWsp * _tcsicmp(s1, s2);
		}

		if(bDir1)
			return -1;
		if(bDir2)
			return 1;

		switch(abs(Crit))
		{
			case cn_name + 1:
				return iWsp * _tcsicmp(s1, s2);
			case cn_size + 1:
				if(iWsp * (pld1->getSize() - pld2->getSize()) > 0)
					return 1;
				else if(iWsp * (pld1->getSize() - pld2->getSize()) < 0)
					return -1;
				else
					return 0;
		}
	}

	//ASSERT(0);
	return 0;
}

int CALLBACK comp_dates (LPARAM lpEl1, LPARAM lpEl2, LPARAM bDesc)
{
	int nRet = 0;
	ListData * pEl1 = (ListData *) lpEl1;
	ListData * pEl2 = (ListData *) lpEl2;

	nRet = CompareFileTime(&pEl1->getFileTime(), &pEl2->getFileTime());
	
	if(bDesc)
		nRet *= -1;
	return nRet;
}

int CALLBACK comp_sizes (LPARAM pEl1, LPARAM pEl2, LPARAM bDesc)
{
	int nRet = 0;
	LONGLONG s1 = ((ListData *) pEl1)->getSize();
	LONGLONG s2 = ((ListData *) pEl2)->getSize();
	if(s1 > s2)
		nRet = 1;
	else if(s1 < s2)
		nRet = -1;
	
	if(bDesc)
		nRet *= -1;
	return nRet;
}