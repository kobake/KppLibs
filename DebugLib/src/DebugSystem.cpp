#include "DebugSystem.h"
#include <stdlib.h> //malloc
#define _CRTDBG_MAP_ALLOC   // malloc→_malloc_dbgの置き換え
#include <crtdbg.h> //_CrtDumpMemoryLeaks


void DebugSystem::memoryCheckBegin()
{
	// 全てのレポート出力をウィンドウに送る
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_WNDW);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);

	// プログラムの最初でコールする
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
}
