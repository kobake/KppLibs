#include "_required.h"
#include <winsock2.h> //windows.h���O�ɂȂ��ƃ_��
#include <windows.h>
#include <DebugLib.h>
#include "std/myexception.h"
#include <StringLib.h>
#include "CSocket.h"
#include "CSocketAddress.h"
#include "CIpAddress.h"
#include "util/std.h"



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CSocket::CSocket()
{
	Net::init();
	m_s=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_s==INVALID_SOCKET)throw init_failed(L"socket failed");
}

//! SOCKET�����b�v����ŁB�f�X�g���N�^��SOCKET�̓N���[�Y�����B
CSocket::CSocket(SOCKET _s,const sockaddr_in& _saddr)
: m_s(_s)
{
	Net::init();
	if(m_s==INVALID_SOCKET)throw init_failed(L"socket failed");

	current_address._assign(_saddr);
}


//########���ׂẴ\�P�b�g���I��������WSACleanup���Ăяo��(Net�N���X�ɈϏ�)�ׂ������H
CSocket::~CSocket()
{
	if(m_s!=INVALID_SOCKET){
		int ret=closesocket(m_s);
		assert(ret==0);
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ���                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	
bool CSocket::valid() const
{
	return m_s!=INVALID_SOCKET;
}

//! �ڑ����Ă���z�X�g�������true
bool CSocket::isConnected() const
{
	return current_address.getPort()!=0;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �N���C�A���g�C���^�[�t�F�[�X                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CSocket::Connect(const CSocketAddress& addr)
{
	int ret=::connect(m_s,&addr.getImp(),addr.getImpSize());
	if(ret<0)throw connect_failed(L"connect failed");

	current_address=addr;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  �T�[�o�C���^�[�t�F�[�X                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//! ���X���҂���
void CSocket::bind(ushort port)
{
	SOCKADDR_IN saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(port);
	saddr.sin_addr.s_addr=INADDR_ANY;
	int ret=::bind(m_s,(sockaddr*)&saddr,sizeof(saddr));
	if(ret==SOCKET_ERROR){
		throw wexception(L"bind failed");
	}

	current_address._assign(saddr);
}

//! �҂�����
void CSocket::listen()
{
	int ret=::listen(m_s,0);
	if(ret==SOCKET_ERROR){
		throw wexception(L"listen failed");
	}
}

//! �N���C�A���g��t
CSocket* CSocket::accept()
{
	SOCKADDR_IN from;
	int from_len=sizeof(from);
	SOCKET client_s=::accept(m_s,(SOCKADDR*)&from,&from_len);
	if(client_s==INVALID_SOCKET){
		throw wexception(L"accept failed");
	}

	CSocket* new_s=new CSocket(client_s,from);
	return new_s;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                    ���ʃC���^�[�t�F�[�X                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int CSocket::send(const void* data,uint len,int flags)
{
	int ret=::send(m_s,(const char*)data,len,flags);
	if(ret==-1)throw send_failed(L"send failed");
	return ret;
}


int CSocket::recv(void* buf,uint len,int flags)
{
	int ret=::recv(m_s,(char*)buf,len,flags);
	if(ret==-1)throw recv_failed(L"recv failed");
	return ret;
}



void CSocket::Close()
{
	if(m_s!=0){
		int ret=closesocket(m_s);
		if(ret<0){
			throw close_failed(L"closesocket failed");
		}else{
			m_s=INVALID_SOCKET;
		}
	}

	current_address._clear();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          ���̑�                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CSocket::shutdown()
{
	int ret=::shutdown(m_s,SD_BOTH);
	if(ret!=0){
		throw shutdown_failed(L"shutdown failed");
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �@�\                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!������IP�A�h���X���擾
CIpAddress CSocket::getLocalAddress() const
{
	sockaddr_in addr;
	int len=sizeof(addr);
	int ret=getsockname(m_s,(sockaddr*)&addr,&len);
	if(ret!=0){
		throw getsockname_failed(L"getsockname failed");
	}
	return CIpAddress(addr.sin_addr);
}

//!�����IP�A�h���X���擾
CIpAddress CSocket::getRemoteAddress() const
{
	sockaddr_in addr;
	int len=sizeof(addr);
	int ret=getpeername(m_s,(sockaddr*)&addr,&len);
	if(ret!=0){
		throw getpeername_failed(L"getpeername failed");
	}
	return CIpAddress(addr.sin_addr);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        �g���N���X                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
#include <StringLib.h>
void CSocketSender::SendTextW(const wchar_t* pText, int nLen)
{
	char bufDst[1024];
	int nDst = 0;
	const wchar_t* pSrc = pText;
	int nSrc = nLen;
	while(nSrc>0){
		int nSrcConv = min<int>(nSrc, _countof(bufDst)/2-1);
		nDst = ::WideCharToMultiByte(
			CP_ACP,
			0,
			pSrc,
			nSrcConv,
			bufDst,
			_countof(bufDst)-1,
			NULL,
			NULL
		);
		pSrc+=nSrcConv;
		nSrc-=nSrcConv;
		m_pcSocketRef->send(bufDst, nDst);
	}
}

