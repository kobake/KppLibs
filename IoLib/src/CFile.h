#pragma once

#include <string>
#include <vector>
#include "CFilePath.h"

namespace IoLib{

class CFile{
public:
	CFile() : m_cPath(L"") { }
	CFile(const CFilePath& cPath) : m_cPath(cPath) { }
	CFile(const wchar_t* szDir, const wchar_t* szTitle) : m_cPath(szDir, szTitle) { }
	//読み書き
	void ReadFileBytes(std::vector<unsigned char>* pvBuf) const;
	//属性値
	bool	IsDirectory() const;
	bool	IsFile() const;
	bool	IsExist() const;
	//パス
	bool IsValid() const{ return wcslen(m_cPath.GetPath())>=1; }
	const wchar_t* GetPath() const{ return m_cPath.GetPath(); }
	const CFilePath& GetPathClass() const{ return m_cPath; }
	//各種属性
	__int64 GetFileSize() const;
	time_t	GetCreatedTime() const;		//作成日時
	time_t	GetAccessedTime() const;	//アクセス日時
	time_t	GetModifiedTime() const;	//更新日時
protected:
	//実装補助
	void _AdjustPath(); //パス内の「..」および「.」を解決
private:
	CFilePath m_cPath;
//	std::wstring m_strPath;
};

}
using namespace IoLib;

#if 0

#include "CFilePath.h"

class File{
public:
	//コンストラクタ・デストラクタ
	File(const char* _path="") : path(_path) { }
	File(const char* _dir,const char* _title) : path(_dir,_title) { }
	explicit File(const CFilePath& _path) : path(_path) { }
	virtual ~File(){ }
	//作成・破棄
	void create(int att); //0:ファイル 1:フォルダ
	void remove();
	//移動・コピー
	void move(const CFilePath& dst);
	void copy(const CFilePath& dst);
	//情報取得
	bool exist();
	int GetSize();
	int getAttribute(); //0:ファイル 1:フォルダ -1:なし
	//アクセス
	void touch();
private:
	CFilePath path;
public:
	// -- -- CFilePathラッパー -- -- //
	//設定
	void SetPath(const char* _path)						{ return path.SetPath(_path); }
	void SetPath(const char* _dir,const char* _title)	{ return path.SetPath(_dir,_title); }
	void SetDirPath(const char* _dir)						{ return path.SetDirPath(_dir); }
	void SetTitle(const char* _title)					{ return path.SetTitle(_title); }
	void SetExt(const char* _ext)						{ return path.SetExt(_ext); }
	//取得
	const char* GetPath() const							{ return path.GetPath(); }
	const char* GetDir() const							{ return path.GetDir(); }
	const char* GetTitle() const						{ return path.GetTitle(); }
	const char* GetExt() const							{ return path.GetExt(); }
public:
	// -- -- 例外 -- -- //
	class touch_failed : public std::exception{
	public:
		const char* what() const{ return "File::touch_failed"; }
	};
};


#endif
