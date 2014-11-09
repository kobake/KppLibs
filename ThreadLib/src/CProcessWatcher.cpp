#include "common/_required.h"
#include <windows.h>
#include <psapi.h>
#include "CProcessWatcher.h"
using namespace std;
#include "CProcess.h"

std::vector<CProcess> CProcessWatcher::GetProcessList() const
{
	//�v���Z�XID�̗�
	DWORD aProcesses[1024];
	DWORD dw;
	BOOL bRet=EnumProcesses(aProcesses,sizeof(aProcesses),&dw);
	if(!bRet)return vector<CProcess>(); //���s
	int nProcesses=dw/sizeof(DWORD);
	//CProcess�̔z��ɕϊ�
	vector<CProcess> vProcesses;
	for(int i=0;i<nProcesses;i++){
		vProcesses.push_back(CProcess(aProcesses[i]));
	}
	return vProcesses;
}
