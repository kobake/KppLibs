#pragma once

#include "CFilePath.h"
#include <windows.h>

class FileFind{
public:
	//コンストラクタ・デストラクタ
	FileFind();
	~FileFind();
	//インターフェース
	bool findFirst(const wchar_t* fdir,const wchar_t* ftitle);
	bool findFirst(const wchar_t* fpath);
	bool findNext();
	void findClose();
	//取得
	const CFilePath& getFound() const{ return path; }
	bool isFolder() const;

private:
	HANDLE hFind;
	WIN32_FIND_DATA finddata;

	CFilePath path;
};
