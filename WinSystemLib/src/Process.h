#pragma once

#include <BaseLib.h> // libfwd_win(HANDLE)

class ProcessSystem{
public:
	// -- -- static�C���^�[�t�F�[�X -- -- //
	static HANDLE createProcess(const wchar_t* cmdline); //!<�v���Z�X���쐬
};

