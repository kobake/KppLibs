#include "common/_required.h"
#include "CLog.h"
#include <stdio.h>
#include <map>
#include "common/CAppInfo.h"
#include <StringLib.h>
#include "CLogType.h"
#include <c2lib.h>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// CLog
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

/*
	[2010/02/20 12:31:40][INFO][Abc.cpp:80:CHoge::Do] Hello
*/
void CLog::PrintV(CLogType& cType, const char* str, va_list v, bool bMsg)
{
	char buf[1024];
	int n = 0;
	char* p = buf;

	//�Ăяo�����t�@�C�����̃f�B���N�g�����������
	const char* szFileName = m_szFileName;
	{
		const char* p = strrchr(szFileName, '\\');
		if(p){
			szFileName = p + 1;
		}
	}

	//���ݓ����A�v���Z�X���A���O��A�Ăяo����
	SYSTEMTIME st;
	GetLocalTime(&st);
	n = sprintf(
		p,
		"[%04d/%02d/%02d %02d:%02d:%02d.%03d][%d:%ls][%hs][%hs:%d:%hs] ",
		st.wYear, st.wMonth,  st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
		::GetCurrentProcessId(),
		CAppInfo::Instance()->GetExeName(),
		cType.GetName(),
		szFileName,
		m_nLineNo,
		m_szFunctionName
	);
	p+=n;

	// ���b�Z�[�W
	const char* pWinMessage = p;
	n = vsprintf(p,str,v);
	p+=n;

	//����
	p=p-1;
	while(p>=buf && (*p=='\r' || *p=='\n'))p--; //���̉��s������
	p++;
	*p='\0';

	//���O�^�C�v�ɂ��o��
	cType.Print(buf);

	// ���b�Z�[�W
	if(bMsg){
		std::string strCaption = cpp_wcstombs(CAppInfo::Instance()->GetExeNameWithoutExt());
		::MessageBoxA(NULL, pWinMessage, strCaption.c_str(), MB_OK);
	}
}

void CLog::Print(CLogType& cType, const char* str, ...)
{
	if(cType.GetTarget() == LOG_TARGET_NONE)return;
	va_list v;
	va_start(v,str);
	PrintV(cType, str, v, false);
	va_end(v);
}

void CLog::PrintAndMessage(CLogType& cType, const char* str, ...)
{
	if(cType.GetTarget() == LOG_TARGET_NONE)return;
	va_list v;
	va_start(v,str);
	PrintV(cType, str, v, true);
	va_end(v);
}

