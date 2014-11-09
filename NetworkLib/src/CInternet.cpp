#include "common/_required.h"
#include "CInternet.h"
#include <exception>

CInternet::CInternet(const wchar_t* szAgent)
{
	// WININET‰Šú‰»
	m_hInternet = InternetOpen(
		szAgent,
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0
	);
	if(!m_hInternet)throw std::exception("InternetOpen failed.");
}

CInternet::~CInternet()
{
	InternetCloseHandle(m_hInternet);
}
