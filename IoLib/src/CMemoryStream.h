#pragma once


#include <BaseLib.h>

#include "CInputStream.h"

class CMemoryInputStream : public CInputStream{
public:
	CMemoryInputStream(const void* p,uint len);
	CMemoryInputStream(Memory mem);
	bool good() const;
	bool Eof() const;
	int Read(void* p,int size);
	void peek(void* p,int size);
	void skip(uint n);
	void seek(int n,SeekMode mode);
	uint tell() const;
	uint Available() const;
	void Close();
private:
	const char*	m_pData;
	uint		m_nDataLen;
	const char*	m_pCur;
};


#include "COutputStream.h"

class CMemoryOutputStream : public COutputStream{
public:
	CMemoryOutputStream(void* p,uint len);
	CMemoryOutputStream(Memory mem);
	bool good() const;
	int Write(const void* p,int size);
	void skip(uint n);
	void seek(int n,SeekMode mode);
	uint tell() const;
	void Close();
private:
	char*	m_pData;
	uint	m_nDataLen;
	char*	m_pCur;
};
