#pragma once

#include <vector>
#include <string>
#include "CIpAddress.h"
#include "std/myexception.h"

class CHost{
public:
	GEN_MYEXCEPTION(gethostname_failed);
public:
	//コンストラクタ・デストラクタ
	CHost(const char* _hostname=NULL);
	//インターフェース
	const char*						GetName() const				{ return m_name.c_str(); } //!<正式名を取得
	const std::vector<std::string>&	GetAliases() const			{ return m_aliases;      } //!<別名のリストを取得
	short							GetAddressFamily() const	{ return m_af;           } //!<アドレスファミリを取得。AF_INETを返す
	const std::vector<CIpAddress>&	GetIpAddresses() const		{ return m_addresses;    } //!<IPアドレスのリストを取得
	CIpAddress						GetIpAddress() const		{ return m_addresses.size()?m_addresses[0]:CIpAddress();    } //!<IPアドレス(代表)を取得
private:
	std::string					m_name;
	std::vector<std::string>	m_aliases;
	short						m_af;
	std::vector<CIpAddress>		m_addresses;
};

#include "_lib_wsock32.h"
