#include "CAppInfo.h"
#include <IoLib.h> //CFilePath

CModuleInfo::CModuleInfo(HMODULE hModule)
{
	wchar_t buf[_MAX_PATH] = L"";
	// EXE�p�X //
	::GetModuleFileName(hModule, buf, _countof(buf));
	m_strExePath = buf;
	CFilePathWin cExePath = buf;
	// EXE�f�B���N�g�� //
	m_strExeDir = cExePath.GetDir();
	// EXE�� //
	m_strExeName = cExePath.GetTitle();
	// INI�p�X //
	m_strIniPath = m_strExeDir + L"\\" + cExePath.GetTitleWithoutExt() + L".ini";
}

