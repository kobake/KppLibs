#include "CConfFile.h"
#include <StringLib.h>

inline bool IS_SPACE(wchar_t c)
{
	return c == L'\t' || c == L' ';
}

CConfFile::CConfFile()
{
}

CConfFile::CConfFile(const wchar_t* szConfFile)
{
	Load(szConfFile);
}
CConfFile::operator bool() const
{
	return m_values.size() > 0;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 読み書き
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CConfFile::Load(const wchar_t* szConfFile)
{
	::setlocale(LC_ALL, "");
	FILE* fp = _wfopen(szConfFile, L"rt");
	if(!fp)return;
	wchar_t buf[1024];
	while(fgetws(buf, _countof(buf), fp) != NULL){
		wchar_t* p;
		wchar_t* line = buf;
		// 改行除去
		p = wcschr(line, L'\n');
		if(p)*p = '\0';
		// コメント除去
		p = wcschr(line, L'#');
		if(p)*p = '\0';
		// 前後空白除去
//		while(IS_SPACE(*p))p++;
//		line = p;
//		p = wcsrchr(line, '\0');
//		while(p - 1 >= line && IS_SPACE(*(p - 1)))p--;
//		*p = '\0';
		// 解釈
		std::vector<std::wstring> v = wcssplit(line, L'=');
		if(v.size() >= 2){
			std::wstring key = cpp_wcstrim(v[0].c_str(), L" \t");
			std::wstring value = cpp_wcstrim(v[1].c_str(), L" \t");
			if(key.length() > 0){
				m_values[key] = value;
			}
		}
	}
	fclose(fp);
}

void CConfFile::Save(const wchar_t* szConfFile) const
{
	::setlocale(LC_ALL, "");
	// -- -- 読みながら保存予約構築 -- -- //
	std::map<std::wstring, std::wstring> tmpValues = m_values;
	std::vector<std::wstring> toWrite;
	FILE* fp = _wfopen(szConfFile, L"rt");
	if(!fp)goto LBL_WRITE;
	wchar_t buf[1024];
	bool bLastReturn = false;
	while(fgetws(buf, _countof(buf), fp) != NULL){
		wchar_t* p;
		wchar_t* line = buf;
		std::wstring originalLine = buf;
		// 改行除去
		bLastReturn = false;
		std::wstring strReturn;
		p = wcschr(line, L'\n');
		if(p){
			bLastReturn = true;
			strReturn = L"\n";
			*p = '\0';
		}
		// コメント除去＆一時保持
		p = wcschr(line, L'#');
		std::wstring strComment;
		if(p){
			strComment = p;
			*p = '\0';
		}
		// 解釈
		std::wstring newLine = originalLine;
		std::vector<std::wstring> v = wcssplit(line, L'=');
		if(v.size() >= 2){
			std::wstring key = cpp_wcstrim(v[0].c_str(), L" \t");
			std::wstring value = cpp_wcstrim(v[1].c_str(), L" \t");
			if(key.length() > 0){
				std::map<std::wstring, std::wstring>::iterator itr = tmpValues.find(key);
				if(itr != tmpValues.end()){
					newLine = key + L" = " + itr->second;
					if(strComment.length()){
						newLine += L" ";
						newLine += strComment;
					}
					newLine += strReturn;
					// tmpValuesから除去
					tmpValues.erase(itr);
				}
			}
		}
		// 構築準備
		toWrite.push_back(newLine);
	}
	fclose(fp);
	// 最後の行に改行がなかったら改行を追加
	if(!bLastReturn && toWrite.size() > 0){
		toWrite.push_back(L"\n");
	}
LBL_WRITE:
	// tmpValuesに残ってる分も toWrite に push
	for(std::map<std::wstring, std::wstring>::const_iterator itr = tmpValues.begin(); itr != tmpValues.end(); itr++){
		std::wstring strNewLine = itr->first + L" = " + itr->second + L"\n";
		toWrite.push_back(strNewLine);
	}
	// -- -- 書込 -- -- //
	fp = _wfopen(szConfFile, L"wt");
	if(!fp)return;
	for(int i = 0; i < (int)toWrite.size(); i++){
		fprintf(fp, "%ls", toWrite[i].c_str());
	}
	fclose(fp);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 取得
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
std::wstring CConfFile::GetString(const wchar_t* szKey) const
{
	std::map<std::wstring, std::wstring>::const_iterator itr = m_values.find(szKey);
	if(itr == m_values.end())return L"";
	return itr->second;
}

int CConfFile::GetInt(const wchar_t* szKey, int defaultValue) const
{
	std::wstring v = GetString(szKey);
	if(v.length()){
		return _wtoi(v.c_str());
	}
	else{
		return defaultValue;
	}
}

int CConfFile::GetHexInt(const wchar_t* szKey, int defaultValue) const
{
	std::wstring v = GetString(szKey);
	if(v.length()){
		return wcstol(v.c_str(), NULL, 16);
	}
	else{
		return defaultValue;
	}
}

bool CConfFile::Exists(const wchar_t* szKey) const
{
	return m_values.find(szKey) != m_values.end();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 設定
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CConfFile::SetValue(const wchar_t* szKey, const wchar_t* szValue)
{
	m_values[szKey] = szValue;
}
