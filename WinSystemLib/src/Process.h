#pragma once

#include "sdk/libfwd_win.h" //HANDLE

class ProcessSystem{
public:
	// -- -- static�C���^�[�t�F�[�X -- -- //
	static HANDLE createProcess(const wchar_t* cmdline); //!<�v���Z�X���쐬
};

