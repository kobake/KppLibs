#pragma once

#include "common/_required.h"

/*
参考:
C++サンプル
http://winofsql.jp/VA003334/smalltech060519171448.htm
Provider一覧
http://homepage1.nifty.com/kojama/works/rdbms/conn/connstr.html#mssqloledb
80004005エラーについて
http://tryasp.winscom.co.jp/note/80004005.htm
*/

#import "c:\Program Files\Common Files\System\ADO\msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")

void OnComError(HRESULT hr);

inline void TESTHR(HRESULT hr)
{
	if(FAILED(hr)){
		OnComError(hr);
	}
}

void TESTHR_F(HRESULT hr);

#include <exception>
#include <string>
class CComError : public std::exception{
public:
	CComError(const _com_error& e, const wchar_t* szAdditionalInfo = L"");
	const char* what() const;
private:
	std::string m_strMsg;
};
