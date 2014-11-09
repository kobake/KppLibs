#include "common/_required.h"
#include <windows.h>
#include <stdio.h>
#include "CThreadInfo.h"
#include "../../WinSystemLib/src/Cast.h"
using namespace std;

CThreadInfo::CThreadInfo(HANDLE hProcessRef, DWORD dwThreadID)
: m_hProcessRef(hProcessRef)
, m_dwThreadID(dwThreadID)
{
}


void CThreadInfo::Print()
{
	puts("CThreadInfo");
	vector<HWND> vWindows=GetWindowList();
	for(int i=0;i<(int)vWindows.size();i++){
		wchar_t buf[256];
		GetWindowText(vWindows[i],buf,_countof(buf));
		printf("HWND[%02d]=%08X (%ls)\n",i,(int)PointerToLong(vWindows[i]),buf);
	}
}

HANDLE CThreadInfo::GetProcessHandle() const
{
	return m_hProcessRef;
}

DWORD CThreadInfo::GetThreadID() const
{
	return m_dwThreadID;
}


static BOOL CALLBACK EnumThreadWndProc(HWND hwnd,LPARAM lParam)
{
	vector<HWND>* pvWindows=(vector<HWND>*)lParam;
	pvWindows->push_back(hwnd);
	return TRUE;
}

std::vector<HWND> CThreadInfo::GetWindowList() const
{
	vector<HWND> vWindows;
	EnumThreadWindows(
		this->GetThreadID(),
		EnumThreadWndProc,
		(LPARAM)&vWindows
	);
	return vWindows;
}
