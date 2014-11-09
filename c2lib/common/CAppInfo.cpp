#include "CAppInfo.h"
#include <IoLib.h> //CFilePath

CModuleInfo::CModuleInfo(HMODULE hModule)
{
	wchar_t buf[_MAX_PATH] = L"";
	// EXEパス //
	::GetModuleFileName(hModule, buf, _countof(buf));
	m_strExePath = buf;
	CFilePathWin cExePath = buf;
	// EXEディレクトリ //
	m_strExeDir = cExePath.GetDir();
	// EXE名 //
	m_strExeName = cExePath.GetTitle();
	// INIパス //
	m_strIniPath = m_strExeDir + L"\\" + cExePath.GetTitleWithoutExt() + L".ini";
}

