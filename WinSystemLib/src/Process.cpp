#include "_required.h"
#include "Process.h"
#include <windows.h>
#include <StringLib.h>

#define MY_MAX mmax(4*1024,_MAX_PATH)

//!�v���Z�X���쐬
HANDLE ProcessSystem::createProcess(const wchar_t* _cmdline)
{
	//CreateProcess�̑�2������const�z��͓n���Ȃ��̂ŁA�����ŏ����\�Ȕz�����蒼��
	static wchar_t cmdline[MY_MAX];
	wcssafecpy(cmdline,_cmdline,countof(cmdline));

	//STARTUPINFO�p��
	STARTUPINFO si;
	memset(&si,0,sizeof(si));
	si.cb = sizeof(si);

	//PROCESS_INFORMATION�p��
	PROCESS_INFORMATION pi;
	memset(&pi,0,sizeof(pi));

	//�v���Z�X�쐬
	BOOL ret=CreateProcess(
		NULL,    //���s�\���W���[���̖��O
		cmdline, //�R�}���h���C���̕�����
		NULL,    //�Z�L�����e�B�L�q�q
		NULL,    //�Z�L�����e�B�L�q�q
		FALSE,   //�n���h���̌p���I�v�V����
		0,       //�쐬�̃t���O
		NULL,    //�V�������u���b�N
		NULL,    //�J�����g�f�B���N�g���̖��O
		&si,     //�X�^�[�g�A�b�v���
		&pi      //�v���Z�X���
	);

	//�X���b�h�̃n���h���͎g��Ȃ��̂ŁA�N���[�Y���Ă���
	CloseHandle(pi.hThread);

	//�v���Z�X�̃n���h�������Ԃ�
	return pi.hProcess;
}


