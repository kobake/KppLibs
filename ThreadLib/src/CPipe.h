#pragma once

#include <windows.h>
#include <string>

class CPipe{
public:
	CPipe(int nSize = 1024);
	~CPipe();
	void _Dispose(); //明示的な破棄
	bool IsValid() const{ return m_hRead!=NULL; }
	HANDLE GetReadHandle() const{ return m_hRead; }
	HANDLE GetWriteHandle() const{ return m_hWrite; }

	// 読み取り //
	int Peek();
	int Read(void* pData, int nDataLen, bool bWaitForSomething);
	int ReadText(std::string* pStr, bool bWaitForSomething);

	// ラップ //
	int ReadLine(std::string* pStr, bool bWaitForSomething);
private:
	HANDLE	m_hRead;
	HANDLE	m_hWrite;
	// ラップ用 //
	std::string	m_strBuf;
};

