#include "common/_required.h"
#include "common/sys.h"
#include <ThreadLib.h>
#include <windows.h>
#include <DebugLib.h>

class CMyThread : public CThread{
protected:
	DWORD Run()
	{
		return 0;
	}
};

int wmain(int argc, wchar_t* argv[])
{
	c2lib::Init(argc, argv);
	debugInit(DEBUG_OUT_DEBUGGER);

	// スレッド //
	if(0){
		CMyThread c;
		c.Create();
		::Sleep(10);
	}

	// パイプ //
	if(1){
		CPipe cPipe;
//		CProcess2 cProcess(L"ping www.yahoo.co.jp", cPipe.GetWriteHandle(), cPipe.GetWriteHandle(), NULL);
		CProcess2 cProcess(L"cat sample.txt", cPipe.GetWriteHandle(), cPipe.GetWriteHandle(), NULL);
		while(cPipe.Peek() || cProcess.IsAlive()){
			std::string str;
			/*
			if(cPipe.ReadTextIfPeek(&str)){
				printf("%s", str.c_str());
			}
			*/
			if(cPipe.ReadLine(&str, false)){
				printf("%s\n", str.c_str());
			}
		}
	}

	return 0;
}

