#pragma once

#include <string.h>
#include "common/_required.h" // achar

inline int auto_strlen(const achar* str){ return (int)strlen(str); }
inline int auto_strlen(const wchar_t* str){ return (int)wcslen(str); }
