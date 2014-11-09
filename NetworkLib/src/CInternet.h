#pragma once

#include <wininet.h> //HINTERNET

class CInternet{
public:
	CInternet(const wchar_t* szAgent = L"MofmofBrowser");
	~CInternet();
	HINTERNET GetInternetHandle() const
	{
		return m_hInternet;
	}
private:
	HINTERNET m_hInternet;
};
