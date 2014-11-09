#pragma once

#include <IoLib.h> // CFilePath
#include <windows.h>

class FolderDialog{
public:
	//コンストラクタ・デストラクタ
	FolderDialog();
	virtual ~FolderDialog();
	//インターフェース
	bool showSelect(const wchar_t* caption,HWND owner);
	const CFilePath& GetPath() const{ return m_fpath; }
	CFilePath& GetPath(){ return m_fpath; }
private:
	CFilePath m_fpath;
};


