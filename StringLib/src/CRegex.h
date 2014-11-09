#pragma once

#include <vector>
#include <string>

/*
	使用例

		std::vector<std::wstring> tmp = CRegex::GetMatchStrings(L"abc aef ghi", L"a(.).");
		↓
		tmp[0] = "abc"
		tmp[1] = "b"
*/
class CRegex{
public:
	//検索(存在有無のみ)
	static bool Exists(const wchar_t* szSource, const wchar_t* szPattern);
	static bool Exists(const char* szSource, const char* szPattern);
	//検索
	static std::vector<std::wstring> GetMatchStrings(const wchar_t* szSource, const wchar_t* szPattern);
	static std::vector<std::string> GetMatchStrings(const char* szSource, const char* szPattern);
	//置換
	static std::wstring Replace(const wchar_t* szSource, const wchar_t* szPattern, const wchar_t* szReplacing);
	static std::string Replace(const char* szSource, const char* szPattern, const char* szReplacing);
};

