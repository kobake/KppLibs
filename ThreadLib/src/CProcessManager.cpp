#include "CProcessManager.h"
#include "CPipe.h"
#include "CProcess2.h"

// プロセス起動。終了するまで待ち、その標準出力を返す。//
std::string	CProcessManager::RunProcessSync(const wchar_t* szCommand)
{
	// EXE実行 //
	std::string strOutput;
	CPipe cPipe;
	CProcess2 cProcess(szCommand, cPipe.GetWriteHandle(), NULL, NULL);
	while(cPipe.Peek() || cProcess.IsAlive()){
		std::string str;
		if(cPipe.ReadText(&str, false)){
			strOutput += str;//cpp_mbstowcs(str, CP_UTF8);
		}
	}
	return strOutput;
}

// プロセス起動。終了を待たずにすぐに制御を返す。//
bool CProcessManager::RunProcessAsync(const wchar_t* szCommand)
{
	CProcess2 cProcess(szCommand, NULL, NULL, NULL);
	return true;
}

