#include "common/_required.h"
#include "CEnumIdList.h"
#include "CShellFolder.h"

CEnumIdList::CEnumIdList(IEnumIDList* p)
{
	m_pEnum = p;
}

CEnumIdList::~CEnumIdList()
{
	if(m_pEnum){
		m_pEnum->Release();
		m_pEnum = NULL;
	}
}

CItemIdList CEnumIdList::CreateNextIDList()
{
	if(!m_pEnum)return CItemIdList();

	LPITEMIDLIST pidl;
	HRESULT hr = m_pEnum->Next(1,&pidl,NULL);
	if(hr!=NOERROR)return CItemIdList();

	CItemIdList cidl;
	cidl.Attach(pidl);

	return cidl;
}
