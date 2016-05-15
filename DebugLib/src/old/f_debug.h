#ifdef _DEBUG

void debugprintf(const wchar *format,...);

const wchar *debugGetMessageName(UINT msg);
const wchar *debugGetNotifyName(UINT msg);

const wchar *debugGetTreeViewMessage(UINT msg);
const wchar *debugGetTreeViewNotify(UINT msg);

const wchar *debugGetMixerComponentType(DWORD dwComponentType);
const wchar *debugGetMixerControlType(DWORD dwControlType);

const wchar *debugGetErrorName(int err);

#else

void debugprintf_dummy(const wchar *format,...);
#define debugprintf debugprintf_dummy

#define debugGetMessageName(msg)		""
#define debugGetNotifyName(msg)			""

#define debugGetTreeViewMessage(msg)	""
#define debugGetTreeViewNotify(msg)		""

#define debugGetMixerComponentType(t)	""
#define debugGetMixerControlType(t)		""

#define debugGetErrorName(err)			""

#endif
