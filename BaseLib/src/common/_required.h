#pragma once

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         SDK指示                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500 //Windows 2000
#endif

/*
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0500 //Windows 2000
#endif
*/

//_WIN32_WINNT定義の強制
#ifndef _WIN32_WINNT
//#error _WIN32_WINNT must be 0x0500.
#endif

#if _WIN32_WINNT!=0x0500
//#error _WIN32_WINNT must be 0x0500.
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500 //SHGFI_ADDOVERLAYS
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        危ない警告                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//警告C4715(値を返さないコントロールパスがあります)をエラーとして扱う
#pragma warning(error:4715)

//警告C4717(すべてのコントロールのパス、関数を回帰するとランタイム スタック オーバーフローが発生します。)をエラーとして扱う
#pragma warning(error:4717)

//警告C4150('XXX' 型を削除するため delete 演算子が呼び出されましたが、定義がありません。)をエラーとして扱う
#pragma warning(error:4150)

//警告C4552(演算子にプログラム上の作用がありません。作用を持つ演算子を使用してください)をエラーとして扱う
#pragma warning(error:4552)

//警告C4005(マクロが再定義されました)をエラーとして扱う
#pragma warning(error:4005)


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       無視する警告                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//警告C4996('strcpy' が古い形式として宣言されました。…等)を無視する
#pragma warning(disable:4996)
//#define _CRT_SECURE_NO_DEPRECATE

//警告C4786(識別子が長すぎる)を無視する (STLを使うとこの警告がよく出る)
#pragma warning(disable:4786)

//警告C4094(タグのない 'class' がシンボルなしで宣言されています。)を無視する。(マクロ用)
#pragma warning(disable:4094)


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           定数                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

#ifndef NULL
#define NULL 0
#endif


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                            型                               //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
typedef unsigned int uint;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;
typedef signed char schar;
typedef float f32;
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

#include <tchar.h>
typedef TCHAR tchar;

typedef wchar_t wchar;
typedef char achar;

typedef char ACHAR;
typedef wchar_t WCHAR;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          マクロ                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

#define m_delete(p)  do{ if(p)delete (p);   (p)=0; }while(0)
#define m_delete2(p) do{ if(p)delete[] (p); (p)=0; }while(0)
#define m_free(p)    do{ if(p)free(p);      (p)=0; }while(0)

//配列の要素数を取得するマクロ
#define countof(a) (sizeof(a)/sizeof((a)[0]))

//wchar_t
#define __LTEXT(A) L##A
#define LTEXT(A) __LTEXT(A)

//char
#define ATEXT(A) A

//swap
#define m_swap(n1,n2,tmp) do{ tmp=n1; n1=n2; n2=tmp; }while(0)


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          その他                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
#include "../design/DefOrClass.h"


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          リンク                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")		//DragQueryFile
#pragma comment(lib, "Gdi32.lib")		//StretchDIBits
#pragma comment(lib, "advapi32.lib")	//レジストリ系
#pragma comment(lib, "winmm.lib")


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       よく使う機能                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
#include "../common/sys.h"
#include "../std/myexception.h"

#if _MSV_VER < 1400
	#define vsprintf_s _vsnprintf
	#define wcscpy_s(DST, COUNT, SRC) wcsncpy(DST, SRC, COUNT)
	#define vswprintf_s _vsnwprintf
	#define _swprintf swprintf
	#define swprintf_s _snwprintf
	#define wcscat_s(DST, COUNT, SRC) wcsncat(DST, SRC, COUNT)
	#define strcpy_s(DST, COUNT, SRC) strncpy(DST, SRC, COUNT)
	#ifndef _countof
		#define _countof(A) (sizeof(A) / sizeof(A[0]))
	#endif
#endif
