#pragma once

#include "std/myexception.h"
#include "_head.h"

class CIpAddress{
public:
	GEN_MYEXCEPTION(InvalidAddress);
public:
	void _Set(const char* str);
	//�R���X�g���N�^�E�f�X�g���N�^
	CIpAddress();
	CIpAddress(const char* str);
	CIpAddress(const wchar_t* str);
	CIpAddress(const in_addr& _addr);
	CIpAddress(const void* p,size_t len);
	bool IsValid() const;
	//�A�N�Z�X
	const in_addr& getImp() const{ return m_addr; }
	//�\��
	const wchar_t* to_string() const;
private:
	in_addr m_addr;
};

#define _WINSOCKAPI_ //windows.h��winsock.h���C���N���[�h����Ă��܂��̂����

#include "_lib_wsock32.h"
