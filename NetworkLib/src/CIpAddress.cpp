#include "_required.h"
#include <winsock2.h> //windows.hより前にないとダメ
#include <windows.h>
#include <wchar.h>
#include <DebugLib.h>
#include <StringLib.h>
#include "CIpAddress.h"
#include "util/mem.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CIpAddress::_Set(const char* str)
{
	Net::init();

	//まずはinet_addrを試す
	m_addr.S_un.S_addr=inet_addr(str);
	if(m_addr.S_un.S_addr!=INADDR_NONE)return;
	//次にDNS解決を試す (WSAStartupがあらかじめ実行されている必要あり)
	hostent* p=gethostbyname(str);
	if(p==NULL){ memset(&m_addr, 0, sizeof(m_addr)); return; }//throw InvalidAddress(tmp_swprintf(L"アドレス[%hs]の解決ができませんでした",str));
	m_addr.S_un.S_addr=*((unsigned long*)p->h_addr_list[0]);
}

CIpAddress::CIpAddress()
{
	memset(&m_addr, 0, sizeof(m_addr));
}

bool CIpAddress::IsValid() const
{
	return !is_mem_zero(&m_addr, sizeof(m_addr));
}

CIpAddress::CIpAddress(const wchar_t* str)
{
	//マルチバイトに変換
	char buf[64];
	wcstombs(buf,str,countof(buf));
	_Set(buf);
}

CIpAddress::CIpAddress(const char* str)
{
	_Set(str);
}

CIpAddress::CIpAddress(const in_addr& _addr)
{
	m_addr=_addr;
}

CIpAddress::CIpAddress(const void* p,size_t len)
{
	//通常のIPならlen=4
	//IPv6ならlen=16 (※今のところIPv6はサポートしない)
	assert(len==4);

	//そのままコピー
	memcpy(&m_addr,p,len);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     インターフェース                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

const wchar_t* CIpAddress::to_string() const
{
	static wchar_t buf[(3+1)*4];
	swprintf(buf,L"%d.%d.%d.%d",m_addr.S_un.S_un_b.s_b1,m_addr.S_un.S_un_b.s_b2,m_addr.S_un.S_un_b.s_b3,m_addr.S_un.S_un_b.s_b4);
	return buf;
}
