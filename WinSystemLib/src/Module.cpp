#include "_required.h"
#include "Module.h"

Module::Module(HINSTANCE _hInst)
: hInst(_hInst)
{
	wchar_t buf[_MAX_PATH];
	GetModuleFileName(hInst,buf,_MAX_PATH-1);
	path.SetPath(buf);
}

