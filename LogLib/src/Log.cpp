#include "Log.h"
#include <stdio.h>
#include <stdarg.h>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     インターフェース                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void Log::print(const wchar_t* str, ...) const
{
	if(enabled){
		va_list mark;
		va_start(mark,str);
		vwprintf(str,mark);
		va_end(mark);
	}
}

void Log::print(const std::string& str) const
{
	const char* p=str.c_str();
	this->print(L"%hs",p);
}

void Log::print(const std::wstring& str) const
{
	const wchar_t* p=str.c_str();
	this->print(L"%ls",p);
}
