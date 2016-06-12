#include <BaseLib.h>
#include "CRegex.h"
#include <boost/regex.hpp>
#include <string>
#include <vector>
using namespace std;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                    検索(存在有無のみ)                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

template <class T>
bool _Exists(const T* _source, const T* _regex)
{
	const T* source = _source; //検索対象
	boost::reg_expression<T, boost::regex_traits<T> > regex(_regex); //正規表現
	boost::match_results<const T*> results; //結果
	//実行
	return boost::regex_search(source, results, regex);
}

bool CRegex::Exists(const wchar_t* szSource, const wchar_t* szPattern)
{
	return _Exists(szSource, szPattern);
}

bool CRegex::Exists(const char* szSource, const char* szPattern)
{
	return _Exists(szSource, szPattern);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           検索                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
template <class T>
void _GetMatchStrings(vector<basic_string<T> >* _ret, const T* _source, const T* _regex)
{
	vector<basic_string<T> >& ret = *_ret;
	const T* source = _source; //検索対象
	boost::reg_expression<T, boost::regex_traits<T> > regex(_regex); //正規表現
	boost::match_results<const T*> results; //結果
	//実行
	bool found = boost::regex_search(source, results, regex);
	//解釈
	if(found){
		for(int i=0;i<(int)results.size();i++){
			ret.push_back(results.str(i));
		}
	}
}

vector<wstring> CRegex::GetMatchStrings(const wchar_t* szSource, const wchar_t* szPattern)
{
	vector<wstring> v;
	_GetMatchStrings(&v, szSource, szPattern);
	return v;
}

vector<string> CRegex::GetMatchStrings(const char* szSource, const char* szPattern)
{
	vector<string> v;
	_GetMatchStrings(&v, szSource, szPattern);
	return v;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           置換                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
string CRegex::Replace(const char* szSource, const char* szPattern, const char* szReplacing)
{
	boost::reg_expression<char, boost::regex_traits<char> > regex(szPattern);
	return boost::regex_replace((string)szSource, regex, (string)szReplacing);
}

wstring CRegex::Replace(const wchar_t* szSource, const wchar_t* szPattern, const wchar_t* szReplacing)
{
	boost::reg_expression<wchar_t, boost::regex_traits<wchar_t> > regex(szPattern);
	return boost::regex_replace((wstring)szSource, regex, (wstring)szReplacing);
}
