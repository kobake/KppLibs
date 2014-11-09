#pragma once

#include <string>
#include <IoLib.h> //CFilePath

class CIniSection{
public:
	//コンストラクタ・デストラクタ
	CIniSection(const wchar_t* _fpath, const wchar_t* _section_name);
	~CIniSection();

public:
	//基本情報
	const wchar_t*	GetSectionName() const{ return m_strSectionName.c_str(); }	//!<セクション名を取得

	//読取
	std::wstring	GetStringEx(const wchar_t* szKey, const wchar_t* def) const;	//!<文字列を取得。環境変数%～%を置換する
	std::wstring	GetString(const wchar_t* szKey, const wchar_t* def) const;	//!<文字列を取得
	
	//書込
	void			WriteString(const wchar_t* szKey, const wchar_t* szValue);		//!<文字列を設定

private:
	std::wstring	m_strFilePath;
	std::wstring	m_strSectionName;
};


class CIniFile{
public:
	// 初期化と終了 //
	CIniFile(const wchar_t* _fpath);
	const CFilePath&	GetPath() const{ return m_cFilePath; }

	// セクション //
	int					GetSectionCount() const;
	CIniSection			GetSectionByIndex(int iSectionIndex) const;
	CIniSection			GetSectionByName(const wchar_t* szSectionName) const;

	// 読取 //
	std::wstring		GetStringEx(const wchar_t* szSection, const wchar_t* szKey, const wchar_t* def);	//!< 文字列を取得。環境変数%～%を置換する
	std::wstring		GetString(const wchar_t* szSection, const wchar_t* szKey, const wchar_t* def);		//!< 文字列を取得

	// 書込 //
	void				WriteString(const wchar_t* szSection, const wchar_t* szKey, const wchar_t* szValue);	//!< 文字列を設定

private:
	CFilePath	m_cFilePath;
};


#pragma comment(lib,"comsuppw.lib") //_bstr_t

