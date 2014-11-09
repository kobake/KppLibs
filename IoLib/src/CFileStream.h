#pragma once

#include "std/mem.h"

#include "CInputStream.h"

#include <cstdio> //FILE

class CFileInputStream : public CInputStream{
public:
	CFileInputStream(const wchar_t* path,const wchar_t* mode=L"rb"); //開けなかったら、Io::OpenExceptionを投げる
	CFileInputStream(FILE* _fp,bool _hold);
	~CFileInputStream();
	bool good() const;
	bool Eof() const;
	int Read(void* p,int size);
	void peek(void* p,int size);
	void skip(uint n);
	void seek(int n,SeekMode mode);
	uint tell() const;
	uint Available() const;
	void Close();

	//1バイト特化
	int get();
private:
	FILE* fp;
	bool fp_hold;
};


#include "COutputStream.h"

class CFileOutputStream : public COutputStream{
public:
	CFileOutputStream(const wchar_t* path,const wchar_t* mode=L"wb");
	~CFileOutputStream();
	bool good() const;
	int Write(const void* p,int size);
	void skip(uint n);
	void seek(int n,SeekMode mode);
	uint tell() const;
	void Close();

	//1バイト特化
	void put(int n);
private:
	FILE* fp;
};
