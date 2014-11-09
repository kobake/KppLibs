#include "common/_required.h"
#include <windows.h>
#include <psapi.h>
#include "CProcessWatcher.h"
using namespace std;
#include "CProcess.h"

std::vector<CProcess> CProcessWatcher::GetProcessList() const
{
	//プロセスIDの列挙
	DWORD aProcesses[1024];
	DWORD dw;
	BOOL bRet=EnumProcesses(aProcesses,sizeof(aProcesses),&dw);
	if(!bRet)return vector<CProcess>(); //失敗
	int nProcesses=dw/sizeof(DWORD);
	//CProcessの配列に変換
	vector<CProcess> vProcesses;
	for(int i=0;i<nProcesses;i++){
		vProcesses.push_back(CProcess(aProcesses[i]));
	}
	return vProcesses;
}
