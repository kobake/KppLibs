#include <BaseLib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>
#include "debug.h"

#ifdef _DEBUG

ELogLevel g_nLogLevel = LEVEL_ALL; //����ԏo��

void _SetLogLevel(ELogLevel nLevel)
{
	g_nLogLevel = nLevel;
}

void _ErrorLog(ELogLevel nLevel, const wchar_t* szMsg, ...)
{
	if(nLevel & g_nLogLevel){
		wchar_t buf[1024];
		va_list v;
		va_start(v, szMsg);
		vswprintf_s(buf, _countof(buf), szMsg, v);
		va_end(v);

		debugPrint(buf);
	}
}







EDebugOutputMode g_eDebugMode = DEBUG_OUT_DEBUGGER;
ASSERT_HANDLE g_pfAssertHandle = NULL;

const wchar_t* debug_format(const wchar_t* str,...)
{
	static wchar_t buf[256];
	va_list mark;
	va_start(mark,str);
	_vsnwprintf(buf,countof(buf)-1,str,mark);
	va_end(mark);
	return buf;
}

//!�f�o�b�O������I��
void debug_init(EDebugOutputMode mode, ASSERT_HANDLE pfAssertHandle)
{
	g_eDebugMode = mode;
	g_pfAssertHandle = pfAssertHandle;
}

//!�f�o�b�O���b�Z�[�W�o��
void debug_output(const char* str, ...)
{
	static char buf[256];
	va_list mark;
	va_start(mark,str);
	vsprintf(buf,str,mark);
	va_end(mark);

	switch(g_eDebugMode){
	case DEBUG_OUT_STDOUT:
		//�R���\�[���ɏo��
		printf("%s",buf);
		break;
	case DEBUG_OUT_MSGBOX:
		//���b�Z�[�W�{�b�N�X�ɏo�� (or DebugOutputString)
		MessageBoxA(NULL,buf,"Error",MB_OK);
		break;
	case DEBUG_OUT_DEBUGGER:
		//�f�o�b�K�ɏo��
		OutputDebugStringA(buf);
		break;
	}
}

//!�f�o�b�O���b�Z�[�W�o��
void debug_output(const wchar_t* str, ...)
{
	static wchar_t buf[256];
	va_list mark;
	va_start(mark,str);
	vswprintf(buf,str,mark);
	va_end(mark);

	switch(g_eDebugMode){
	case DEBUG_OUT_STDOUT:
		//�R���\�[���ɏo��
		wprintf(L"%s",buf);
		break;
	case DEBUG_OUT_MSGBOX:
		//���b�Z�[�W�{�b�N�X�ɏo�� (or DebugOutputString)
		MessageBox(NULL,buf,L"Error",MB_OK);
		break;
	case DEBUG_OUT_DEBUGGER:
		//�f�o�b�K�ɏo��
		OutputDebugString(buf);
		break;
	}
}
//!�����I��
void debug_exit()
{
	if(!g_eDebugMode){
		exit(1);
	}else{
		exit(1);
	}
}

void debug_on_assert()
{
	if(g_pfAssertHandle){ g_pfAssertHandle(); }
}

#endif



