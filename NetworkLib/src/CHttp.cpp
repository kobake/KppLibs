#include "_required.h"
#include "CHttp.h"
#include <vector>
#include <string>
using namespace std;


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CHttp::CHttp(const wchar_t* url)
: m_hInet(NULL)
, m_hUrl(NULL)
{
	try{
		//インターネット開始
		m_hInet=InternetOpen(
			L"MofmofBrowser",
			INTERNET_OPEN_TYPE_PRECONFIG,
			NULL,
			NULL,
			0
		);
		if(m_hInet==NULL){
			throw wexception(L"InternetOpen failed");
		}

		//URLが指定されていれば、GETする
		if(url){
			this->RequestGet(url, TParams());
		}
	}
	catch(...){
		_final();
		throw;
	}
}

CHttp::~CHttp()
{
	_final();
}

void CHttp::_final()
{
	if(m_hUrl){ InternetCloseHandle(m_hUrl); m_hUrl=NULL; }
	if(m_hInet){ InternetCloseHandle(m_hInet); m_hInet=NULL; }
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        リクエスト                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CHttp::RequestGet(const wchar_t* szUrl, const TParams& tParams)
{
	if(m_hUrl){ InternetCloseHandle(m_hUrl); m_hUrl = NULL; }

	//HTTPセッションの開始、指定のURLオープン
	DWORD dwFlags = 0
		| INTERNET_FLAG_NO_AUTO_REDIRECT	// 自動リダイレクト //
		| INTERNET_FLAG_RELOAD;				// ローカルキャッシュ無視 //
	m_hUrl=InternetOpenUrl(
		m_hInet,
		szUrl,
		NULL,			//[in] リクエストヘッダ
		0,				//[in] リクエストヘッダ長
		dwFlags,		//[in] 各種フラグ
		0				//[in] アプリ定義の値 (コールバック用)
	);

	if(m_hUrl==NULL){
		InternetCloseHandle(m_hInet);
		throw wexception(L"InternetOpenUrl failed");
	}
}

//! POSTリクエスト
/*
void CHttp::RequestPost(const wchar_t* szUrl, const TParams& tParams)
{
	if(m_hUrl){ InternetCloseHandle(m_hUrl); m_hUrl = NULL; }

	CHttpRequest cReuqest(
	::InternetConnect(m_hInet, L"
	::HttpOpenRequest(
		m_hInet,
		L"POST",
		L"/page/a.html",
		NULL,	//version
		NULL,	//referer
		NULL,	//accept_types
		INTERNET_FLAG_RELOAD,	//flag
		0						//dwContext
	);

	//HTTPセッションの開始、指定のURLにPOST
	::HttpSendRequest(
		m_hInet,	//HINTERNET
		strHeaders,	//ヘッダ
		nHeaders,	//ヘッダ長
		option,
		optionlen
	);


	m_hUrl=InternetOpenUrl(
		m_hInet,
		url,
		NULL,								//[in] リクエストヘッダ
		0,									//[in] リクエストヘッダ長
		INTERNET_FLAG_NO_AUTO_REDIRECT,		//[in] 各種フラグ
		0									//[in] アプリ定義の値 (コールバック用)
	);

	if(m_hUrl==NULL){
		InternetCloseHandle(m_hInet);
		throw wexception(L"InternetOpenUrl failed");
	}
}
*/

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     インターフェース                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

std::wstring CHttp::GetHeader() const
{
	DWORD dwSize=255;
	vector<wchar_t> buf(dwSize+1);
	BOOL b;

retry:
	b=HttpQueryInfoW(
		m_hUrl,
		HTTP_QUERY_RAW_HEADERS_CRLF,
		&buf[0],
		&dwSize,
		NULL
	);

	if(!b){
		switch(GetLastError()){
		case ERROR_HTTP_HEADER_NOT_FOUND:
			//ヘッダが無いので空文字列を返す
			return L"";
		case ERROR_INSUFFICIENT_BUFFER:
			//メモリを再確保してリトライ
			buf.resize(dwSize+1);
			goto retry;
		default:
			//それ以外のエラーでは例外を投げる
			throw wexception(L"HttpQueryInfo failed");
		}
	}

	buf[dwSize]=L'\0';
	return &buf[0];
}

//! 応答コード
EHttpStatusCode CHttp::GetStatusCode() const
{
	wchar_t buf[256];
	DWORD dwSize = sizeof(buf)-1;
	BOOL b =HttpQueryInfoW(
		m_hUrl,
		HTTP_QUERY_STATUS_CODE,
		buf,
		&dwSize,
		NULL
	);
	if(b){
		buf[dwSize] = L'\0';
		return (EHttpStatusCode)_wtoi(buf);
	}
	else{
		throw exception("HttpQueryInfo(HTTP_QUERY_STATUS_CODE) failed.");
	}
}

#include <StringLib.h>

std::string CHttp::ReadAllA() const
{
	//読み出すものがなくなるまで読み出す
	vector<char> ret(0);
	while(1){
		DWORD dwRead;
		vector<char> tmp(256);
		InternetReadFile(
			m_hUrl,
			&tmp[0],
			(DWORD)tmp.size(),
			&dwRead
		);

		//読み出すものがなくなったらループ脱出
		if(dwRead==0)break;

		//retに結合
		ret.insert(ret.end(),tmp.begin(),tmp.begin()+dwRead);
	}
	ret.push_back('\0');

	return &ret[0];
}

std::wstring CHttp::ReadAll(bool bUtf8) const
{
	//読み出すものがなくなるまで読み出す
	vector<char> ret(0);
	while(1){
		DWORD dwRead;
		vector<char> tmp(256);
		InternetReadFile(
			m_hUrl,
			&tmp[0],
			(DWORD)tmp.size(),
			&dwRead
		);

		//読み出すものがなくなったらループ脱出
		if(dwRead==0)break;

		//retに結合
		ret.insert(ret.end(),tmp.begin(),tmp.begin()+dwRead);
	}
	ret.push_back('\0');

	//文字コード変換 UTF-8→UNICODE (####仮)
	if(bUtf8 || strstr(&ret[0], "charset=utf-8")){
		const char* p = &ret[0];
		return cpp_mbstowcs(p, CP_UTF8);
		/*
		int new_size = ::MultiByteToWideChar(CP_UTF8, 0, p, strlen(p), NULL, 0);
		vector<wchar_t>
		int new_size = UTF8NtoSJIS(p,NULL,0);
		vector<char> ret2(new_size+1);
		new_size = UTF8NtoSJIS(p, &ret2[0], new_size);
		return cpp_mbstowcs(&ret2[0]);
		*/
	}

	//文字列に変換
	return cpp_mbstowcs(&ret[0]);
}

#include "CDataSocket.h"

//GETリクエストの返答を取得する
std::string CHttp::RequestGet(const wchar_t* server,const wchar_t* url) const
{
	//ソケットを開く
	CSocket _s;
	CDataSocket s(&_s);

	//サーバに接続
	CIpAddress ip(server); //IP
	CSocketAddress addr(ip, 80); //IP+ポート
	s.Connect(addr);

	//GETリクエスト
	int ret=s.sendf("GET %ls\n",url);

	//受信
	string rc=s.recv_string();

	return rc;
}

