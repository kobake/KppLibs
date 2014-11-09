#pragma once

#include <shlobj.h> //IEnumIDList
class CShellFolder;
class CItemIdList;

class CEnumIdList{
public:
	explicit CEnumIdList(IEnumIDList* p);
	CEnumIdList(const CEnumIdList& rhs)
	{
		m_pEnum = NULL;
		rhs.m_pEnum->Clone(&m_pEnum);
	}
	virtual ~CEnumIdList();
	CItemIdList CreateNextIDList();
private:
	IEnumIDList* m_pEnum;
};
