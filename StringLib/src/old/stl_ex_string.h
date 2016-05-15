#pragma once

class ex_string : public wstring{
public:
	void removeQuote();
	void removeLf();
	void removeCrlf();
	ex_string replace(const wstring& from,const wstring& to) const;
	wstring::iterator last(){ return end()-1; }
	wstring::iterator first(){ return begin(); }
	int toInt();
	ex_string(const wchar *p,size_type n) : wstring(p,n) { }
	ex_string(const wstring& s) : wstring(s) { }
	ex_string() : wstring() { }
	ex_string(const wchar *p) : wstring(p) { }
};

