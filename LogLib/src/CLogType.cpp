#include "CLog.h"
#include <stdio.h>
#include <map>
#include "common/CAppInfo.h"
#include <StringLib.h>
#include "CLogType.h"

static const int DEFAULT_ROTATION_SIZE = 512 * 1024;	// 512KB //

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// CLogType
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

static std::map<std::wstring, CLogStream*> g_mapStreams;

CLogType::CLogType()
: m_strName("UnknownType")
, m_eTarget(LOG_TARGET_STDOUT)
, m_pcStreamRef(NULL)
, m_nRotationSize(DEFAULT_ROTATION_SIZE)
{
}

CLogType::~CLogType()
{
	if(m_pcStreamRef){
		m_pcStreamRef->Release();
	}
}

void CLogType::SetFile(const wchar_t* szFileNameTemplate, bool bLogHold)
{
	// 以前に設定していたものは解放
	if(m_pcStreamRef){
		m_pcStreamRef->Release();
		m_pcStreamRef = NULL;
	}

	// ファイル名指定が無効の場合は無視。
	if(!szFileNameTemplate || !szFileNameTemplate[0]){
		return;
	}

	// 新しく設定
	m_pcStreamRef = g_mapStreams[szFileNameTemplate]; // ### szFileNameTemplate が同じでも bLogHold が違うものが混在するかもしれない //
	if(!m_pcStreamRef){
		m_pcStreamRef = new CLogStream(szFileNameTemplate, m_nRotationSize, bLogHold);
		g_mapStreams[szFileNameTemplate] = m_pcStreamRef;
	}
	m_pcStreamRef->AddRef();
}

void CLogType::SetRotationSize(int nSize)
{
	m_nRotationSize = nSize;
	if(m_pcStreamRef){
		m_pcStreamRef->SetRotationSize(nSize);
	}
}

void CLogType::Print(const char* str)
{
	if(m_eTarget & LOG_TARGET_STDOUT){
		fprintf(stdout, "%hs\n", str);
	}
	if(m_eTarget & LOG_TARGET_STDERR){
		fprintf(stderr, "%hs\n", str);
	}
	if((m_eTarget & LOG_TARGET_FILE) && m_pcStreamRef){
		m_pcStreamRef->PrintLn(str);
	}
	if(m_eTarget & LOG_TARGET_DEBUGGER){
		::OutputDebugStringA(str);
		::OutputDebugStringA("\r\n");
	}
}
