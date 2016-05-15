#include "_required.h"
#include "StdError.h"
#include <errno.h>
#include "std/myexception.h"
#include <windows.h>

StdError::StdError()
: e(errno)
{
}

void StdError::show_error() const
{
	MessageBoxW(NULL, to_string(), L"Error", MB_OK | MB_ICONINFORMATION);
}

void StdError::throw_error() const
{
	throw myexception(to_string());
}

const wchar_t* StdError::to_string() const
{
	char* p=strerror(e);
	static wchar_t buf[256];
	mbstowcs(buf,p,countof(buf));
	return buf;
	/*
	switch(e){
	case E2BIG:
	case EACCES:
	case EAGAIN:
	case EBADF:
	case ECHILD:
	case EDEADLOCK:
	case EDOM:
	case EEXIST:
	case EINVAL:
	case EMFILE:
	case ENOENT:
	case ENOEXEC:
	case ENOMEM:
	case ENOSPC:
	case ERANGE:
	case EXDEV:
	}
	*/
}

