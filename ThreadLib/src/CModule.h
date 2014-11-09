#pragma once

#include <windows.h> // HANDLE //

class CModule{
public:
	CModule(HANDLE hProcessRef, HMODULE hModuleRef);
	bool IsValid() const;
	void GetBaseName(wchar_t* pBuf, int nBufCnt) const;
	void GetFilePath(wchar_t* pBuf, int nBufCnt) const;
	void Print() const;
private:
	HANDLE	m_hProcessRef;
	HMODULE m_hModuleRef;
};
