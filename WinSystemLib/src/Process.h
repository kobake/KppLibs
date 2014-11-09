#pragma once

#include "sdk/libfwd_win.h" //HANDLE

class ProcessSystem{
public:
	// -- -- staticインターフェース -- -- //
	static HANDLE createProcess(const wchar_t* cmdline); //!<プロセスを作成
};

