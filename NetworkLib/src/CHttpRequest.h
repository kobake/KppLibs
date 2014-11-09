#pragma once

#include "CUrl.h"
#include "CInternetSession.h"
#include <vector>
#include <map>
#include <string>

class CHttpRequest{
public:
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                           �\�z                              //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	CHttpRequest(
		const CInternetSession&	cSession,	//�Z�b�V�����n���h�����K�v
		const CUrl&				cUrl,		//�p�X���K�v
		const wchar_t*			szVerb = L"GET"
	);
	~CHttpRequest();
	void AddHeader(
		const wchar_t* szHeader	// ��: "Content-Type: application/x-www-form-urlencoded\r\n"
	);

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                      ���N�G�X�g���M                         //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//send
	void SendRequest(const void* pBody, int nBodyLength);
	void SendRequest(const std::map<std::wstring, std::wstring>& mapPost);

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                      ���X�|���X��M                         //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//recv
	bool ReadResponse(
		void*	pBuf,		//[out]
		DWORD*	pdwBufSize	//[in/out]
	);
	std::string ReadAllA();
	std::wstring ReadAllW();
private:
	HINTERNET m_hHttpRequest;
};
