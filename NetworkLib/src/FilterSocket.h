#pragma once

#include "CSocket.h"
#include "CIpAddress.h"

//! CSocket�����b�v����N���X
class FilterSocket{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	FilterSocket(CSocket* _s) : s(_s) { }

	//���
	bool valid() const{ return s->valid(); }

	//�N���C�A���g�C���^�[�t�F�[�X
	void Connect(const CSocketAddress& addr){ return s->Connect(addr); }

	//�T�[�o�C���^�[�t�F�[�X
	void bind(ushort port){ return s->bind(port); }
	void listen(){ return s->listen(); }
	CSocket* accept(){ return s->accept(); }

	//���ʃC���^�[�t�F�[�X
	int send(const void* data,uint len,int flags=0){ return s->send(data,len,flags); }
	int recv(void* buf,uint len,int flags=0){ return s->recv(buf,len,flags); }
	void Close(){ return s->Close(); }
	const CSocketAddress& getAddress() const{ return s->getAddress(); }

	//���̑�
	void shutdown(){ return s->shutdown(); }

	//�@�\
	CIpAddress getLocalAddress() const{ return s->getLocalAddress(); }
	CIpAddress getRemoteAddress() const{ return s->getRemoteAddress(); }

private:
	CSocket* s; //!<���b�v����\�P�b�g�ւ̎Q��
};
