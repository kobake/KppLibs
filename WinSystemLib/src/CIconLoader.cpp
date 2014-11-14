#include "common/_required.h"
#include "CIconLoader.h"
#include <string>
#include <ShellAPI.h>
using namespace std;

HICON CIconLoader::LoadIconOfFolder()
{
	SHFILEINFO fi;

	//フラグ
	UINT uFlags = 0;
	uFlags |= SHGFI_ICON;			//アイコンハンドルを取得 -> hIcon (※使い終わったら、アプリ側でDestroyIconする)
	uFlags |= SHGFI_SMALLICON;		//小さいアイコン
	uFlags |= SHGFI_SYSICONINDEX;	//システムイメージリスト内インデックスを取得 -> iIcon
	uFlags |= SHGFI_USEFILEATTRIBUTES;

	//アイコン情報の取得
	DWORD dwShellResult = (DWORD)SHGetFileInfo(
		L"",
		FILE_ATTRIBUTE_DIRECTORY,
		&fi,
		sizeof(fi),
		uFlags
	);
	if(dwShellResult!=0){
		return fi.hIcon;
	}
	else{
		return NULL;
	}
}


HICON CIconLoader::LoadIconOfExt(
	const wchar_t* szExt	//「.txt」のような形式
)
{
	SHFILEINFO fi;

	//フラグ
	UINT uFlags = 0;
	uFlags |= SHGFI_ICON;			//アイコンハンドルを取得 -> hIcon (※使い終わったら、アプリ側でDestroyIconする)
	uFlags |= SHGFI_SMALLICON;		//小さいアイコン
	uFlags |= SHGFI_SYSICONINDEX;	//システムイメージリスト内インデックスを取得 -> iIcon
	uFlags |= SHGFI_USEFILEATTRIBUTES;

	//アイコン情報の取得
	wstring strPath = L"*"; strPath+=szExt;
	DWORD dwShellResult = (DWORD)SHGetFileInfo(
		strPath.c_str(),
		0,
		&fi,
		sizeof(fi),
		uFlags
	);
	if(dwShellResult!=0){
		return fi.hIcon;
	}
	else{
		return NULL;
	}
}


//!ファイルを示すアイコンをロード
HICON CIconLoader::LoadIconOfFile(const wchar_t* szPath)
{
	SHFILEINFO fi;

	//フラグ
	UINT uFlags = 0;
	uFlags |= SHGFI_ICON;			//アイコンハンドルを取得 -> hIcon (※使い終わったら、アプリ側でDestroyIconする)
	uFlags |= SHGFI_SMALLICON;		//小さいアイコン
	uFlags |= SHGFI_SYSICONINDEX;	//システムイメージリスト内インデックスを取得 -> iIcon
	uFlags |= SHGFI_ADDOVERLAYS;

	//※ドライブの場合はオーバーレイ無効
	if(wcslen(szPath)<=3){ //####仮
		uFlags &= ~SHGFI_ADDOVERLAYS;
	}
	
	//アイコン情報の取得
	DWORD dwShellResult = (DWORD)SHGetFileInfo(
		szPath,
		0,
		&fi,
		sizeof(fi),
		uFlags
	);
	if(dwShellResult!=0){
		return fi.hIcon;
	}
	else{
		return NULL;
	}
}

