#pragma once

#include <windows.h>
#include <IoLib.h> // CFilePath

class Module{
public:
	//コンストラクタ・デストラクタ
	Module(HINSTANCE _hInst=NULL);
	//インターフェース
	const CFilePath& GetPath() const{ return path; }
private:
	HINSTANCE hInst;
	CFilePath path;
};
