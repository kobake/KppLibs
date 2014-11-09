#include "_required.h"
#include "CWinError.h"
#include "std/myexception.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CWinError::CWinError()
{
	update();
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     インターフェース                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!直前のWindowsエラーを保持する
void CWinError::update()
{
	error_code=GetLastError();

	s_getMessage(msg,countof(msg),error_code);
}

//!保持していたエラーを解放する
void CWinError::clear()
{
	error_code=0;
	msg[0]='\0';
}

//!ダイアログで表示
void CWinError::show_error() const
{
	MessageBox(NULL, msg, L"Error", MB_OK | MB_ICONINFORMATION);
}

//!例外を投げる
void CWinError::throw_error() const
{
	throw myexception(msg);
}

//!エラーメッセージを取得
const wchar_t* CWinError::what_w() const
{
	return msg;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          static                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CWinError::s_getMessage(wchar_t* buf,size_t count,DWORD error_code)
{
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error_code,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), //規定の言語
		buf,
		(DWORD)count,
		NULL
	);
}

