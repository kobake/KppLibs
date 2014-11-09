#include "common/_required.h"
#include "CIniFile.h"
#include <StringLib.h>
#include "util/file.h"
#include <IoLib.h> //CFileStream.h
#include <comutil.h>
#include <windows.h>
using namespace std;
using namespace util;

// ================================================================================ //
// CIniSection
// ================================================================================ //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 初期化と終了
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
CIniSection::CIniSection(const wchar_t* _fpath,const wchar_t* _section_name)
: m_strFilePath(_fpath), m_strSectionName(_section_name)
{
}

CIniSection::~CIniSection()
{
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 読取
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//!文字列を取得
wstring CIniSection::GetString(const wchar_t* szKey,const wchar_t* def) const
{
	static wchar_t tmp[_MAX_PATH];
	GetPrivateProfileString(m_strSectionName.c_str(),szKey,def,tmp,countof(tmp),m_strFilePath.c_str());
	return tmp;
}

//!文字列を取得。環境変数%～%を置換する
wstring CIniSection::GetStringEx(const wchar_t* szKey,const wchar_t* def) const
{
	static wchar_t tmp[_MAX_PATH];
	static wchar_t tmp2[_MAX_PATH];
	GetPrivateProfileString(m_strSectionName.c_str(),szKey,def,tmp,countof(tmp),m_strFilePath.c_str());
	ExpandEnvironmentStrings(tmp,tmp2,_MAX_PATH);
	return tmp2;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 書込
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//!文字列を設定
void CIniSection::WriteString(const wchar_t* szKey,const wchar_t* szValue)
{
	ftouch(m_strFilePath.c_str());
	WritePrivateProfileString(m_strSectionName.c_str(), szKey, szValue, m_strFilePath.c_str());
}



// ================================================================================ //
// CIniFile
// ================================================================================ //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// 初期化と終了
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CIniFile::CIniFile(const wchar_t* _fpath)
: m_cFilePath(_fpath)
{
	//フルパスでない場合、カレントディレクトリまたはシステムディレクトリからファイルを探し、
	//フルパスを割り当てる
	if(wcswcs(_fpath,L":")==NULL){
		wchar_t buf[_MAX_PATH];
		wchar_t* delimiter;
		DWORD ret=SearchPath(NULL,_fpath,NULL,countof(buf),buf,&delimiter);
		if(ret!=0){
			m_cFilePath.SetPath(buf);
		}
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  セクションへのアクセス                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//※注意：ANSIのみ受付
int CIniFile::GetSectionCount() const
{
	try{
		CFileInputStream in(m_cFilePath.GetPath(),L"rt");
		//行の先頭の"["をカウントする
		int cnt=0;
		int c;
		bool linehead=true;
		while((c=in.get())!=EOF){
			if(c=='\n'){ linehead=true; continue; }
			if(linehead){
				if(c=='[')cnt++;
				linehead=false;
			}
		}
		return cnt;
	}
	catch(Io::OpenException){
		return 0;
	}
}

//※注意：ANSIのみ受付
CIniSection CIniFile::GetSectionByIndex(int iSectionIndex) const
{
	CFileInputStream in(m_cFilePath.GetPath(),L"rt");
	//行の先頭の"["をカウントし、iSectionIndexに達するまで走査する
	int cnt=0;
	int c;
	bool linehead=true;
	while((c=in.get())!=EOF){
		if(c=='\n'){ linehead=true; continue; }
		if(linehead){
			if(c=='[')cnt++;
			linehead=false;

			if(cnt-1==iSectionIndex)goto found;
		}
	}
	throw myexception(L"セクション[%d]が見つかりません", iSectionIndex);

found:
	//セクション名を読み取る
	vector<char> buf;
	while(1){
		c=in.get();
		if(c==']' || c==EOF)break;
		if(c=='\n')throw wexception(L"不正なiniファイルです");
		buf.push_back(c);
	}
	buf.push_back('\0');
	_bstr_t section=&buf[0];

	return CIniSection(m_cFilePath.GetPath(),section);
}

CIniSection CIniFile::GetSectionByName(const wchar_t* szSectionName) const
{
	return CIniSection(m_cFilePath.GetPath(), szSectionName);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           読取                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

wstring CIniFile::GetString(const wchar_t* szSection,const wchar_t* szKey,const wchar_t* def)
{
	static wchar_t tmp[_MAX_PATH];
	GetPrivateProfileString(szSection, szKey, def, tmp, countof(tmp), m_cFilePath.GetPath());
	return tmp;
}

wstring CIniFile::GetStringEx(const wchar_t* szSection,const wchar_t* szKey,const wchar_t* def)
{
	static wchar_t tmp[_MAX_PATH];
	static wchar_t tmp2[_MAX_PATH];
	GetPrivateProfileString(szSection, szKey, def, tmp, countof(tmp), m_cFilePath.GetPath());
	ExpandEnvironmentStrings(tmp, tmp2, _MAX_PATH);
	return tmp2;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           書込                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CIniFile::WriteString(const wchar_t* szSection,const wchar_t* szKey,const wchar_t* szValue)
{
	ftouch(m_cFilePath.GetPath());
	WritePrivateProfileString(szSection, szKey, szValue, m_cFilePath.GetPath());
}
