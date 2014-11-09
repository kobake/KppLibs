#include "common/_required.h"
#include "CInternetSession.h"
#include <exception>

CInternetSession::CInternetSession(const CInternet& cInternet, const wchar_t* szServer, int nPort)
{
	// ÉTÅ[ÉoÇ÷ê⁄ë±
	m_hSession = InternetConnect(
		cInternet.GetInternetHandle(),
		szServer,
		nPort,
		NULL,	// user name
		NULL,	// user password
		INTERNET_SERVICE_HTTP,
		0,
		0
	);
	if(!m_hSession)throw std::exception("InternetConnect failed.");
}

CInternetSession::~CInternetSession()
{
	InternetCloseHandle(m_hSession);
}
