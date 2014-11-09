#include "_required.h"
#include "CHost.h"
using namespace std;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CHost::CHost(const char* _hostname)
{
	Net::init();

	char** p;

	//デフォルトのホスト名
	if(_hostname==NULL){
		static char buf[256];
		int ret=gethostname(buf,sizeof(buf));
		if(ret!=0){
			throw gethostname_failed(L"gethostname failed");
		}
		_hostname=buf;
	}

	//ホスト情報取得
	hostent* ent=gethostbyname(_hostname);

	//公式名
	m_name=ent->h_name;

	//別名のリスト
	p=ent->h_aliases;
	while(*p){
		m_aliases.push_back(*p);
		p++;
	}

	//アドレスファミリ
	m_af=ent->h_addrtype;

	//IPアドレスのリスト
	p=ent->h_addr_list;
	while(*p){
		m_addresses.push_back(CIpAddress(*p,ent->h_length));
		p++;
	}
}


