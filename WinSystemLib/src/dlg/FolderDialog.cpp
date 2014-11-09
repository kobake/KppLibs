#include "common/_required.h"
#include "FolderDialog.h"
#include <StringLib.h>
using namespace util;
#include <shlobj.h>

FolderDialog::FolderDialog()
{
}

FolderDialog::~FolderDialog()
{
}


int CALLBACK DirSelectDialog_Callback(HWND hwnd,UINT msg,LPARAM lParam,LPARAM lpData)
{
	if(msg==BFFM_INITIALIZED){
		SendMessage(hwnd,BFFM_SETSELECTION,(WPARAM)TRUE,lpData);
	}
	return 0;
}

bool FolderDialog::showSelect(const wchar_t* caption,HWND owner)
{
	//構造体準備
	wchar_t tmp[_MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner=owner;
	bi.pidlRoot=NULL; //ツリーのルート
	bi.pszDisplayName=tmp; //フォルダ名(たとえば「ローカルディスク(C:)」など)
	bi.lpszTitle=caption;
	bi.ulFlags=BIF_BROWSEFORCOMPUTER | BIF_RETURNONLYFSDIRS;
	bi.lpfn=DirSelectDialog_Callback;
	bi.lParam=(LPARAM)m_fpath.GetDir();
	bi.iImage=0;

	//アロケータ取得
	LPMALLOC pMalloc;
	SHGetMalloc(&pMalloc);

	//ダイアログ表示
	ITEMIDLIST* ret=SHBrowseForFolder(&bi);

	if(ret!=NULL){
		//フォルダパスを取得
		wchar_t buf[_MAX_PATH];
		SHGetPathFromIDList(ret,buf);

		//CFilePathに変換
		wcssafecat(buf,L"\\",countof(buf));
		m_fpath.SetPath(buf);
	}

	//戻り値解放
	if(ret)pMalloc->Free(ret);

	//アロケータ解放
	pMalloc->Release();
	
	//完
	return ret!=NULL;
}

