#include "common/_required.h"
#include "CProcess2.h"

CProcess2::CProcess2(
	const wchar_t*	szCommand,
	HANDLE			hStdOutput,
	HANDLE			hStdError,
	HANDLE			hStdInput
)
{
	//�v���Z�X�쐬
	PROCESS_INFORMATION pi = {0};
	{
		//�X�^�[�g�A�b�v���
		STARTUPINFO si = {0};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hStdOutput;
		si.hStdError = hStdError;
		si.hStdInput = hStdInput;

		//�R�}���h���C��
		wchar_t bufCommand[_MAX_PATH];
		wcscpy(bufCommand, szCommand); //###

		//�쐬
		BOOL bRet = ::CreateProcess(
			NULL,				//���s�\���W���[���̖��O
			bufCommand,			//�R�}���h���C��������
			NULL,				//�Z�L�����e�B�L�q�q (�v���Z�X)
			NULL,				//�Z�L�����e�B�L�q�q (�X���b�h)
			TRUE,				//�n���h���̌p���I�v�V����
			CREATE_NO_WINDOW,	//�쐬�̃t���O (�R���\�[���A�v���P�[�V�������R���\�[���E�B���h�E�Ȃ��Ŏ��s�����܂��B)
			NULL,				//�V�������u���b�N
			NULL,				//�J�����g�f�B���N�g��
			&si,				//�X�^�[�g�A�b�v���
			&pi					//�v���Z�X���
		);

		//�K�v�ȃn���h���ێ�
		m_hProcess = pi.hProcess;

		//�s�v�ȃn���h�����
		::CloseHandle(pi.hThread);
	}
}

CProcess2::~CProcess2()
{
	::CloseHandle(m_hProcess);
}
