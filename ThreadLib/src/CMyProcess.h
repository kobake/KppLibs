#pragma once

#include <windows.h> //DWORD
#include <vector>

/*
	���v���Z�X�Ɋւ�����擾
*/
class CMyProcess{
public:
	static std::vector<DWORD>	GetChildThreadIds();
};
