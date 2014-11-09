#pragma once

#include <windows.h>

#ifdef _DEBUG
	void debugprintf(const wchar_t* format,...);
#else
	void debugprintf_dummy(const wchar_t* format,...);
	#define debugprintf debugprintf_dummy
#endif




const wchar_t* debugGetMessageName(UINT msg);
const wchar_t* debugGetNotifyName(UINT msg);

const wchar_t* debugGetTreeViewMessage(UINT msg);
const wchar_t* debugGetTreeViewNotify(UINT msg);

const wchar_t* debugGetMixerComponentType(DWORD dwComponentType);
const wchar_t* debugGetMixerControlType(DWORD dwControlType);

const wchar_t* debugGetErrorName(int err);
