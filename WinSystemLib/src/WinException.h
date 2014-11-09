#pragma once

#include "std/myexception.h"

//!GetLastErrorを元にwhat()を構築する例外クラス
class WinException : public wexception{
public:
	WinException();
	const wchar_t* what_w() const throw(){ return str; }
private:
	wchar_t str[256];
};

#define GEN_WIN_EXCEPTION(CLASS) \
class CLASS : public WinException{ \
public: \
	CLASS() : WinException() { } \
}
