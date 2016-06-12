#pragma once

#include <string>
#include "../StringLib/src/mywstring_funcs.h" // fextract2

/*
	http://yahoo.com/abc/def
	↓
	[http://] [yahoo.com] [/abc/def]

	local://C:/abc/def
	↓
	[local://] [C:] [/abc/def]

	samba://landisk/abc/def
	↓
	[samba://] [landisk] [/abc/def]

	ftp://yahoo.com/abc/def
	↓
	[ftp://] [yahoo.com] [/abc/def]

	ftp://kobake:pass@yahoo.com/abc/def
	↓
	[ftp://] [kobake:pass@] [yahoo.com] [/abc/def]

	local://var/www/html
	↓
	[local://] [var/www/html]
*/


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// プロトコル内パス
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
/*
	パス。
	区切り文字は常に「/」

	例：
		/www.yahoo.co.jp/hogehoge/abc
		/kobake:pass@www.yahoo.co.jp/hogehoge/abc
		/var/www/html
		/c:/hoge/hage
*/
class CInsidePath{
public:
	CInsidePath() : m_strPath(L"") {}
	explicit CInsidePath(const wchar_t* szPath) : m_strPath(szPath){ _Extract(); }
	CInsidePath(const CInsidePath& cDir, const wchar_t* szItemName) : m_strPath(cDir.m_strPath + L"/" + szItemName) { _Extract(); }
	// 取得 //
	const wchar_t*	GetString() const				{ return m_strPath.c_str(); }
	const wchar_t*	GetStringWithoutHead() const	{ return m_strPath.length()?(m_strPath.c_str() + 1):m_strPath.c_str(); }

protected:
	// 「..」「.」の展開 //
	void _Extract()
	{
		if(wcsstr(m_strPath.c_str(), L"..") == 0 || wcsstr(m_strPath.c_str(), L"./") == 0 || wcsstr(m_strPath.c_str(), L"/.") == 0){ // ###
			m_strPath = fextract2(m_strPath.c_str(), L'/');
		}
		if(m_strPath.length() == 0){
			m_strPath = L"/";
		}
	}

private:
	std::wstring	m_strPath;
};

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// プロトコル付きパス
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
/*
	区切り文字は常に「/」

	例：
		http://www.yahoo.co.jp/hogehoge/abc
		ftp://kobake:pass@www.yahoo.co.jp/hogehoge/abc
		samba://var/www/html
		local://c:/hoge/hage
*/
class CCompletePath{
public:
	// 初期化と終了 //
	CCompletePath() : m_strPath(L"") {}
	explicit CCompletePath(const wchar_t* szPath) : m_strPath(szPath) { _Extract(); }
	CCompletePath(const CCompletePath& cDir, const wchar_t* szItemName)
	{
		if(wcslen(cDir.GetInsidePath().GetStringWithoutHead()) == 0){
			m_strPath = cDir.m_strPath + szItemName; // ※ルートからのアクセス //
		}
		else{
			m_strPath = cDir.m_strPath + L"/" + szItemName;
		}
		_Extract();
	}
	/*
		そのまま取得。
	*/
	const wchar_t* GetString() const
	{
		return m_strPath.c_str();
	}
	/*
		プロトコルを取得。

		例："http", "ftp", "local"
	*/
	std::wstring GetProtocol() const
	{
		const wchar_t* path = m_strPath.c_str();
		const wchar_t* p = wcsstr(path, L"://");
		if(p){
			return std::wstring(path, p);
		}
		else{
			return L"local"; // #### 例外 //
		}
	}
	/*
		内部パスを取得。

		例："/c:/hoge/hage", "/var/www/html"
	*/
	CInsidePath GetInsidePath() const
	{
		const wchar_t* path = m_strPath.c_str();
		const wchar_t* p = wcsstr(path, L"://");
		if(p){
			return CInsidePath(p + 2); // ※先頭の「/」を含める //
		}
		else{
			return CInsidePath(path);
		}
	}
	/*
		アイテム名を取得

		例："hoge.txt", "a.conf"
	*/
	const wchar_t* GetItemName() const
	{
		const wchar_t* p = wcsrchr(m_strPath.c_str(), L'/');
		if(p){
			return p + 1;
		}
		else{
			return m_strPath.c_str();
		}
	}
	/*
		比較
	*/
	int Compare(const CCompletePath& rhs) const
	{
		return m_strPath.compare(rhs.m_strPath);
	}

protected:
	// 「..」「.」の展開 //
	void _Extract()
	{
		if(wcsstr(m_strPath.c_str(), L"..") == 0 || wcsstr(m_strPath.c_str(), L"./") == 0 || wcsstr(m_strPath.c_str(), L"/.") == 0){ // ###
			std::wstring strProtocol = this->GetProtocol();
			std::wstring strInside = fextract2(this->GetInsidePath().GetString(), L'/');
			if(strInside.length() == 0){
				strInside = L"/";
			}
			m_strPath = strProtocol + L":/" + strInside;
		}
	}

private:
	std::wstring	m_strPath;
};
