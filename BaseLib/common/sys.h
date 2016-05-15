#pragma once

#include <winsock2.h> //windows.h���O�ɂȂ��ƃ_��
#include <windows.h> //HINSTANCE, LPWSTR
//#include <DebugLib.h> // ASSERT_HANDLE
typedef void (*ASSERT_HANDLE)();

namespace BaseLib{

	// -- -- -- -- ������ -- -- -- -- //
	//main�p
	void Init(
		int				argc,
		wchar_t*			argv[],
		ASSERT_HANDLE	pfAssertHandle = NULL
	);
	//WinMain�p
	void Init(
		HINSTANCE		hInst,
		HINSTANCE		hPrevInst,
		LPWSTR			lpCmdLine,
		int				nCmdShow,
		ASSERT_HANDLE	pfAssertHandle = NULL
	);
	//DllMain�p
	void Init(
		HINSTANCE		hInst,
		DWORD			fdwReason,
		PVOID			pvReserved,
		ASSERT_HANDLE	pfAssertHandle = NULL
	);

	//! COM������
	void InitCom();

	// -- -- -- -- �C���^�[�t�F�[�X -- -- -- --
	HINSTANCE GetInstance();

} //namespace BaseLib

