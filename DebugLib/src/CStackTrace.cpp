#include "CStackTrace.h"
#include <dbghelp.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

#pragma comment(lib, "Dbghelp.lib") 


//
// �f�t�H���g�R���X�g���N�^
//
CStackTrace::CStackTrace(void)
{
}


//
// �f�X�g���N�^
//
CStackTrace::~CStackTrace(void)
{
}


//
// ���W���[����ǂݍ��݂܂��B
//
// �����F[i] hProcess ... �v���Z�X�n���h��
//           dwProcessId ... �v���Z�XID
//
// �߂�l�F����
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

		// ���W���[�����擾
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
// �X�^�b�N�g���[�X��\�����܂��B
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


	// ������
	if (!SymInitialize(hProcess, NULL, FALSE))
	{
		throw;
	}

	// ���W���[���ǂݍ���
	this->LoadModules(hProcess, dwProcessId);

	// Context �擾
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
		// �X�^�b�N�t���[�����擾
		if (!StackWalk64(machineType, hProcess, hThread, &stackFrame,
						&context, NULL, NULL, NULL, NULL))
			break;

		// �X�^�b�N�t���[��������
		if (stackFrame.AddrPC.Offset == stackFrame.AddrReturn.Offset)
		{
			// �G���h���X�ɂȂ�̂ŏI���
			break;
		}
		if (stackFrame.AddrPC.Offset == 0)
		{
			// �s���ȃX�^�b�N�t���[��
			break;
		}       
		if (stackFrame.AddrReturn.Offset == 0)
		{
			// �Ō�̃X�^�b�N�t���[��
			break;
		}

		// �t�@�C�����A�s���A���\�b�h�����擾
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
