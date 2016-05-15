#include "_required.h"
#include "myexception.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <StringLib.h>


const char* wexception::what() const throw()
{
	static char buf[256];
	safe_wcstombs(what_w(), buf, _countof(buf));
	return buf;
}

myexception::myexception(const char* format, ...)
{
	va_list v;
	va_start(v,format);
	vsprintf_s(m_buf, _countof(m_buf), format, v);
	va_end(v);
}

myexception::myexception(const wchar_t* format, ...)
{
	wchar_t wbuf[1024];
	va_list v;
	va_start(v,format);
	vswprintf_s(wbuf, _countof(wbuf), format, v);
	va_end(v);

	safe_wcstombs(wbuf, m_buf, _countof(m_buf));
}
void myexception::assignv(const wchar_t* _str, va_list& mark)
{
	wchar_t wbuf[1024];
	vswprintf_s(wbuf, _countof(wbuf), _str, mark);
	safe_wcstombs(wbuf, m_buf, _countof(m_buf));
}



mywexception::mywexception(const wchar_t* _str,...)
{
	va_list v;
	va_start(v,_str);
	vswprintf(str,countof(str),_str,v);
	va_end(v);
}

void mywexception::assignv(const wchar_t* _str, va_list& mark)
{
	vswprintf(str,countof(str),_str,mark);
}

