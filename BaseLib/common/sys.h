#pragma once

#include <winsock2.h> //windows.hより前にないとダメ
#include <windows.h> //HINSTANCE, LPWSTR
//#include <DebugLib.h> // ASSERT_HANDLE
typedef void (*ASSERT_HANDLE)();

namespace BaseLib{

	// -- -- -- -- 初期化 -- -- -- -- //
	//main用
	void Init(
		int				argc,
		wchar_t*			argv[],
		ASSERT_HANDLE	pfAssertHandle = NULL
	);
	//WinMain用
	void Init(
		HINSTANCE		hInst,
		HINSTANCE		hPrevInst,
		LPWSTR			lpCmdLine,
		int				nCmdShow,
		ASSERT_HANDLE	pfAssertHandle = NULL
	);
	//DllMain用
	void Init(
		HINSTANCE		hInst,
		DWORD			fdwReason,
		PVOID			pvReserved,
		ASSERT_HANDLE	pfAssertHandle = NULL
	);

	//! COM初期化
	void InitCom();

	// -- -- -- -- インターフェース -- -- -- --
	HINSTANCE GetInstance();

} //namespace BaseLib

