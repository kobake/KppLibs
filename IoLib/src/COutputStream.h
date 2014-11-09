#pragma once

#include "Io.h"

class COutputStream : public Io{
public:
	virtual ~COutputStream(){}
	virtual bool good() const=0;
	virtual int Write(const void* p,int size)=0;
	virtual void skip(uint n)=0;
	virtual void seek(int n,SeekMode mode)=0;
	virtual uint tell() const=0;
	virtual void Close()=0;                     //!<出力を終了する
	//ラップ
	operator bool() const{ return good(); }
};

class CFilterOutputStream : public COutputStream{
public:
	CFilterOutputStream(COutputStream* out) : m_out(out) { }
	virtual bool good() const{ return m_out->good(); }
	virtual int Write(const void* p,int size){ return m_out->Write(p,size); }
	virtual void skip(uint n){ m_out->skip(n); }
	virtual void seek(int n,SeekMode mode){ m_out->seek(n,mode); }
	virtual uint tell() const{ return m_out->tell(); }
	virtual void Close(){ return m_out->Close(); }
private:
	COutputStream* m_out;
};

