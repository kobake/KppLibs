#include "_required.h"
#include "CHost.h"
using namespace std;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CHost::CHost(const char* _hostname)
{
	Net::init();

	char** p;

	//�f�t�H���g�̃z�X�g��
	if(_hostname==NULL){
		static char buf[256];
		int ret=gethostname(buf,sizeof(buf));
		if(ret!=0){
			throw gethostname_failed(L"gethostname failed");
		}
		_hostname=buf;
	}

	//�z�X�g���擾
	hostent* ent=gethostbyname(_hostname);

	//������
	m_name=ent->h_name;

	//�ʖ��̃��X�g
	p=ent->h_aliases;
	while(*p){
		m_aliases.push_back(*p);
		p++;
	}

	//�A�h���X�t�@�~��
	m_af=ent->h_addrtype;

	//IP�A�h���X�̃��X�g
	p=ent->h_addr_list;
	while(*p){
		m_addresses.push_back(CIpAddress(*p,ent->h_length));
		p++;
	}
}


