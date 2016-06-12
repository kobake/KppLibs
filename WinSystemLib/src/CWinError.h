#pragma once

#include <wchar.h>
#include <BaseLib.h> // libfwd_win

class CWinError{
public:
	CWinError();
	void update();             //!<直前のWindowsエラーを保持する
	void clear();              //!<保持していたエラーを解放する
	void show_error() const;   //!<ダイアログで表示
	void throw_error() const;  //!<例外を投げる
	const wchar_t* what_w() const; //!<エラーメッセージを取得
public:
	static void s_getMessage(wchar_t* buf,size_t count,DWORD error_code);
private:
	DWORD error_code;
	wchar_t msg[256];
};

