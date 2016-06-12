#pragma once

#include <BaseLib.h>
#include <string.h>

inline int auto_strlen(const achar* str){ return (int)strlen(str); }
inline int auto_strlen(const wchar_t* str){ return (int)wcslen(str); }
