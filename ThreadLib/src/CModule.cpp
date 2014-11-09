#include "common/_required.h"
#include <windows.h>
#include "CModule.h"
#include <psapi.h>
#include <stdio.h>

CModule::CModule(HANDLE hProcessRef, HMODULE hModuleRef)
: m_hProcessRef(hProcessRef), m_hModuleRef(hModuleRef)
{
}

bool CModule::IsValid() const
{
	return m_hProcessRef!=NULL && m_hModuleRef!=NULL;
}

void CModule::GetBaseName(wchar_t* pBuf, int nBufCnt) const
{
	GetModuleBaseName(m_hProcessRef,m_hModuleRef,pBuf,nBufCnt);
}

void CModule::GetFilePath(wchar_t* pBuf, int nBufCnt) const
{
	GetModuleFileNameEx(m_hProcessRef,m_hModuleRef,pBuf,nBufCnt);
}

void CModule::Print() const
{
	wchar_t szPath[_MAX_PATH];
//	GetFilePath(szPath,_countof(szPath));
//	_putws(szPath);
	GetBaseName(szPath,_countof(szPath));
	_putws(szPath);
}
