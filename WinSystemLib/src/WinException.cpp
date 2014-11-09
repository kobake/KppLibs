#include "_required.h"
#include "WinException.h"
#include "CWinError.h"
#include <windows.h>

WinException::WinException()
{
	CWinError::s_getMessage(str,countof(str),GetLastError());
}

