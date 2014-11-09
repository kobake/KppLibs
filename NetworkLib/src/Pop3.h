#pragma once

class CIpAddress;
#include "CDataSocket.h"
#include <string>
#include <LogLib.h>

class Pop3{
private:
	typedef std::string mystring;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	Pop3(bool log_print); //!< log_print: ���O���R���\�[���ɏo�͂��邩�ǂ���
	~Pop3();
	//�C���^�[�t�F�[�X
	void Connect(const CIpAddress& ip);               //!< �T�[�o�ɐڑ�
	void login(const wchar_t* user,const wchar_t* pass); //!< ���O�C��
	void quit();                                     //!< �ʐM�I��
	mystring getMail(int no);
private:
	Log log;
	CSocket s2;
	CDataSocket s;
	int nmessage;
};
