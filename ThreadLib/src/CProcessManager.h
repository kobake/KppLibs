#pragma once

#include <string>

class CProcessManager{
public:
	// �v���Z�X�N���B�I������܂ő҂��A���̕W���o�͂�Ԃ��B//
	static std::string	RunProcessSync(const wchar_t* szCommand);

	// �v���Z�X�N���B�I����҂����ɂ����ɐ����Ԃ��B//
	static bool			RunProcessAsync(const wchar_t* szCommand);
};

