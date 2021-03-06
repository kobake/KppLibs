#include "_required.h"
#include <winsock2.h> //windows.hより前にないとダメ
#include <windows.h>
#include <DebugLib.h>
#include <StringLib.h>
#include "std/myexception.h"
#include "CSocketAddress.h"
#include "CIpAddress.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          初期化                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CSocketAddress::CSocketAddress(const CIpAddress& ip,ushort port)
{
	m_saddr.sin_family=AF_INET;
	m_saddr.sin_port=htons(port);
	m_saddr.sin_addr=ip.getImp();
	memset(m_saddr.sin_zero,0,sizeof(m_saddr.sin_zero));
}


void CSocketAddress::_clear()
{
	memset(&m_saddr,0,sizeof(m_saddr));
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          演算子                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

bool CSocketAddress::operator == (const CSocketAddress& rhs) const
{
	if(this==&rhs)return true;
	if(memcmp(&m_saddr, &rhs.m_saddr, sizeof(m_saddr))==0)return true;
	return false;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           取得                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


CIpAddress CSocketAddress::getIpAddress() const
{
	return CIpAddress(m_saddr.sin_addr);
}

ushort CSocketAddress::getPort() const
{
	return ntohs(m_saddr.sin_port);
}