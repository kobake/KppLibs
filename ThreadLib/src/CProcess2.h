/*
	�p�C�v����ē��o�͂��s���^�C�v��
	�v���Z�X�N�����W���[��
*/
#pragma once

#include <windows.h>

/*
	�v���Z�X�N��
	CProcess2 process("hoge.exe hoge hoge", NULL, NULL, NULL);

	�f�X�g���N�^�̓v���Z�X�n���h����������邾���Ȃ̂ŁA�v���Z�X�������I��������킯�ł͂Ȃ��B
*/
class CProcess2{
public:
	CProcess2(
		const wchar_t*	szCommand,
		HANDLE			hStdOutput,
		HANDLE			hStdError,
		HANDLE			hStdInput
	);
	~CProcess2();

	// ��� //
	bool	IsAlive() const
	{
		DWORD n = ::WaitForSingleObject(m_hProcess, 0);
		return n == WAIT_TIMEOUT;
	}

	// ����� //
	HANDLE _GetHandle() const{ return m_hProcess; }
private:
	HANDLE m_hProcess;
};

