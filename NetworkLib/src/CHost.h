#pragma once

#include <vector>
#include <string>
#include "CIpAddress.h"
#include "std/myexception.h"

class CHost{
public:
	GEN_MYEXCEPTION(gethostname_failed);
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CHost(const char* _hostname=NULL);
	//�C���^�[�t�F�[�X
	const char*						GetName() const				{ return m_name.c_str(); } //!<���������擾
	const std::vector<std::string>&	GetAliases() const			{ return m_aliases;      } //!<�ʖ��̃��X�g���擾
	short							GetAddressFamily() const	{ return m_af;           } //!<�A�h���X�t�@�~�����擾�BAF_INET��Ԃ�
	const std::vector<CIpAddress>&	GetIpAddresses() const		{ return m_addresses;    } //!<IP�A�h���X�̃��X�g���擾
	CIpAddress						GetIpAddress() const		{ return m_addresses.size()?m_addresses[0]:CIpAddress();    } //!<IP�A�h���X(��\)���擾
private:
	std::string					m_name;
	std::vector<std::string>	m_aliases;
	short						m_af;
	std::vector<CIpAddress>		m_addresses;
};

#include "_lib_wsock32.h"
