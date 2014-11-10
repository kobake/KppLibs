#include "CCommandLine.h"

void CCommandLine::Initialize(const wchar_t* _str)
{
	size_t n = wcslen(_str);
	std::vector<wchar_t> vbuf(n + 1);
	std::vector<std::wstring> args;
	wchar_t* buf = &vbuf[0];
	wchar_t* p = buf;
	while(1){
		// スペース読み飛ばし
		if(*p == L' ')p++;

		// ダブルクォーテーション読み飛ばし
		wchar_t sep = L' ';
		if(*p == L'"'){
			p++;
			sep = L'"';
		}

		// 単語抽出
		wchar_t* q = wcschr(p, sep);
		if(!q)q = wcschr(p, L'\0');
		std::wstring arg(p, q);

		// 最後のダブルクォーテーション読み飛ばし
		if(*q == L'"'){
			q++;
		}
		p = q;

		// 追加
		args.push_back(arg);
	}
	Initialize(args);
}

int CCommandLine::argc() const
{
	return (int)m_vParameters.size();

}

// ※一度呼んだら次以降からは内容が変わらないことに注意。
//   あと、m_vParametersの内容が変わったら、これの戻り値は壊れることに注意。（将来対応予定）
const wchar_t** CCommandLine::argv() const
{
	int n = (int)m_vParameters.size();
	static std::vector<wchar_t*> buf(n);
	for(int i = 0; i < n; i++){
		buf[i] = const_cast<wchar_t*>(this->m_vParameters[i].c_str());
	}
	wchar_t** p = &buf[0];
	return (const wchar_t**)p;
}

void CCommandLine::Initialize(int argc, wchar_t* argv[])
{
	std::vector<std::wstring> args;
	for(int i = 0; i < argc; i++){
		args.push_back(argv[i]);
	}
	Initialize(args);
}

void CCommandLine::Initialize(const std::vector<std::wstring>& args)
{
	for(int i = 0; i < (int)args.size(); i++){
		m_vParameters.push_back(args[i]);
		if(0){
		}
//		if(argv[i][0] == L'/'){
//			// オプション //
//		}
		else if(args[i][0] == L'-' && args[i][1] == L'-'){
			// オプション //
		}
		else if(args[i][0] == L'-' && args[i][1] != L'-'){
			// オプション //
		}
		else if(args[i][0] == L'/'){
			// オプション //
		}
		else{
			m_vValuedParameters.push_back(args[i]);
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
