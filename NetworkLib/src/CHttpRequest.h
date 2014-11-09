#pragma once

#include "CUrl.h"
#include "CInternetSession.h"
#include <vector>
#include <map>
#include <string>

class CHttpRequest{
public:
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                           構築                              //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	CHttpRequest(
		const CInternetSession&	cSession,	//セッションハンドルが必要
		const CUrl&				cUrl,		//パスが必要
		const wchar_t*			szVerb = L"GET"
	);
	~CHttpRequest();
	void AddHeader(
		const wchar_t* szHeader	// 例: "Content-Type: application/x-www-form-urlencoded\r\n"
	);

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                      リクエスト送信                         //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//send
	void SendRequest(const void* pBody, int nBodyLength);
	void SendRequest(const std::map<std::wstring, std::wstring>& mapPost);

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                      レスポンス受信                         //
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
