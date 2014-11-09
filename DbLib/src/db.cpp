#include "_required.h"
#include "CDbAlert.h"
#include <string>
#include <exception>
using namespace std;

//########‚ ‚Æ‚ÅCOMê—pƒ‰ƒCƒuƒ‰ƒŠ‹óŠÔ‚ÉˆÚŠÇ—\’è
std::wstring GetResultMessage(HRESULT hr)
{
	LPWSTR p;

	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Šù’è‚ÌŒ¾Œê
		(LPWSTR)&p,
		0,
		NULL
	);

	wstring ret = p;
	LocalFree(p);

	return ret;
}

void GetResultMessage(HRESULT hr, char* buf, size_t nbuf)
{
	FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Šù’è‚ÌŒ¾Œê
		buf,
		(DWORD)nbuf,
		NULL
	);
}

void OnComError(HRESULT hr)
{
	static char buf[256];
	GetResultMessage(hr, buf, _countof(buf));
	throw std::exception(buf);
//	_com_issue_error(x);
}

void TESTHR_F(HRESULT hr)
{
	if(FAILED(hr)){
		OnComError(hr);
	}
}


#include "util/mywstring.h"

CComError::CComError(const _com_error& e, const wchar_t* szAdditionalInfo)
{
	m_strMsg = "<<CComError>>\n"
		+ CDbAlert().GetComError(e) + "\n";
	if(szAdditionalInfo && *szAdditionalInfo){
		m_strMsg +=
			"-- additional info --\n"
			+ cpp_wcstombs(szAdditionalInfo) + "\n";
	}
}

const char* CComError::what() const
{
	return m_strMsg.c_str();
}


