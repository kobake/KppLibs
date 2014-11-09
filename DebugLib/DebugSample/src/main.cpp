#include "common/_required.h"
#include "common/sys.h"
#include <DebugLib.h>
#include <windows.h>

void g()
{
	CStackTrace t;
	t.Show();
}

void f()
{
	g();
}

int wmain(int argc, wchar_t* argv[])
{
	c2lib::Init(argc, argv);
	debugInit(DEBUG_OUT_DEBUGGER);

	f();

	return 0;
}

