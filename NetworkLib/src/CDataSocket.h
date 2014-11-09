#pragma once

#include "FilterSocket.h"

//!CSocket�ɋ@�\��ǉ������ŁB
class CDataSocket : public FilterSocket{
public:
	CDataSocket(CSocket* s) : FilterSocket(s) { }
	//�C���^�[�t�F�[�X
	int sendf(const char* str,...);                   //!<�����񑗐M
	int recv_smart(void* buf,uint len,int flags=0);   //!<������Ƃ���M
	std::string recv_string();                        //!<���������M�ł��邾����M
};
