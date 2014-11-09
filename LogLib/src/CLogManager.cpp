#include "CLog.h"
#include <stdio.h>
#include <map>
#include "common/CAppInfo.h"
#include <StringLib.h>
#include "CLogManager.h"
#include "CLogType.h"
#include <c2lib.h>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// CLogManager
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CLogType _TRACE;
CLogType _INFO;
CLogType _WARN;
CLogType _ERROR;
CLogType _FATAL;
CLogType _QUERY;

void CLogManager::Init()
{
	Init(*CAppInfo::Instance());
}

void CLogManager::Init(
	const CModuleInfo&	cModuleInfo,
	const wchar_t*		szLogPath,
	bool				bLogHold		// ログをオープンしっぱなしにするか //
)
{
#ifdef _DEBUG
#else
	return;	// RELEASEビルドでは何もしない //
#endif
	m_bInitialized = true;

	// ログ出力パス
	std::wstring strPath = szLogPath;
	strPath = cpp_wcsreplace(strPath, L"$exedir", cModuleInfo.GetExeDir());
	strPath = cpp_wcsreplace(strPath, L"$exename", cModuleInfo.GetExeNameWithoutExt().c_str());

	// 各タイプの設定
	_TRACE.SetName("TRACE");
	_TRACE.SetFile(strPath.c_str(), bLogHold);
	_TRACE.SetTarget(LOG_TARGET_STDOUT);

	_INFO.SetName("INFO");
	_INFO.SetFile(strPath.c_str(), bLogHold);
	_INFO.SetTarget(LOG_TARGET_STDOUT);

	_WARN.SetName("WARN");
	_WARN.SetFile(strPath.c_str(), bLogHold);
	_WARN.SetTarget(LOG_TARGET_STDOUT);

	_ERROR.SetName("ERROR");
	_ERROR.SetFile(strPath.c_str(), bLogHold);
	_ERROR.SetTarget(LOG_TARGET_STDOUT);

	_FATAL.SetName("FATAL");
	_FATAL.SetFile(strPath.c_str(), bLogHold);
	_FATAL.SetTarget(LOG_TARGET_STDOUT);

	_QUERY.SetName("QUERY");
	_QUERY.SetFile(strPath.c_str(), bLogHold);
	_QUERY.SetTarget(LOG_TARGET_STDOUT);
}

void CLogManager::InitNoLog()
{
	m_bInitialized = true;
	_TRACE.SetTarget(LOG_TARGET_NONE);
	_INFO.SetTarget(LOG_TARGET_NONE);
	_WARN.SetTarget(LOG_TARGET_NONE);
	_ERROR.SetTarget(LOG_TARGET_NONE);
	_FATAL.SetTarget(LOG_TARGET_NONE);
	_QUERY.SetTarget(LOG_TARGET_NONE);
}
