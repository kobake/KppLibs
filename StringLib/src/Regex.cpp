#include "Regex.h"
#include <boost/regex.hpp>
#include <string>
#include <vector>
using namespace std;


typedef boost::reg_expression<wchar_t, boost::regex_traits<wchar_t> > RegExpression;


Regex::Regex(const mystring& pattern)
{
	RegExpression* p = new RegExpression(pattern.c_str());
	m_regex = p;
}

Regex::~Regex()
{
	if(m_regex){
		RegExpression* p = (RegExpression*)m_regex;
		delete p;
		m_regex = NULL;
	}
}

myvector<mystring> Regex::Match(const mystring& target) const
{
	// 検索
	RegExpression& regex = *((RegExpression*)m_regex);
	boost::match_results<const wchar_t*> results; //結果
	bool found = boost::regex_search(target.c_str(), results, regex);
	// 結果構築
	myvector<mystring> ret;
	if(found){
		for(int i=0;i<(int)results.size();i++){
			ret.push_back(results.str(i));
		}
	}
	return ret;
}

bool Regex::IsMatch(const mystring& target) const
{
	// 検索
	RegExpression& regex = *((RegExpression*)m_regex);
	boost::match_results<const wchar_t*> results; //結果
	bool found = boost::regex_search(target.c_str(), results, regex);
	return found;
}

mystring Regex::Replace(const mystring& target, const mystring& after) const
{
	RegExpression& regex = *((RegExpression*)m_regex);
	return boost::regex_replace(target, regex, after);
}
