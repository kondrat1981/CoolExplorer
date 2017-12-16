// FilesListVirtual.cpp: implementation of the CFilesListVirtual class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "coolexplorer.h"
#include "FilesListVirtual.h"
#include "FilesListNormal.h"
#include "FilesListManager.h"
#include "UnicodeFile.h"
#include "ItemsStorageVirt.h"
#include "ItemsStorageShell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilesListVirtual::CFilesListVirtual(CFilesListManager *pManager) : CFilesListBase(pManager)
{
	m_sPath = VIRT_NAME;
	m_mode = Virtual;
	m_bUsingNormal = FALSE;
	m_pList = new CFilesListNormal(pManager);
	
}

CFilesListVirtual::~CFilesListVirtual()
{
	delete m_pList;
	m_pList = NULL;
}

BOOL CFilesListVirtual::InitVirtualList(ListDataCreator * pListDataCreator)
{
	LockList();
	{
		if(pListDataCreator)
		{
			LISTData * lst = new LISTData;
			pListDataCreator->CreateList(*lst);	
			SetNewList(lst);
		}
		else
		{
			FreeList();
		}
	}
	UnlockList();
	return TRUE;
}

BOOL CFilesListVirtual::AddToVirtualList(LISTData &lst)
{
	LockList();
	{
		POSITION pos = lst.GetHeadPosition();

		while(pos)
		{
			ListData &ld = lst.GetNext(pos);
			AddTail(ld);			
		}
	}
	UnlockList();
	return TRUE;
}

BOOL CFilesListVirtual::RemoveFromVirtualList(const CStringList &lst)
{
	LockList();
	{
		POSITION pos = GetHeadPosition();
		LISTData * newlst = new LISTData;
		while(pos)
		{
			POSITION pos_old = pos;
			ListData *pld = GetNext(pos);
			if(NULL == lst.Find(pld->getName()))
				newlst->AddTail(*pld);
		}
		SetNewList(newlst);
	}
	UnlockList();
	return TRUE;
}

BOOL CFilesListVirtual::EnumFiles(LPCTSTR sPath, int nJob)
{
	BOOL bRet = FALSE;
	if(m_sPath == sPath)//refresh
	{
		LockList();
		{
			m_bUsingNormal = FALSE;
			m_sStartNormal = _T("");

			POSITION pos = GetHeadPosition();
			WIN32_FIND_DATA fd;
			LISTData * lst = new LISTData;
			while(pos && m_pManager->CanContinue(nJob))
			{
				ListData * ld = GetNext(pos);
				CString sName = ld->getName();
				HANDLE hFind = INVALID_HANDLE_VALUE;
				memset(&fd, 0, sizeof(fd));
				//if(!m_pManager->CanContinue(nJob))
				//return FALSE;
				UnlockList();
				hFind = FindFirstFile(sName, &fd);
				LockList();
				if(hFind != INVALID_HANDLE_VALUE)
				{
					FindClose(hFind);
					ld->setFileInfo(fd);
					ld->setName(sName);
					ld->setExactIcon(-1);
					ld->setIcon(-1);
					lst->AddTail(*ld);
				}
				else
				{
					ld->setExactIcon(DELETED_ICONINDEX);
					lst->AddTail(*ld);
				}
			}
			if(m_pManager->CanContinue(nJob))
				SetNewList(lst);
			else
				delete lst;
			lst = NULL;
		}
		UnlockList();
		bRet = TRUE;
	}
	else 
	{
		LockList();
		//jesli ktos wpisal sciezke, sprawdz czy istnieje taki element
		//czyli z Virt mozna przegladac tylko katalogi, ktore sa na glownej liscie
		// np jest tylko "c:\aa" to "c:\aa\bb" mozna przegladac a "c:\" juz nie
		//if(m_sStartNormal.IsEmpty())
		{
			POSITION pos = CFilesListBase::GetHeadPosition();
			while(pos)
			{
				ListData *p = CFilesListBase::GetNext(pos);
				CString sEl = p->getName();
				if(p->isDir() && !_tcsncmp(sEl, sPath + _tcslen(VIRT_NAME), sEl.GetLength()))
				{
					m_sStartNormal = sEl + dir_separator;
					m_bUsingNormal = TRUE;
					break;
				}
			}
		}
		CString sTemp = m_sPath + m_sStartNormal;
		UnlockList();
		if(!m_sStartNormal.IsEmpty() && !_tcsncmp(sTemp, sPath, sTemp.GetLength()-1))
		{
			bRet = m_pList->EnumFiles(sPath + m_sPath.GetLength(), nJob);
			if(!bRet)
			{
				LockList();
				m_sStartNormal = _T("");
				m_bUsingNormal = FALSE;
				UnlockList();
			}
		}
	}
	return bRet;
}

BOOL CFilesListVirtual::Select(const ListData *pld, BOOL &bBrowseItem, int nJob)
{
	BOOL bRet = FALSE;
	if(m_bUsingNormal)
	{
		if(m_sStartNormal == m_pList->GetDir() && pld->getName() == RET_NAME)
		{
			m_bUsingNormal = FALSE;
			m_sStartNormal = _T("");
			bRet = TRUE;
		}
		else
		{
			bRet = m_pList->Select(pld, bBrowseItem, nJob);
		}		
	}
	else if(!pld->isDir())
	{
		CString sName = pld->getName();
		CString sDir, sFile;
		GetDirFile(sDir, sFile, sName, dir_separator);
		SHELLEXECUTEINFO shi={0};
		shi.cbSize = sizeof (SHELLEXECUTEINFO); 
		shi.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_FLAG_DDEWAIT; 
		shi.hwnd = GetParent(); 
		shi.lpVerb = NULL; 
		shi.lpFile = sName; 
		shi.lpParameters = NULL; 
		shi.lpDirectory = sDir; 
		shi.nShow = SW_SHOWNORMAL; 
		
		TCHAR sCurDir[MAX_PATH*2];
		GetCurrentDirectory(sizeof(sCurDir), sCurDir);
		SetCurrentDirectory(sDir);
		if(ShellExecuteEx( &shi))
		{
			bRet = TRUE;		
		}
		SetCurrentDirectory(sCurDir);
	}
	else
	{
		CString sName = pld->getName();
		CString sDir, sFile;
		GetDirFile(sDir, sFile, sName, dir_separator);
		bRet = m_pList->EnumFiles(sName, nJob);
		if(bRet)
		{
			m_sStartNormal = m_pList->GetDir();
			m_bUsingNormal = TRUE;
		}

	}
	return bRet;
}

CString CFilesListVirtual::GetFileFullPath(const ListData * pdl) const
{
	if(m_bUsingNormal)
		return m_pList->GetFileFullPath(pdl);

	CString sFileFullPath;
	if(pdl)
		sFileFullPath = pdl->getName();
	return sFileFullPath;
}

ListData * CFilesListVirtual::GetNext(POSITION &pos)
{
	if(m_bUsingNormal)
		return m_pList->GetNext(pos);
	else
		return CFilesListBase::GetNext(pos);
}

POSITION CFilesListVirtual::GetHeadPosition() const
{
	if(m_bUsingNormal)
		return m_pList->GetHeadPosition();	
	
	return CFilesListBase::GetHeadPosition();
}

CString CFilesListVirtual::GetDir() const
{
	if(m_bUsingNormal)
		return m_sPath + m_pList->GetDir();
	else
		return m_sPath;
}

INT_PTR CFilesListVirtual::GetAllCount() const
{
	if(m_bUsingNormal)
		return m_pList->GetAllCount();
	
	return CFilesListBase::GetAllCount();
}

BOOL CFilesListVirtual::CreateDirectory(LPCTSTR sDir)
{
	BOOL bRet = FALSE;
	
	if(m_bUsingNormal)
		bRet = m_pList->CreateDirectory(sDir);	

	return bRet;
}

BOOL CFilesListVirtual::SaveToFile(LPCTSTR sFile)
{
	BOOL bRet = TRUE;

	LockList();
	{		
		CMarkup xml;
		xml.AddElem( _T("VirtualPanel") );
		xml.IntoElem();
		xml.AddElem( _T("Items") );
		xml.IntoElem();
		POSITION pos = CFilesListBase::GetHeadPosition();
		while(pos)
		{
			ListData *pld = CFilesListBase::GetNext(pos);
			xml.AddElem( _T("ITEM") );
			//xml.IntoElem();
			
			xml.AddAttrib( _T("Path"), pld->getName() );
			_TCHAR szVal[50] = {0};
			LONGLONG nValue = pld->getSize();					  
			_stprintf( szVal, _T("%I64d"), nValue );

			xml.AddAttrib( _T("Size"), szVal );
			xml.AddAttrib( _T("Desc"), pld->getDescription() );
			xml.AddAttrib( _T("Ext"), pld->getExtension() );
			xml.AddAttrib( _T("Atr"), pld->getAttrib() );
			
			//xml.OutOfElem();
		}

		xml.OutOfElem();
		
		CString sXml = xml.GetDoc();
		bRet = CUnicodeFile::WriteToFile(sFile, sXml, TRUE);
	}
	UnlockList();

	return bRet;
}
BOOL CFilesListVirtual::LoadFromFile(LPCTSTR sFile)
{
	BOOL bRet = TRUE;

	LockList();
	{
		CMarkup xml;
		CString sXml;
		bRet = CUnicodeFile::ReadFromFileBinary(sFile, sXml);
		FreeList();
		if(bRet)
		{
			if(xml.SetDoc(sXml) && xml.FindElem(_T("VirtualPanel")))
			{				
				xml.IntoElem();
				if(xml.FindElem(_T("Items")))
				{
					
					xml.IntoElem();
					ListData ld;
					while ( xml.FindElem(_T("ITEM")) )
					{
						ld.Clear();

						ld.setName(xml.GetAttrib(_T("Path")));
						ld.setExtension(xml.GetAttrib(_T("Ext")));
						ld.setSize(_ttoi64(xml.GetAttrib( _T("Size"))));
						ld.setDescription(xml.GetAttrib( _T("Desc")));
						ld.setAttrib(_ttoi(xml.GetAttrib( _T("Atr"))));

						AddTail(ld);
					}
				}
			}
		}
	}
	UnlockList();

	//EnumFiles(VIRT_NAME, JOBNUM_NONE);
	::PostMessage(GetParent(), WM_REFRESH, 0,0);

	return bRet;
}
/*
BOOL CFilesListVirtual::DisplayExplorerMenu(CItemsStorage *pData, CPoint pt)
{
	CItemsStorageVirt *pDataVirt = (CItemsStorageVirt *)pData;
	if(m_bUsingNormal)
		return m_pList->DisplayExplorerMenu(pDataVirt->getShellList(), pt);
	CMenu menu;
	menu.CreatePopupMenu();

	int nCmd = 100;
	menu.AppendMenu(MF_STRING, ID_DELETE_FROMVIRTUAL, _T_LNGSTR(245));//"Remove from Virtual Panel\t+Del"
	menu.AppendMenu(MF_STRING, ID_DELETE, _T_LNGSTR(246));//"Delete file(s) from disk\tCtrl+Del"

	nCmd = ::TrackPopupMenu(menu.m_hMenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, GetParent(), NULL);
	//menu.
	CString sSelected;
	if(nCmd>0)
	{
		::SendMessage(GetParent(), WM_COMMAND, nCmd, 0);
	}

	menu.DestroyMenu();
	return TRUE;
}*/


CItemsStorage *CFilesListVirtual::SaveItems(LISTDataPointers &listItems)
{
	CItemsStorageVirt *pData = NULL;
	CItemsStorageShell *pShellData = NULL;
	CString sName;
	
	if(m_bUsingNormal)
	{
		pShellData = (CItemsStorageShell *)m_pList->SaveItems(listItems);
		sName = m_pList->GetDir();
	}
	else
	{
		pShellData = (CItemsStorageShell *)SaveItemsUsingFilesNames(listItems);
		sName = GetDir();
	}

	if(pShellData)
		pData = new CItemsStorageVirt(GetParent(), this, pShellData);
	else		
		pData = new CItemsStorageVirt(GetParent(), this, NULL);
	pData->setDir(sName);

	return pData;
}