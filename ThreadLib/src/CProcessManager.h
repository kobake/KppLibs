#pragma once

#include <string>

class CProcessManager{
public:
	// プロセス起動。終了するまで待ち、その標準出力を返す。//
	static std::string	RunProcessSync(const wchar_t* szCommand);

	// プロセス起動。終了を待たずにすぐに制御を返す。//
	static bool			RunProcessAsync(const wchar_t* szCommand);
};

