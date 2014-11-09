#include "common/_required.h"
#include "CPipe.h"
#include <exception>

//#define PIPE_SIZE 1
#define PIPE_SIZE 1024

CPipe::CPipe(int nSize)
: m_hRead(NULL)
, m_hWrite(NULL)
{
	//�Z�L�����e�B���
	SECURITY_ATTRIBUTES sa = {0};
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//�p�C�v�쐬
	BOOL bRet = ::CreatePipe(
		&m_hRead,
		&m_hWrite,
		&sa,
		nSize		//�p�C�v�̃T�C�Y�B0 ���w�肷��ƃf�t�H���g�B
	);
	if(!bRet)throw std::exception("CreatePipe failed.");
}

CPipe::~CPipe()
{
	_Dispose();
}

void CPipe::_Dispose()
{
	if(m_hRead){
		::CloseHandle(m_hRead);
		::CloseHandle(m_hWrite);
		m_hRead = NULL;
		m_hWrite = NULL;
	}
}


int CPipe::Peek()
{
	DWORD dwAvail = 0;
	if(::PeekNamedPipe(m_hRead, NULL, 0, NULL, &dwAvail, NULL) && dwAvail>0){
		return dwAvail;
	}
	else{
		return 0;
	}
}

int CPipe::Read(void* pData, int nDataLen, bool bWaitForSomething)
{
	if(!bWaitForSomething){
		if(Peek() <= 0){
			return 0;
		}
	}

	DWORD dwRead = 0;
	BOOL bRet = ::ReadFile(
		m_hRead,
		pData,
		nDataLen,
		&dwRead,		//�ǂݎ�����o�C�g��
		NULL
	);
	if(bRet){
		return dwRead;
	}
	else{
		return 0;
	}
}

int CPipe::ReadText(std::string* pStr, bool bWaitForSomething)
{
	char buf[1024*10]; //##��
	int n = Read(buf, _countof(buf) - 1, bWaitForSomething);
	buf[n] = 0;
	*pStr = buf;
	return (int)pStr->length();
}

int CPipe::ReadLine(std::string* pStr, bool bWaitForSomething)
{
	// �o�b�t�@�ɉ��s���܂܂�Ă���ΕԂ� //
	const char* p = m_strBuf.c_str();
	const char* q = strchr(p, '\n');
	if(q){
		pStr->assign(p, q); // �����s�͊܂߂Ȃ� //
		m_strBuf = q + 1;
		return (int)(q - p);
	}

	// �e�L�X�g��ǂ� //
	std::string str;
	int n = ReadText(&str, bWaitForSomething);
	if(n <= 0){
		*pStr = "";
		return 0;
	}

	// �o�b�t�@�ɒǉ� //
	m_strBuf += str;

	// ���� //
	return ReadLine(pStr, bWaitForSomething);
}
