#pragma once

#include "CSocket.h"
#include "CDataSocket.h"
#include <string>
#include <LogLib.h>
class CIpAddress;

class Smtp{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	Smtp(bool _log_print);
	~Smtp();
	//�C���^�[�t�F�[�X
	void Connect(const wchar_t* _hostname); //!< �ڑ�
	void quit();                          //!< �ؒf
	void sendmail(
		const char* to,
		const char* subject,
		const char* body,
		const char* from
	);
private:
	Log log;
	std::wstring hostname;
	CSocket s2;
	CDataSocket s;
};

