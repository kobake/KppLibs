#pragma once

#include <vector>
#include <string>

/*
	�g�p��

		std::vector<std::wstring> tmp = CRegex::GetMatchStrings(L"abc aef ghi", L"a(.).");
		��
		tmp[0] = "abc"
		tmp[1] = "b"
*/
class CRegex{
public:
	//����(���ݗL���̂�)
	static bool Exists(const wchar_t* szSource, const wchar_t* szPattern);
	static bool Exists(const char* szSource, const char* szPattern);
	//����
	static std::vector<std::wstring> GetMatchStrings(const wchar_t* szSource, const wchar_t* szPattern);
	static std::vector<std::string> GetMatchStrings(const char* szSource, const char* szPattern);
	//�u��
	static std::wstring Replace(const wchar_t* szSource, const wchar_t* szPattern, const wchar_t* szReplacing);
	static std::string Replace(const char* szSource, const char* szPattern, const char* szReplacing);
};

