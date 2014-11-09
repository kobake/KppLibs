#pragma once

#include "Io.h"

class CInputStream : public Io{
public:
	virtual ~CInputStream(){}
	virtual bool good() const=0;              //!<エラーが無ければtrue
	virtual bool Eof() const=0;               //!<終端に達していたらtrue
	virtual int Read(void* p,int size)=0;   //!<読んでポインタを進める
	virtual void peek(void* p,int size)=0;   //!<読むけどポインタを進めない
	virtual void skip(uint n)=0;              //!<現在位置からシーク
	virtual void seek(int n,SeekMode mode)=0; //!<指定位置からシーク
	virtual uint tell() const=0;              //!<現在位置を取得
	virtual uint Available() const=0;         //!<現在位置から読めるバイト数を取得
	virtual void Close()=0;                   //!<入力を終了する
	//ラップ
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

