#pragma once

#ifdef _DEBUG
	#include <cstdlib>
	#include <new>
	#include <memory>
	#include <crtdbg.h>
	#define _CRTDBG_MAP_ALLOC
	#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
