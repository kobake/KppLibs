#pragma once

// ゆとり文字列クラス

#include <string>
#include <vector>
#include "myvector.h"
#include "mystring.h"

class mystring : public std::wstring{
public:
	mystring()                                         : std::wstring() {}
	mystring(const std::wstring& str)                  : std::wstring(str) {}
	mystring(const wchar_t* str)                       : std::wstring(str) {}
	mystring(const wchar_t* begin, const wchar_t* end) : std::wstring(begin, end) {}
	// 操作 //
	myvector<mystring>		split(const mystring& chars) const;
	mystring				replace(const mystring& before, const mystring& after) const;
	mystring				replace_reg(const mystring& pattern, const mystring& after) const; // 正規表現による置換
	mystring				trim(const mystring& chars = L" \t\r\n") const;
	myvector<mystring>		match(const mystring& pattern) const;
	bool					startsWith(const mystring& str) const;
	// 変換 //
	int						toInt() const;
	mystring				toLower() const;
};


// 文字コード変換 //
std::wstring sjis2unicode(const std::string& str);
std::string unicode2utf8(const std::wstring& wstr);
// 文字コード変換 //
std::wstring atow(const std::string& str);
std::string wtoa(const std::wstring& str);

// 末尾改行削除 //
std::string Chomp(const std::string& str);

// 文字列分割 //
std::vector<std::string> split(std::string str, char sep);

// 文字列置換 //
std::string replace(const std::string& src,const char* from,const char* to);
std::wstring wreplace(const std::wstring& src, const wchar_t* from, const wchar_t* to);

// int→文字列 //
std::string cpp_itoa(int i);
std::wstring cpp_itow(int i);

// トリミング //
std::string trim(std::string str, char c);
