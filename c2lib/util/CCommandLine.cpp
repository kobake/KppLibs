#include "CCommandLine.h"

void CCommandLine::Initialize(int argc, wchar_t* argv[])
{
	for(int i = 1; i < argc; i++){
		m_vParameters.push_back(argv[i]);
		if(0){
		}
//		if(argv[i][0] == L'/'){
//			// オプション //
//		}
		else if(argv[i][0] == L'-' && argv[i][1] == L'-'){
			// オプション //
		}
		else if(argv[i][0] == L'-' && argv[i][1] != L'-'){
			// オプション //
		}
		else if(argv[i][0] == L'/'){
			// オプション //
		}
		else{
			m_vValuedParameters.push_back(argv[i]);
		}
	}
}

bool CCommandLine::GetFlagBool(const wchar_t* szKey) const
{
	return FindParam(szKey) != -1;
}

/*
	szKey … "/hoge"
*/
std::wstring CCommandLine::GetFlagString(const wchar_t* szKey) const
{
	std::wstring strKeyEq = (std::wstring)szKey + L"=";
	for(int i = 0; i < (int)m_vParameters.size(); i++){
		/*
			/hoge abc
		*/
		if(wcscmp(m_vParameters[i].c_str(), szKey) == 0){
			if(i + 1 < (int)m_vParameters.size() && m_vParameters[i + 1].at(0) != L'/'){
				return m_vParameters[i + 1];
			}
			else{
				return L"";
			}
		}
		/*
			/hoge=abc
			/hoge="abc"
		*/
		else if(wcsncmp(m_vParameters[i].c_str(), strKeyEq.c_str(), strKeyEq.length()) == 0){
			return m_vParameters[i].c_str() + strKeyEq.length();
		}
	}
	return L"";
}

int CCommandLine::FindParam(const wchar_t* szKey) const
{
	for(int i = 0; i < (int)m_vParameters.size(); i++){
		if(wcscmp(m_vParameters[i].c_str(), szKey) == 0){
			return i;
		}
	}
	return -1;
}

std::wstring CCommandLine::GetParam(int iIndex) const
{
	if(iIndex >= 0 && iIndex < (int)m_vParameters.size()){
		return m_vParameters[iIndex].c_str();
	}
	return L"";
}

std::wstring CCommandLine::GetValuedParam(int iIndex) const
{
	if(iIndex >= 0 && iIndex < (int)m_vValuedParameters.size()){
		return m_vValuedParameters[iIndex].c_str();
	}
	return L"";
}
