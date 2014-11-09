#include "common/_required.h"
#include "CPipe.h"
#include <exception>

//#define PIPE_SIZE 1
#define PIPE_SIZE 1024

CPipe::CPipe(int nSize)
: m_hRead(NULL)
, m_hWrite(NULL)
{
	//セキュリティ情報
	SECURITY_ATTRIBUTES sa = {0};
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//パイプ作成
	BOOL bRet = ::CreatePipe(
		&m_hRead,
		&m_hWrite,
		&sa,
		nSize		//パイプのサイズ。0 を指定するとデフォルト。
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
		&dwRead,		//読み取ったバイト数
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
	char buf[1024*10]; //##仮
	int n = Read(buf, _countof(buf) - 1, bWaitForSomething);
	buf[n] = 0;
	*pStr = buf;
	return (int)pStr->length();
}

int CPipe::ReadLine(std::string* pStr, bool bWaitForSomething)
{
	// バッファに改行が含まれていれば返す //
	const char* p = m_strBuf.c_str();
	const char* q = strchr(p, '\n');
	if(q){
		pStr->assign(p, q); // ※改行は含めない //
		m_strBuf = q + 1;
		return (int)(q - p);
	}

	// テキストを読む //
	std::string str;
	int n = ReadText(&str, bWaitForSomething);
	if(n <= 0){
		*pStr = "";
		return 0;
	}

	// バッファに追加 //
	m_strBuf += str;

	// 続き //
	return ReadLine(pStr, bWaitForSomething);
}
