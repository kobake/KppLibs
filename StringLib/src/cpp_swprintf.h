#pragma once

#include <string> //wstring
#include <stdlib.h> //va_list

std::wstring cpp_vswprintf(const wchar_t* format, va_list& v);
std::wstring cpp_swprintf(const wchar_t* format, ...);
