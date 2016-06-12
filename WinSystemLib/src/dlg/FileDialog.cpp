#include <BaseLib.h>
#include "FileDialog.h"
#include <StringLib.h>
#include <shlobj.h>
using namespace util;

FileDialog::FileDialog(wchar_t *_filter,wchar_t *_defext)
{
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.hwndOwner=NULL; //後で
	ofn.hInstance=NULL;
	ofn.lpstrFilter=filter; memcpy(filter,_filter,sizeof(filter));
	ofn.lpstrCustomFilter=NULL; //調査中
	ofn.nMaxCustFilter=0;       //調査中
	ofn.nFilterIndex=0;         //調査中
	ofn.lpstrFile=NULL;         //後で
	ofn.nMaxFile=_MAX_PATH-1;
	ofn.lpstrFileTitle=NULL;
	ofn.nMaxFileTitle=0;
	ofn.lpstrInitialDir=NULL;   //後で
	ofn.lpstrTitle=NULL;        //後で
	ofn.Flags=0;                //後で
	ofn.nFileOffset=0;          //調査中
	ofn.nFileExtension=0;       //調査中
	ofn.lpstrDefExt=NULL; if(_defext!=NULL && *_defext!='\0')ofn.lpstrDefExt=wcsdup(_defext);
	ofn.lCustData=0;            //調査中
	ofn.lpfnHook=NULL;          //調査中
	ofn.lpTemplateName=NULL;    //調査中
}

FileDialog::~FileDialog()
{
	free((void*)ofn.lpstrDefExt);
}

bool FileDialog::showOpen(const wchar_t* caption,HWND owner)
{
	int test=sizeof(FileDialog);

	//デフォルトのファイル名
	wchar_t buf[_MAX_PATH];
	wcssafecpy(buf,fpath.GetPath(),countof(buf));
	
	//デフォルトのディレクトリ
	if(*fpath.GetDir()){
		wcssafecpy(initialdir,fpath.GetDir(),countof(initialdir));
	}else{
		SHGetSpecialFolderPath(NULL,initialdir,CSIDL_PERSONAL,FALSE); //マイドキュメント
	}
	
	//その他設定
	ofn.lpstrFile=buf;
	ofn.lpstrTitle=caption;
	ofn.hwndOwner=owner;
	ofn.Flags=OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_PATHMUSTEXIST;

	//ダイアログ表示
	BOOL ret=GetOpenFileName(&ofn);

	//CFilePathに変換
	if(ret){
		fpath.SetPath(buf);
	}

	return ret!=FALSE;
}

bool FileDialog::showSave(const wchar_t* caption,HWND owner)
{
	//デフォルトのファイル名
	wchar_t buf[_MAX_PATH];
	wcssafecpy(buf,fpath.GetPath(),countof(buf));

	//デフォルトのディレクトリ
	if(*fpath.GetDir()){
		wcssafecpy(initialdir,fpath.GetDir(),countof(initialdir));
	}else{
		SHGetSpecialFolderPath(NULL,initialdir,CSIDL_PERSONAL,FALSE); //マイドキュメント
	}

	//その他設定
	ofn.lpstrFile=buf;
	ofn.lpstrTitle=caption;
	ofn.hwndOwner=owner;
	ofn.Flags=OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

	//ダイアログ表示
	BOOL ret=GetSaveFileName(&ofn);

	//CFilePathに変換
	if(ret){
		fpath.SetPath(buf);
	}

	return ret!=FALSE;
}
