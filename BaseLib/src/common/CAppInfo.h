#pragma once

#include "../design/TSingleton.h"
#include <string>

class CModuleInfo{
public:
	CModuleInfo(HMODULE hModule = NULL);
	const wchar_t*	GetExeDir() const	{ return m_strExeDir.c_str(); }
	const wchar_t*	GetExePath() const	{ return m_strExePath.c_str(); }
	const wchar_t*	GetExeName() const	{ return m_strExeName.c_str(); }
	const wchar_t*	GetIniPath() const	{ return m_strIniPath.c_str(); }

	std::wstring	GetExeNameWithoutExt() const
	{
		const wchar_t* p = wcsrchr(m_strExeName.c_str(), L'.');
		if(p){
			return std::wstring(m_strExeName.c_str(), p);
		}
		else{
			return m_strExeName;
		}
	}
private:
	std::wstring	m_strExeDir;
	std::wstring	m_strExePath;
	std::wstring	m_strExeName;
	std::wstring	m_strIniPath;
};

class CAppInfo : public CModuleInfo, public TSingleton<CAppInfo>{
public:
	CAppInfo() : CModuleInfo(NULL) {}
};

