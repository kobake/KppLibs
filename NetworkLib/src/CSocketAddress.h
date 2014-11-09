#pragma once

#include "std/myexception.h"

class CIpAddress;
#include "CIpAddress.h" //�����Ă����v�����ǁA�������ق����֗�

class CSocketAddress{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CSocketAddress(){ _clear(); }
	CSocketAddress(const CIpAddress& ip,ushort port);
	CSocketAddress(const CSocketAddress& rhs){ m_saddr=rhs.m_saddr; }

	//���Z�q
	bool operator == (const CSocketAddress& rhs) const;

	//�v�f
	CIpAddress getIpAddress() const;
	ushort getPort() const;

	//�A�N�Z�X
	const sockaddr& getImp() const{ return *((sockaddr*)&m_saddr); }
	int getImpSize() const{ return sizeof(m_saddr); }

	//�J���p�F���
	void _assign(const sockaddr_in& _saddr){ m_saddr=_saddr; }
	void _clear();
private:
	sockaddr_in m_saddr;
};
