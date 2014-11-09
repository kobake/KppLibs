#include <c2lib.h>
#include <LogLib.h>
#include <DebugLib.h>

int wmain(int argc, wchar_t* argv[])
{
	c2lib::Init(argc, argv);
	CLogManager::Instance()->Init();
	_INFO.SetTarget(LOG_TARGET_STDOUT | LOG_TARGET_DEBUGGER);

	KLOG(_INFO, "hoge:%d %hs %ls", argc, "‚ ‚¢‚¤", L"‚©‚«‚­");
	KLOG(_INFO, "hoge:%d %hs %ls", argc, "‚ ‚¢‚¤", L"‚©‚«‚­");

	char buf[32];
	fgets(buf, _countof(buf), stdin);


	return 0;
}

