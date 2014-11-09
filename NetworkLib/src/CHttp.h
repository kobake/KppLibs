#pragma once

#include <Wininet.h>
#include <string>

#include "EHttpStatusCode.h"
#include <map>
#include <string>

class CHttp{
protected:
	void _final();
public:
	//型
	typedef std::map<std::wstring, std::wstring> TParams;
public:
	//コンストラクタ・デストラクタ
	CHttp(const wchar_t* url = NULL);
	~CHttp();
	
	//リクエスト
	void RequestGet(const wchar_t* szUrl, const TParams& tParams);
	void RequestPost(const wchar_t* szUrl, const TParams& tParams);

	//取得インターフェース
	std::wstring GetHeader() const; //生ヘッダ
	EHttpStatusCode GetStatusCode() const; //応答コード
	std::string ReadAllA() const;
	std::wstring ReadAll(bool bUtf8 = false) const;
	std::string RequestGet(const wchar_t* server,const wchar_t* url) const; //!< GETリクエストの返答を取得する
private:
	HINTERNET m_hInet;
	HINTERNET m_hUrl;
};

#include "_lib_wininet.h"
