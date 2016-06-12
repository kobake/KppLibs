#pragma once

#include <wchar.h>
#include <BaseLib.h> // libfwd_win

class CWinError{
public:
	CWinError();
	void update();             //!<���O��Windows�G���[��ێ�����
	void clear();              //!<�ێ����Ă����G���[���������
	void show_error() const;   //!<�_�C�A���O�ŕ\��
	void throw_error() const;  //!<��O�𓊂���
	const wchar_t* what_w() const; //!<�G���[���b�Z�[�W���擾
public:
	static void s_getMessage(wchar_t* buf,size_t count,DWORD error_code);
private:
	DWORD error_code;
	wchar_t msg[256];
};

