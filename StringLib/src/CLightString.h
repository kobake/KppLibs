/*
	Œy‚¢•¶š—ñƒNƒ‰ƒX

	E”äŠr‚ªŒy‚¢
	EƒRƒs[‚ªŒy‚¢
*/
#pragma once

#include <string>

struct Imp{
//	std::wstring	str;
	wchar_t*		str;
//	wchar_t			str[256];
	int				cnt;

	Imp(const wchar_t* rhs = L"", int _cnt = 0)
	{
		if(rhs == L""){
			str = L"";
//			str[0] = 0;
		}
		else{
			str = wcsdup(rhs);
//			wcscpy(str, rhs);
		}
		cnt = _cnt;
	}
	~Imp()
	{
		if(str == L""){
		}
		else{
			free(str);
		}
	}
	void AddRef()
	{
		++cnt;
	}
	void Release()
	{
		if(--cnt <= 0){
			delete this;
		}
	}
};
extern Imp nullimp;

#if 1
typedef std::wstring CLightString;
#else
class CLightString{
public:
	CLightString()
	{
		m_imp = &nullimp;
		m_imp->AddRef();
	}
	CLightString(const CLightString& rhs)
	{
		m_imp = rhs.m_imp;
		m_imp->AddRef();
	}
	CLightString(const std::wstring& rhs)
	{
		m_imp = new Imp(rhs.c_str(), 1);
	}
	CLightString(const wchar_t* rhs)
	{
		m_imp = new Imp(rhs, 1);
	}
	~CLightString()
	{
		m_imp->Release();
	}
	CLightString& operator = (const CLightString& rhs)
	{
		if(m_imp == rhs.m_imp)return *this;	// ‰½‚à‚µ‚È‚¢ //
		m_imp->Release();
		m_imp = rhs.m_imp;
		m_imp->AddRef();
		return *this;
	}
	CLightString& operator = (const wchar_t* rhs)
	{
		m_imp->Release();
		m_imp = new Imp(rhs, 1);
		return *this;
	}
	int		compare(const CLightString& rhs) const
	{
		if(m_imp == rhs.m_imp)return 0;
		return wcscmp(m_imp->str, rhs.m_imp->str);
	}
	int		compare(const wchar_t* rhs) const
	{
		return wcscmp(m_imp->str, rhs);
	}
	const wchar_t*	c_str() const
	{
		return m_imp->str;
	}
	CLightString	operator + (const CLightString& rhs) const
	{
		std::wstring ret = m_imp->str;
		ret += rhs.m_imp->str;
		return CLightString(ret.c_str());
	}
	CLightString	operator + (const std::wstring& rhs) const
	{
		std::wstring ret = m_imp->str;
		ret += rhs;
		return CLightString(ret.c_str());
	}
	CLightString	operator + (const wchar_t* rhs) const
	{
		std::wstring ret = m_imp->str;
		ret += rhs;
		return CLightString(ret.c_str());
	}
	size_t			length() const
	{
		return wcslen(m_imp->str);
	}
private:
	Imp*	m_imp;
};
#endif
