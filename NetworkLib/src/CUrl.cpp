#include "common/_required.h"
#include "CUrl.h"
#include <exception>
#include <vector>
#include <string>
#include <StringLib.h>
using namespace std;

inline BOOL InternetCrackUrl_Auto(
	LPCSTR lpszUrl,
	DWORD dwUrlLength,
	DWORD dwFlags,
	LPURL_COMPONENTSA lpUrlComponents
)
{
	return InternetCrackUrlA(lpszUrl, dwUrlLength, dwFlags, lpUrlComponents);
}
inline BOOL InternetCrackUrl_Auto(
	LPCWSTR lpszUrl,
	DWORD dwUrlLength,
	DWORD dwFlags,
	LPURL_COMPONENTSW lpUrlComponents
)
{
	return InternetCrackUrlW(lpszUrl, dwUrlLength, dwFlags, lpUrlComponents);
}

template <class T>
_CUrl<T>::_CUrl(const T* szUrl)
{
	T zero[] = {0};
	if(szUrl==NULL)szUrl = zero;

	m_strUrl = szUrl;
	m_sUrl.dwStructSize      = sizeof(m_sUrl);
	m_sUrl.lpszHostName      = m_szHost;
	m_sUrl.dwHostNameLength  = _countof(m_szHost);
	m_sUrl.lpszScheme        = m_szScheme;
	m_sUrl.dwSchemeLength    = _countof(m_szScheme);
	m_sUrl.lpszUserName      = m_szUser;
	m_sUrl.dwUserNameLength  = _countof(m_szUser);
	m_sUrl.lpszPassword      = m_szPass;
	m_sUrl.dwPasswordLength  = _countof(m_szPass);
	m_sUrl.lpszUrlPath       = m_szPath;
	m_sUrl.dwUrlPathLength   = _countof(m_szPath);
	m_sUrl.lpszExtraInfo     = m_szExtraInfo;
	m_sUrl.dwExtraInfoLength = _countof(m_szExtraInfo);
	if(*szUrl){
		BOOL ret = ::InternetCrackUrl_Auto(
			szUrl,
			(DWORD)auto_strlen(szUrl),
			0,//ICU_ESCAPE,
			&m_sUrl
		);
		if(!ret)throw std::exception("InternetCrackUrl failed.");
	}
	else{
		*m_szHost = 0;
		*m_szScheme = 0;
		*m_szUser = 0;
		*m_szPass = 0;
		*m_szPath = 0;
		*m_szExtraInfo = 0;
	}

	//オプション引数のURLエンコード
//	if(m_szExtraInfo[0]!=L'\0'){
//		wstring s = urlencode(&m_szExtraInfo[1]); //先頭の「?」はエンコードから除外
//		wcscpy(&m_szExtraInfo[1], s.c_str());
//	}
}

char char_hex(int n)
{
	if(n<10){ return '0' + n; }
	else{ return 'A' + (n-10); }
}

template <class T> const T* empty_string();
template <> const char* empty_string(){ return ""; }
template <> const wchar_t* empty_string(){ return L""; }

//ギコナビ参考
template <class T>
basic_string<T> urlencode(const T* str)
{
#define UNSAFE_CHAR L"{}|\\^~[]`%" 
#define RESERVED_CHAR L";/?:@=&"
//	static const char *const special_char = 
//		UNSAFE_CHAR RESERVED_CHAR "+";
	T ch;
	if( !str )
		return empty_string<T>();

	vector<T> ret;
	for( const T *p = str; *p ;p ++ )
	{
		ch = *p;
//		if(  0x7f > ch && ch > 0x1f &&
//			!strchr( special_char  , ch )
		if( isalnum( ch )  ){
			ret.push_back(ch);
		}
		else if( ch == 0x20 ){ //半角スペース
			ret.push_back(L'+');
		}
		else if(wcschr(L"_-.",ch) ){
			ret.push_back(ch);
		}
		else{
			T tmp[10];
			tmp[0] = '%';
			tmp[1] = char_hex((ch>>4) & 0xF);
			tmp[2] = char_hex((ch>>0) & 0xF);
			tmp[3] = 0;
			ret.insert(ret.end(), &tmp[0], &tmp[auto_strlen(tmp)]);
		}
	}
	ret.push_back(0);
	return &ret[0];
}

template class _CUrl<char>;
template class _CUrl<wchar_t>;

template basic_string<char> urlencode(const char* str);
template basic_string<wchar_t> urlencode(const wchar_t* str);


#include <StringLib.h>

//総合変換
CUrlW a2w(const CUrlA& cUrlA)
{
	return CUrlW(tmp_mbstowcs(cUrlA.GetUrl()));
}

CUrlA w2a(const CUrlW& cUrlW)
{
	return CUrlA(tmp_wcstombs(cUrlW.GetUrl()));
}
