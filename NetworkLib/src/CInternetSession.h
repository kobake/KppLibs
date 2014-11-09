#pragma once

#include "CInternet.h"

class CInternetSession{
public:
	CInternetSession(const CInternet& cInternet, const wchar_t* szServer, int nPort);
	~CInternetSession();
	HINTERNET GetSessionHandle() const
	{
		return m_hSession;
	}
private:
	HINTERNET m_hSession;
};
