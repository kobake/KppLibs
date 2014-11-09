#include "CStackTrace.h"
#include <dbghelp.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

#pragma comment(lib, "Dbghelp.lib") 


//
// デフォルトコンストラクタ
//
CStackTrace::CStackTrace(void)
{
}


//
// デストラクタ
//
CStackTrace::~CStackTrace(void)
{
}


//
// モジュールを読み込みます。
//
// 引数：[i] hProcess ... プロセスハンドル
//           dwProcessId ... プロセスID
//
// 戻り値：結果
//
bool CStackTrace::LoadModules(HANDLE hProcess, DWORD dwProcessId)
{
	HANDLE hSnap;

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);

	if (hSnap == (HANDLE)(-1))
	{
		return false;
	}

	try
	{
		BOOL loop;
		MODULEENTRY32 entry;
		entry.dwSize = sizeof(entry);

		// モジュールを取得
		loop = Module32First(hSnap, &entry);

		while (loop)
		{
			TCHAR* image;
			TCHAR* module;

			image = _tcsdup(entry.szExePath);
			module = _tcsdup(entry.szModule);

			SymLoadModule64(hProcess, NULL, (PSTR)(image), (PSTR)(module),
							(DWORD64)(entry.modBaseAddr),
							static_cast<DWORD>(entry.modBaseSize));

			loop = Module32Next(hSnap, &entry);
		}


		CloseHandle(hSnap);
	}
	catch (...)
	{
		CloseHandle(hSnap);
	}


	return true;
}


//
// スタックトレースを表示します。
//
void CStackTrace::Show()
{
	printf("%s", GetInfo().c_str());
}

std::string CStackTrace::GetInfo()
{
	char buf[1024];
	std::string ret;
	HANDLE hProcess = GetCurrentProcess();
	DWORD dwProcessId = GetCurrentProcessId();


	// 初期化
	if (!SymInitialize(hProcess, NULL, FALSE))
	{
		throw;
	}

	// モジュール読み込み
	this->LoadModules(hProcess, dwProcessId);

	// Context 取得
	HANDLE hThread = GetCurrentThread();
	CONTEXT context;

	memset(&context, 0, sizeof(context));
	context.ContextFlags = CONTEXT_FULL;


#if defined(_M_IX86)
	__asm    call(x);
	__asm x: pop eax;
	__asm    mov context.Eip, eax;
	__asm    mov context.Ebp, ebp;
	__asm    mov context.Esp, esp;
#else
	// for Windows 64-bit editions
	RtlCaptureContext(&context);
#endif


	STACKFRAME64 stackFrame;
	memset(&stackFrame, 0, sizeof(stackFrame));

#if defined(_M_IX86)
	stackFrame.AddrPC.Offset = context.Eip;
	stackFrame.AddrPC.Mode = AddrModeFlat;
	stackFrame.AddrFrame.Offset = context.Ebp;
	stackFrame.AddrFrame.Mode = AddrModeFlat;
	stackFrame.AddrStack.Offset = context.Esp;
	stackFrame.AddrStack.Mode = AddrModeFlat;
	DWORD machineType = IMAGE_FILE_MACHINE_I386;
#elif defined(_M_AMD64)
	stackFrame.AddrPC.Offset = context.Rip;
	stackFrame.AddrPC.Mode = AddrModeFlat;
	stackFrame.AddrFrame.Offset = context.Rbp;
	stackFrame.AddrFrame.Mode = AddrModeFlat;
	stackFrame.AddrStack.Offset = context.Rsp;
	stackFrame.AddrStack.Mode = AddrModeFlat;
	DWORD machineType = IMAGE_FILE_MACHINE_AMD64;
#elif defined(_M_IA64)
	stackFrame.AddrPC.Offset = context.StIIP;
	stackFrame.AddrPC.Mode = AddrModeFlat;
	stackFrame.AddrStack.Offset = context.IntSp;
	stackFrame.AddrStack.Mode = AddrModeFlat;
	stackFrame.AddrBStore.Offset = context.RsBSP;
	stackFrame.AddrBStore.Mode = AddrModeFlat;
	DWORD machineType = IMAGE_FILE_MACHINE_IA64;
#else
#error "Not Supported."
#endif


	for ( ; ; )
	{
		// スタックフレームを取得
		if (!StackWalk64(machineType, hProcess, hThread, &stackFrame,
						&context, NULL, NULL, NULL, NULL))
			break;

		// スタックフレームを検証
		if (stackFrame.AddrPC.Offset == stackFrame.AddrReturn.Offset)
		{
			// エンドレスになるので終わり
			break;
		}
		if (stackFrame.AddrPC.Offset == 0)
		{
			// 不正なスタックフレーム
			break;
		}       
		if (stackFrame.AddrReturn.Offset == 0)
		{
			// 最後のスタックフレーム
			break;
		}

		// ファイル名、行数、メソッド名を取得
		DWORD64 displacement64;
		ULONG64 buffer[(sizeof(SYMBOL_INFO) +
			MAX_SYM_NAME * sizeof(TCHAR) +
			sizeof(ULONG64) - 1) /
			sizeof(ULONG64)];
		PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;

		pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		pSymbol->MaxNameLen = MAX_SYM_NAME;

		if (SymFromAddr(hProcess, stackFrame.AddrReturn.Offset, 
						&displacement64, pSymbol))
		{
			DWORD displacement;
			IMAGEHLP_LINE64 Line;
			memset(&Line, 0, sizeof(Line));
			Line.SizeOfStruct = sizeof(Line);

			if (SymGetLineFromAddr64(hProcess, stackFrame.AddrReturn.Offset,
									&displacement, &Line))
			{
				sprintf_s(buf, _countof(buf), "%s (%lu): %s\n", Line.FileName, Line.LineNumber,
						pSymbol->Name);
				ret += buf;
			}
			else
			{
				sprintf(buf, "??? (---): %s\n", pSymbol->Name);
				ret += buf;
			}
		}
		else
		{
			sprintf(buf, "Failed to retrieve symbol information: 0x%08x\n",
					stackFrame.AddrReturn.Offset);
			ret += buf;
		}
	}
	return ret;
}
