#include "_required.h"
#include "Io.h"
#include "file.h"

//!ファイル名の変更。失敗した場合は例外を投げる。
void Io::rename(
	const wchar_t* oldpath,
	const wchar_t* newpath,
	bool overwrite)
{
	//oldpathの存在チェック
	if(!fexist(oldpath))throw myexception(L"rename: oldpath[%s]が存在しません",oldpath);

	//上書きを許す場合、新しいパスに存在するファイルを削除
	if(overwrite)_wremove(newpath);

	//rename実行
	int ret=::_wrename(oldpath,newpath);
	if(ret!=0){
		throw myexception(L"rename(%s,%s) failed.",oldpath,newpath);
	}
}

