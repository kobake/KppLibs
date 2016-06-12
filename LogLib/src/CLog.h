#pragma once

#include <BaseLib.h> // TSingleton
#include <string>
#include <stdarg.h>
#include "CLogStream.h"
class CLogType;

/*
	ログ機能呼び出され役。

	呼び出し元のファイル名や行番号、関数名などをログ情報に付与する。
*/
class CLog{
public:
	CLog(const char* szFileName, int nLineNo, const char* szFunctionName)
	: m_szFileName(szFileName)
	, m_nLineNo(nLineNo)
	, m_szFunctionName(szFunctionName)
	{
	}
	void PrintV(CLogType& cType, const char* str, va_list v, bool bMsg);
	void Print(CLogType& cType, const char* str, ...);
	void PrintAndMessage(CLogType& cType, const char* str, ...);
private:
	const char*	m_szFileName;
	int			m_nLineNo;
	const char*	m_szFunctionName;
};

#ifdef _DEBUG
	#define KLOG CLog(__FILE__, __LINE__, __FUNCTION__).Print
	#define MSGLOG CLog(__FILE__, __LINE__, __FUNCTION__).PrintAndMessage
#else
	#if _MSC_VER >= 1400
		#define KLOG(...)
		#define MSGLOG(...)
	#else
		inline void _dummy_log(CLogType& type, const char* str, ...){}
		#define KLOG _dummy_log
		#define MSGLOG _dummy_log
	#endif
#endif

