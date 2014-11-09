#pragma once

#include <windows.h>
#include <string>

//
// スタックトレースの出力を提供します。
//
class CStackTrace{
public:
	CStackTrace(void);
	~CStackTrace(void);
	void		Show();
	std::string	GetInfo();

private:
	bool LoadModules(HANDLE hProcess, DWORD dwProcessId);
};
