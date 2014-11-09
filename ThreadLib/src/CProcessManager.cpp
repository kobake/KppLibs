#include "CProcessManager.h"
#include "CPipe.h"
#include "CProcess2.h"

// �v���Z�X�N���B�I������܂ő҂��A���̕W���o�͂�Ԃ��B//
std::string	CProcessManager::RunProcessSync(const wchar_t* szCommand)
{
	// EXE���s //
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

// �v���Z�X�N���B�I����҂����ɂ����ɐ����Ԃ��B//
bool CProcessManager::RunProcessAsync(const wchar_t* szCommand)
{
	CProcess2 cProcess(szCommand, NULL, NULL, NULL);
	return true;
}

