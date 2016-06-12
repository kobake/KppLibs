#pragma once

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         SDK�w��                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500 //Windows 2000
#endif

/*
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0500 //Windows 2000
#endif
*/

//_WIN32_WINNT��`�̋���
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
//                        ��Ȃ��x��                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//�x��C4715(�l��Ԃ��Ȃ��R���g���[���p�X������܂�)���G���[�Ƃ��Ĉ���
#pragma warning(error:4715)

//�x��C4717(���ׂẴR���g���[���̃p�X�A�֐�����A����ƃ����^�C�� �X�^�b�N �I�[�o�[�t���[���������܂��B)���G���[�Ƃ��Ĉ���
#pragma warning(error:4717)

//�x��C4150('XXX' �^���폜���邽�� delete ���Z�q���Ăяo����܂������A��`������܂���B)���G���[�Ƃ��Ĉ���
#pragma warning(error:4150)

//�x��C4552(���Z�q�Ƀv���O������̍�p������܂���B��p�������Z�q���g�p���Ă�������)���G���[�Ƃ��Ĉ���
#pragma warning(error:4552)

//�x��C4005(�}�N�����Ē�`����܂���)���G���[�Ƃ��Ĉ���
#pragma warning(error:4005)


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       ��������x��                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//�x��C4996('strcpy' ���Â��`���Ƃ��Đ錾����܂����B�c��)�𖳎�����
#pragma warning(disable:4996)
//#define _CRT_SECURE_NO_DEPRECATE

//�x��C4786(���ʎq����������)�𖳎����� (STL���g���Ƃ��̌x�����悭�o��)
#pragma warning(disable:4786)

//�x��C4094(�^�O�̂Ȃ� 'class' ���V���{���Ȃ��Ő錾����Ă��܂��B)�𖳎�����B(�}�N���p)
#pragma warning(disable:4094)


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �萔                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

#ifndef NULL
#define NULL 0
#endif


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                            �^                               //
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
//                          �}�N��                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

#define m_delete(p)  do{ if(p)delete (p);   (p)=0; }while(0)
#define m_delete2(p) do{ if(p)delete[] (p); (p)=0; }while(0)
#define m_free(p)    do{ if(p)free(p);      (p)=0; }while(0)

//�z��̗v�f�����擾����}�N��
#define countof(a) (sizeof(a)/sizeof((a)[0]))

//wchar_t
#define __LTEXT(A) L##A
#define LTEXT(A) __LTEXT(A)

//char
#define ATEXT(A) A

//swap
#define m_swap(n1,n2,tmp) do{ tmp=n1; n1=n2; n2=tmp; }while(0)


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          ���̑�                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
#include "../design/DefOrClass.h"


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          �����N                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")		//DragQueryFile
#pragma comment(lib, "Gdi32.lib")		//StretchDIBits
#pragma comment(lib, "advapi32.lib")	//���W�X�g���n
#pragma comment(lib, "winmm.lib")


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       �悭�g���@�\                          //
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
