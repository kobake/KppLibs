#pragma once

#include <wininet.h> //URL_COMPONENTS
#include <string>

template <class T> struct _URL_COMPONENTS;

template<> class _URL_COMPONENTS<wchar_t> : public URL_COMPONENTSW{ };
template<> class _URL_COMPONENTS<char> : public URL_COMPONENTSA{ };

template <class T>
class _CUrl{
public:
	_CUrl(const T* szUrl = NULL);
	// ���l
	const T* GetUrl() const
	{
		return m_strUrl.c_str();
	}
	// http/https/ftp ��
	INTERNET_SCHEME GetScheme() const
	{
		return m_sUrl.nScheme;
	}
	// www.google.co.jp ��
	const T* GetHost() const
	{
		return m_szHost;
	}
	// /abc/d.htm?a=b&c=d ��
	const T* GetPath() const
	{
		if(m_strPathWithExtra.empty()){
			m_strPathWithExtra = m_szPath;
			m_strPathWithExtra += m_szExtraInfo;
		}
		return m_strPathWithExtra.c_str();
	}
	// �C�Ӑ��l (HTTP�ł�80�Ƃ͌���Ȃ� �� http://www.google.co.jp:1234/ ���̎w��������ꍇ)
	int GetPort() const
	{
		return m_sUrl.nPort;
	}
private:
	std::basic_string<T>			m_strUrl;
	_URL_COMPONENTS<T>				m_sUrl;
	T								m_szHost[256];			// "www.yahoo.co.jp"
	T								m_szScheme[256];		// "http"
	T								m_szUser[256];
	T								m_szPass[256];
	T								m_szPath[1024];			// "/contents/a.htm"
	T								m_szExtraInfo[1024];	// "?x=1&y=2"
	//�L���b�V��
	mutable std::basic_string<T>	m_strPathWithExtra;		// "/contents/a.htm?x=1&y=2"
};

template <class T>
std::basic_string<T> urlencode(const T* str);

typedef _CUrl<wchar_t> CUrl;
typedef _CUrl<wchar_t> CUrlW;
typedef _CUrl<char> CUrlA;

//CUrlA��CUrlW�̑����ϊ�
CUrlW a2w(const CUrlA& cUrlA);
CUrlA w2a(const CUrlW& cUrlW);
