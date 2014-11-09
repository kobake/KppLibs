#pragma once

class CSocketAddress;
class CIpAddress;

#include "_head.h"
#include "std/myexception.h"
#include "CSocketAddress.h"

class CSocket{
public:
	GEN_MYEXCEPTION(CSocketException);
	GEN_ARGSEXCEPTION(init_failed,			CSocketException);
	GEN_ARGSEXCEPTION(connect_failed,		CSocketException);
	GEN_ARGSEXCEPTION(send_failed,			CSocketException);
	GEN_ARGSEXCEPTION(recv_failed,			CSocketException);
	GEN_ARGSEXCEPTION(close_failed,			CSocketException);
	GEN_ARGSEXCEPTION(getsockname_failed,	CSocketException);
	GEN_ARGSEXCEPTION(getpeername_failed,	CSocketException);
	GEN_ARGSEXCEPTION(shutdown_failed,		CSocketException);
public:
	static void s_setup();
private:
	//����֎~
	CSocket(const CSocket&);
	CSocket& operator = (const CSocket&);
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CSocket();
	explicit CSocket(SOCKET _s,const sockaddr_in& _saddr); //!< SOCKET�����b�v����ŁB�f�X�g���N�^��SOCKET�̓N���[�Y�����B
	~CSocket();

	//���
	bool valid() const;       //!< SOCKET�n���h�����L���Ȃ�true
	bool isConnected() const; //!< �ڑ����Ă���z�X�g�������true

	//�N���C�A���g�C���^�[�t�F�[�X
	void Connect(const CSocketAddress& addr);          //!<�T�[�o�ɐڑ�

	//�T�[�o�C���^�[�t�F�[�X
	void bind(ushort port);                     //!< �|�[�g�Ƀo�C���h
	void listen();                              //!< ���X�����[�h��
	CSocket* accept();                           //!< �N���C�A���g��t�B�V����CSocket�C���X�^���X��Ԃ��̂ŁA�g���I�������delete����K�v������B

	//���ʃC���^�[�t�F�[�X
	int send(const void* data,uint len,int flags=0);  //!<���M
	int recv(void* buf,uint len,int flags=0);         //!<��M
	void Close();                                     //!<�ڑ��I��
	const CSocketAddress& getAddress() const{ return current_address; } //!< �A�h���X���擾

	//���̑�
	void shutdown();

	//�@�\
	CIpAddress getLocalAddress() const;  //!<������IP�A�h���X���擾�B�ڑ����m�����Ă��Ȃ��ƃG���[�B
	CIpAddress getRemoteAddress() const; //!<�����IP�A�h���X���擾�B�ڑ����m�����Ă��Ȃ��ƃG���[�B

	//���l
	SOCKET _GetHandle() const{ return m_s; }
private:
	SOCKET m_s;
	CSocketAddress current_address;
};

class CSocketSender{
public:
	CSocketSender(CSocket& cSocket) : m_pcSocketRef(&cSocket) { }
	void SendTextW(const wchar_t* pText, int nLen);
private:
	CSocket* m_pcSocketRef;
};

#include "_lib_wsock32.h"
