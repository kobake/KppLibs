#pragma once

#include "std/myexception.h"

class CIpAddress;
#include "CIpAddress.h" //無くても大丈夫だけど、あったほうが便利

class CSocketAddress{
public:
	//コンストラクタ・デストラクタ
	CSocketAddress(){ _clear(); }
	CSocketAddress(const CIpAddress& ip,ushort port);
	CSocketAddress(const CSocketAddress& rhs){ m_saddr=rhs.m_saddr; }

	//演算子
	bool operator == (const CSocketAddress& rhs) const;

	//要素
	CIpAddress getIpAddress() const;
	ushort getPort() const;

	//アクセス
	const sockaddr& getImp() const{ return *((sockaddr*)&m_saddr); }
	int getImpSize() const{ return sizeof(m_saddr); }

	//開発用：代入
	void _assign(const sockaddr_in& _saddr){ m_saddr=_saddr; }
	void _clear();
private:
	sockaddr_in m_saddr;
};
