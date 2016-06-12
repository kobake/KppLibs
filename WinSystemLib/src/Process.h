#pragma once

#include <BaseLib.h> // libfwd_win(HANDLE)

class ProcessSystem{
public:
	// -- -- staticインターフェース -- -- //
	static HANDLE createProcess(const wchar_t* cmdline); //!<プロセスを作成
};

