#include "mystring.h"
#include <Windows.h>
#include "Regex.h"


std::wstring sjis2unicode(const std::string& str)
{
	int n = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), NULL, 0);
	std::vector<wchar_t> vbuf(n + 1);
	wchar_t* buf = &vbuf[0];
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), buf, n);
	buf[n] = 0;
	return buf;
}

std::string unicode2utf8(const std::wstring& wstr)
{
	std::vector<char> buf(wstr.length() * 3 + 2);
	int n = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), &buf[0], (int)buf.size(), NULL, NULL); 
	buf[n] = 0;
	return &buf[0];
}

std::wstring atow(const std::string& str)
{
	std::vector<wchar_t> buf(str.length() + 1);
	int n = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.length(), &buf[0], (int)buf.size());
	buf[n] = 0;
	return &buf[0];
}
std::string wtoa(const std::wstring& str)
{
	std::vector<char> buf(str.length() * 3 + 2);
	int n = ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), (int)str.length(), &buf[0], (int)buf.size(), NULL, NULL);
	buf[n] = 0;
	return &buf[0];
}

myvector<mystring> mystring::match(const mystring& pattern) const
{
	Regex reg(pattern);
	return reg.Match(*this);
}

bool mystring::startsWith(const mystring& str) const
{
	const wchar_t* p = this->c_str();
	return wcsstr(p, str.c_str()) == p;
}

// 末尾改行削除
std::string Chomp(const std::string& str)
{
	const char* s = str.c_str();
	const char* p = strchr(s, '\0');
	if(p - 1 >= s && (*(p - 1) == '\r' || *(p - 1) == '\n')){
		p--;
	}
	return std::string(s, p);
}


// 文字列分割 //
std::vector<std::string> split(std::string str, char sep)
{
	std::vector<std::string> ret;
	const char* pStr = str.c_str();
	const char* p = pStr;
	while(*p){
		const char* q = strchr(p, sep);
		if(q){
			std::string s(p, q);
			ret.push_back(s);
			p = q + 1;
		}
		else{
			std::string s(p);
			ret.push_back(s);
			break;
		}
	}
	return ret;
}

// 文字列置換 //
std::string replace(const std::string& src,const char* from,const char* to)
{
	const char* p=src.c_str();
	int src_len=(int)src.length();
	int from_len=(int)strlen(from);
	int to_len=(int)strlen(to);
	std::string ret;
	while(1){
		const char* q=strstr(p,from);
		if(q){
			//p～qをretに追加
			ret.append(p,q);
			//toをretに追加
			ret.append(to,to+to_len);
			//pを進める
			p=q+from_len;
		}
		else{
			//p～\0をretに追加
			ret.append(p);
			break;
		}
	}
	return ret;
}
// 文字列置換 //
std::wstring wreplace(const std::wstring& src, const wchar_t* from, const wchar_t* to)
{
	const wchar_t* p = src.c_str();
	int src_len  = (int)src.length();
	int from_len = (int)wcslen(from);
	int to_len   = (int)wcslen(to);
	std::wstring ret;
	while(1){
		const wchar_t* q=wcsstr(p,from);
		if(q){
			//p～qをretに追加
			ret.append(p,q);
			//toをretに追加
			ret.append(to,to+to_len);
			//pを進める
			p = q+from_len;
		}
		else{
			//p～\0をretに追加
			ret.append(p);
			break;
		}
	}
	return ret;
}

int mystring::toInt() const
{
	return _wtoi(this->c_str());
}


// int→文字列 //
std::string cpp_itoa(int i)
{
	char buf[32];
	sprintf(buf, "%d", i);
	return buf;
}
std::wstring cpp_itow(int i)
{
	wchar_t buf[32];
	swprintf(buf, L"%d", i);
	return buf;
}

// トリミング //
std::string trim(std::string str, char c)
{
	const char* p = str.c_str();
	while(*p == c){
		p++;
	}
	const char* q = strchr(p, '\0');
	while(q - 1 >= p && *(q - 1) == c){
		q--;
	}
	return std::string(p, q);
}

// 見つからなかったらNULLを返す
wchar_t* wcschrs(const wchar_t* str,const wchar_t* chrs)
{
	const wchar_t* p=str;
	const wchar_t* q;
	while(*p){
		q=chrs;
		while(*q){
			if(*p==*q)return (wchar_t*)p;
			q++;
		}
		p++;
	}
	return NULL;
}

myvector<mystring> mystring::split(const mystring& _seps) const
{
	const wchar_t* str = this->c_str();
	const wchar_t* seps = _seps.c_str();
	const wchar_t* p = str;
	const wchar_t* q;
	int wcs_len = (int)wcslen(str);
	myvector<mystring> ret;
	while(*p){
		q = wcschrs(p, seps);
		if(q == 0)q = wcschr(p,L'\0');
		ret.push_back(mystring(p, q));
		if(*q == L'\0')break;
		p = q + 1;
	}
	return ret;
}

mystring mystring::replace(const mystring& before, const mystring& after) const
{
	return wreplace(*this, before.c_str(), after.c_str());
}

// 正規表現による置換
mystring mystring::replace_reg(const mystring& pattern, const mystring& after) const
{
	Regex reg(pattern);
	return reg.Replace(*this, after);
}

mystring mystring::trim(const mystring& _chars) const
{
	const wchar_t* chars = _chars.c_str();
	const wchar_t* p = this->c_str();
	// 左側削り
	while(wcschr(chars, *p)){
		p++;
	}
	// 右側削り
	const wchar_t* q = wcschr(p, L'\0');
	while(q - 1 >= p && wcschr(chars, *(q - 1))){
		q--;
	}
	// 結果
	return mystring(p, q);
}

mystring mystring::toLower() const
{
	mystring buf;
	wchar_t* p = (wchar_t*)buf.c_str();
	while(*p){
		*p = tolower(*p);
		p++;
	}
	return buf;
}
