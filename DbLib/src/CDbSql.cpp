#include "_required.h"
#include "CDbSql.h"
using namespace std;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        ‚r‚p‚k\’z                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CDbSql::SetTemplate(const wchar_t* szTemplate)
{
	m_strTemplate = szTemplate;
	//u?v‚Ì”‚ğƒJƒEƒ“ƒg
	int n = 0;
	const wchar_t* p = szTemplate;
	while(*p){
		if(*p==L'?')n++;
		p++;
	}
	//ƒpƒ‰ƒ[ƒ^—Ìˆæ‚ğu?v‚Ì”•ª‚¾‚¯Šm•Û
	m_vParameters.resize(n);
}

void CDbSql::SetParameters(const wchar_t* param1, ...)
{
	va_list v;
	va_start(v,param1);
	const wchar_t* p = param1;
	for(int i=0;i<(int)m_vParameters.size();i++){
		if(i>0){
			p = va_arg(v, const wchar_t*);
		}
		m_vParameters[i] = p;
	}
	va_end(v);
}

void CDbSql::SetParameter(int nIndex, const wchar_t* param)
{
	m_vParameters[nIndex] = param;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        ‚r‚p‚kæ“¾                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

wstring _SafeParam(const wstring& str)
{
	wstring ret = L"";
	const wchar_t* p = str.c_str();
	while(*p){
		if(*p==L'\''){
			ret += L"\'\'";
		}
		else{
			ret += *p;
		}
		p++;
	}
	return ret;
}

const wchar_t* CDbSql::GetSql() const
{
	m_bufSql = L"";
	const wchar_t* p = m_strTemplate.c_str();
	int nIndex = 0;
	while(*p){
		if(*p==L'?'){
			m_bufSql += _SafeParam(m_vParameters[nIndex]);
			nIndex++;
		}
		else{
			m_bufSql += *p;
		}
		p++;
	}
	return m_bufSql.c_str();
}

