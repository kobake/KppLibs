#pragma once

//#define WIN32_LEAN_AND_MEAN
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
//#include <winsock.h> //in_addr

#ifdef _WINSOCKAPI_
#undef _WINSOCKAPI_
#endif
#include <winsock2.h>
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
