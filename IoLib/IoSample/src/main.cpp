#include "common/_required.h"
#include "common/sys.h"
#include <IoLib.h>
#include <windows.h>

int wmain(int argc, wchar_t* argv[])
{
	c2lib::Init(argc, argv);
	debugInit(DEBUG_OUT_DEBUGGER);


	return 0;
}

