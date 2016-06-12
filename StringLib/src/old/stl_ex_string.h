#pragma once

#include <string>

class ex_string : public std::wstring{
public:
	void removeQuote();
	void removeLf();
	void removeCrlf();
	ex_string replace(const std::wstring& from,const std::wstring& to) const;
	std::wstring::iterator last(){ return end()-1; }
	std::wstring::iterator first(){ return begin(); }
	int toInt();
	ex_string(const wchar_t *p,size_type n) : std::wstring(p,n) { }
	ex_string(const std::wstring& s) : std::wstring(s) { }
	ex_string() : std::wstring() { }
	ex_string(const wchar_t *p) : std::wstring(p) { }
};

