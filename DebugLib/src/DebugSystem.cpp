#include "DebugSystem.h"
#include <stdlib.h> //malloc
#define _CRTDBG_MAP_ALLOC   // malloc��_malloc_dbg�̒u������
#include <crtdbg.h> //_CrtDumpMemoryLeaks


void DebugSystem::memoryCheckBegin()
{
	// �S�Ẵ��|�[�g�o�͂��E�B���h�E�ɑ���
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_WNDW);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);

	// �v���O�����̍ŏ��ŃR�[������
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
}
