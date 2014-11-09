#pragma once

#include <windows.h>
#include <string>

class CPipe{
public:
	CPipe(int nSize = 1024);
	~CPipe();
	void _Dispose(); //�����I�Ȕj��
	bool IsValid() const{ return m_hRead!=NULL; }
	HANDLE GetReadHandle() const{ return m_hRead; }
	HANDLE GetWriteHandle() const{ return m_hWrite; }

	// �ǂݎ�� //
	int Peek();
	int Read(void* pData, int nDataLen, bool bWaitForSomething);
	int ReadText(std::string* pStr, bool bWaitForSomething);

	// ���b�v //
	int ReadLine(std::string* pStr, bool bWaitForSomething);
private:
	HANDLE	m_hRead;
	HANDLE	m_hWrite;
	// ���b�v�p //
	std::string	m_strBuf;
};

