#pragma once

#include "Io.h"

class CInputStream : public Io{
public:
	virtual ~CInputStream(){}
	virtual bool good() const=0;              //!<�G���[���������true
	virtual bool Eof() const=0;               //!<�I�[�ɒB���Ă�����true
	virtual int Read(void* p,int size)=0;   //!<�ǂ�Ń|�C���^��i�߂�
	virtual void peek(void* p,int size)=0;   //!<�ǂނ��ǃ|�C���^��i�߂Ȃ�
	virtual void skip(uint n)=0;              //!<���݈ʒu����V�[�N
	virtual void seek(int n,SeekMode mode)=0; //!<�w��ʒu����V�[�N
	virtual uint tell() const=0;              //!<���݈ʒu���擾
	virtual uint Available() const=0;         //!<���݈ʒu����ǂ߂�o�C�g�����擾
	virtual void Close()=0;                   //!<���͂��I������
	//���b�v
	operator bool() const{ return good(); }
};

class CFilterInputStream : public CInputStream{
public:
	CFilterInputStream(CInputStream* _in) : m_in(_in) { }
	virtual bool good() const{ return m_in->good(); }
	virtual bool Eof() const{ return m_in->Eof(); }
	virtual int Read(void* p,int size){ return m_in->Read(p,size); }
	virtual void peek(void* p,int size){ m_in->peek(p,size); }
	virtual void skip(uint n){ m_in->skip(n); }
	virtual void seek(int n,SeekMode mode){ m_in->seek(n,mode); }
	virtual uint tell() const{ return m_in->tell(); }
	virtual uint Available() const{ return m_in->Available(); }
	virtual void Close(){ return m_in->Close(); }
private:
	CInputStream* m_in;
};

