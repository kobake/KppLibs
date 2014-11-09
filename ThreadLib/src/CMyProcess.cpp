#include "CMyProcess.h"
#include <tlhelp32.h>

std::vector<DWORD> CMyProcess::GetChildThreadIds()
{
	// スレッド一覧の取得 //
	DWORD dwMyProcessId = ::GetCurrentProcessId();
	std::vector<DWORD> vThreadIds;
//	printf("\n");
//	printf("--------\n");
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if(hSnapshot != INVALID_HANDLE_VALUE){
		THREADENTRY32 te32 = {0};
		te32.dwSize = sizeof(te32);
		if(::Thread32First(hSnapshot, &te32)){
			do{
				if(te32.th32OwnerProcessID == dwMyProcessId){
//					printf("ProcessId:%d ThreadId:%d\n", te32.th32OwnerProcessID, te32.th32ThreadID);
					vThreadIds.push_back(te32.th32ThreadID);
				}
			}
			while(::Thread32Next(hSnapshot, &te32));
		}
		::CloseHandle(hSnapshot);
	}
	// スレッド群 //
	return vThreadIds;
}
