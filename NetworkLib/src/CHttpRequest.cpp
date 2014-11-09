#include "common/_required.h"
#include "CHttpRequest.h"
#include <exception>
#include <StringLib.h>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �\�z                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CHttpRequest::CHttpRequest(
	const CInternetSession&	cSession,	//�Z�b�V�����n���h�����K�v
	const CUrl&				cUrl,		//�p�X���K�v
	const wchar_t*			szVerb		//GET or POST
)
{
	DWORD dwRequestFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_UI | INTERNET_FLAG_KEEP_CONNECTION;
	if(cUrl.GetScheme() == INTERNET_SCHEME_HTTPS){
		dwRequestFlags |= INTERNET_FLAG_SECURE;
		dwRequestFlags |= INTERNET_FLAG_IGNORE_CERT_CN_INVALID;		// �ؖ����̃z�X�g���𖳎����� //
		dwRequestFlags |= INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;	// �ؖ����̓��t�𖳎����� //
	}
	const wchar_t* pszAccept = L"Accept: */*";
	m_hHttpRequest = HttpOpenRequest(
		cSession.GetSessionHandle(),
		szVerb,
		cUrl.GetPath(),
		NULL,				//L"HTTP/1.0" (HTTP_VERSION)
		NULL,				// referrer
		NULL,				// Extra headers &pszAccept,
		dwRequestFlags,		// Flags
		0					// Context
	);
	if(!m_hHttpRequest)throw std::exception("HttpOpenRequest failed.");
}

CHttpRequest::~CHttpRequest()
{
	InternetCloseHandle(m_hHttpRequest);
}

void CHttpRequest::AddHeader(
	const wchar_t* szHeader	// ��: "Content-Type: application/x-www-form-urlencoded\r\n"
)
{
	BOOL bRet = ::HttpAddRequestHeaders(
		m_hHttpRequest,
		szHeader,
		-1,
		HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD
	);
	if(!bRet)throw std::exception("HttpAddRequestHeaders failed.");
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      ���N�G�X�g���M                         //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//send
void CHttpRequest::SendRequest(const void* pBody, int nBodyLength)
{
	const wchar_t* pszContentType = L"Content-Type: application/x-www-form-urlencoded";	// ������厖�݂��� //
	/*
		�s���ȏؖ��@�ւ𖳎�
		http://support.microsoft.com/kb/182888/ja
	*/
	DWORD dwFlags = 0;
	DWORD dwLen = sizeof(dwFlags);
	InternetQueryOption(m_hHttpRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, &dwLen);
	dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
	InternetSetOption(m_hHttpRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

	/*
		���M
	*/
	BOOL bRequestResult = HttpSendRequest(
		m_hHttpRequest,
		pszContentType,
		(DWORD)(pszContentType?wcslen(pszContentType):0),
		const_cast<void*>(pBody),							//�p�����[�^ //#########�L��
		(DWORD)nBodyLength									//�p�����[�^��
	);
	if(!bRequestResult)throw std::exception("HttpSendRequest failed.");
}
void CHttpRequest::SendRequest(const std::map<std::wstring, std::wstring>& mapPost)
{
	// URL�G���R�[�h�ςݕ�����\�z //
	std::string strBody;
	for(std::map<std::wstring, std::wstring>::const_iterator itr = mapPost.begin(); itr != mapPost.end(); itr++){
		if(strBody.length())strBody += "&";
		std::string strKey = urlencode(cpp_wcstombs(itr->first, CP_UTF8));
		std::string strValue = urlencode(cpp_wcstombs(itr->second, CP_UTF8));
		strBody += strKey + "=" + strValue;
	}

	// ���M //
	SendRequest(strBody.c_str(), (int)strBody.length());
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      ���X�|���X��M                         //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//recv
bool CHttpRequest::ReadResponse(
	void*	pBuf,		//[out]
	DWORD*	pdwBufSize	//[in/out]
)
{
	DWORD dwRead = *pdwBufSize;
	BOOL bReadResult = InternetReadFile(
		m_hHttpRequest,
		pBuf,
		*pdwBufSize,
		pdwBufSize
	);
	return bReadResult!=FALSE;
}

std::string CHttpRequest::ReadAllA()
{
	vector<char> vBuf;
	char buf[10240];
	while(1){
		DWORD dwRead = sizeof(buf);
		if(!ReadResponse(buf,&dwRead))break;
		if(dwRead==0)break;
		vBuf.insert(vBuf.end(), &buf[0], &buf[dwRead]);
	}
	vBuf.push_back(0);
	return &vBuf[0];
}

std::wstring CHttpRequest::ReadAllW()
{
	std::string str = ReadAllA();
	return cpp_mbstowcs(str, CP_UTF8);
}
