#pragma once

#include <stdlib.h> //_MAX_PATH
#include <string>

class CFilePath_Base{
public:
	//コンストラクタ・デストラクタ
	CFilePath_Base(const wchar_t* fpath, wchar_t cDelimiter);
	CFilePath_Base(const wchar_t* fdir,const wchar_t* ftitle, wchar_t cDelimiter);
	virtual ~CFilePath_Base();
	void SetDelimiter(wchar_t cDelimiter);

	//型変換
//	operator const wchar_t*() const{ return GetPath(); }

	//代入
	void Assign(const CFilePath_Base& f);
	CFilePath_Base(const CFilePath_Base& f){ Assign(f); }
	CFilePath_Base& operator = (const CFilePath_Base& f){ Assign(f); return *this; }

	//設定
	void SetFullPath(const wchar_t* fpath){ SetPath(fpath); } // 暫定Alias ###削除予定
	void SetDirPath(const wchar_t* fdir){ SetDir(fdir); } // 暫定Alias ###削除予定
	void SetPath(const wchar_t* fpath);
	void SetPath(const wchar_t* fdir,const wchar_t* ftitle);
	void SetDir(const wchar_t* fdir);
	void SetTitle(const wchar_t* ftitle);
	void SetExt(const wchar_t* ext);

	//取得
	const wchar_t* GetFullPath() const { return GetPath(); } // 暫定Alias ###削除予定
	const wchar_t* GetDirPath() const { return GetDir(); } // 暫定Alias ###削除予定
	const wchar_t* GetPath() const;
	const wchar_t* GetDir() const;
	const wchar_t* GetTitle() const;
	const wchar_t* GetExt() const;	//「.txt」のような形式

	std::wstring GetTitleWithoutExt() const;
	std::wstring GetDrive() const;

protected:
	//区切れ目
	void _UpdateDelimiter();

private:
	wchar_t					m_cDelimiter;		//区切り文字。「\」or「/」

	wchar_t					m_path[_MAX_PATH];
	wchar_t*				m_delimiter;		//ディレクトリとファイルの境目。ディレクトリが無い場合はNULL。
	wchar_t*				m_extpos;			//ファイルと拡張子の境目。拡張子が無い場合はNULL。

	//キャッシュ
	mutable std::wstring	m_strDirCache;

};

class CFilePathWin : public CFilePath_Base{
public:
	CFilePathWin(const std::wstring& strPath) : CFilePath_Base(strPath.c_str(), L'\\') { }
	CFilePathWin(const wchar_t* fpath=L"") : CFilePath_Base(fpath, L'\\') { }
	CFilePathWin(const wchar_t* fdir,const wchar_t* ftitle) : CFilePath_Base(fdir, ftitle, L'\\') { }
};

class CFilePathUnix : public CFilePath_Base{
public:
	CFilePathUnix(const std::wstring& strPath) : CFilePath_Base(strPath.c_str(), L'/') { }
	CFilePathUnix(const wchar_t* fpath=L"") : CFilePath_Base(fpath, L'/') { }
	CFilePathUnix(const wchar_t* fdir,const wchar_t* ftitle) : CFilePath_Base(fdir, ftitle, L'/') { }
};

typedef CFilePathWin CFilePath;
