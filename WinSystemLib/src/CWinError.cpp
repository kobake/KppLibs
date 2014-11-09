#include "_required.h"
#include "CWinError.h"
#include "std/myexception.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CWinError::CWinError()
{
	update();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     �C���^�[�t�F�[�X                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!���O��Windows�G���[��ێ�����
void CWinError::update()
{
	error_code=GetLastError();

	s_getMessage(msg,countof(msg),error_code);
}

//!�ێ����Ă����G���[���������
void CWinError::clear()
{
	error_code=0;
	msg[0]='\0';
}

//!�_�C�A���O�ŕ\��
void CWinError::show_error() const
{
	MessageBox(NULL, msg, L"Error", MB_OK | MB_ICONINFORMATION);
}

//!��O�𓊂���
void CWinError::throw_error() const
{
	throw myexception(msg);
}

//!�G���[���b�Z�[�W���擾
const wchar_t* CWinError::what_w() const
{
	return msg;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          static                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CWinError::s_getMessage(wchar_t* buf,size_t count,DWORD error_code)
{
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error_code,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), //�K��̌���
		buf,
		(DWORD)count,
		NULL
	);
}

