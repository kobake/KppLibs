#pragma once

#include <windows.h> //DWORD
#include <vector>

/*
	自プロセスに関する情報取得
*/
class CMyProcess{
public:
	static std::vector<DWORD>	GetChildThreadIds();
};
