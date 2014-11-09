#pragma once

#include "CInputStream.h"
#include <string>
#include <vector>

class CDataInputStream : public CFilterInputStream{
private:
	typedef std::wstring wstring;
public:
	//コンストラクタ・デストラクタ
	CDataInputStream(CInputStream* _in) : CFilterInputStream(_in) { }
	//プリミティブ
	byte ReadByte();
	short ReadShort();
	int ReadInt();
	//まるごと
	void ReadAll(std::vector<BYTE>* pvData);

	//構造体等
	template <class T> void ReadAny(T* t){ Read(t,sizeof(T)); }
	//文字
//	virtual int getc(){ return 0; }     //!< 1文字読む(char)
//	virtual wint_t getwc(){ return 0; } //!< 1文字読む(wchar_t)
	//文字列
	wstring readStringWithLength(); //!< 文字数を読み取り、文字数分のUNICODE文字列を読み取る。
};


#include "COutputStream.h"

class CDataOutputStream : public CFilterOutputStream{
public:
	//コンストラクタ・デストラクタ
	CDataOutputStream(COutputStream* _out) : CFilterOutputStream(_out) { }
	//プリミティブ
	void WriteByte(byte n);
	void WriteShort(short n);
	void WriteInt(int n);
	//構造体等
	template <class T> void writeAny(const T& t){ write(&t,sizeof(T)); }
	//文字列
	void WriteStringWithLength(const wchar_t* str); //!< 文字数を出力し、UNICODE文字列を出力。
};

