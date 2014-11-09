#pragma once

#include <windows.h> //DWORD
#include <vector>

/*
	©ƒvƒƒZƒX‚ÉŠÖ‚·‚éî•ñæ“¾
*/
class CMyProcess{
public:
	static std::vector<DWORD>	GetChildThreadIds();
};
