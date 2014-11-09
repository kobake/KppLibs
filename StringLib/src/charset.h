#pragma once

#include "common/_required.h" // uchar

//---------------------------------------------------------------------------
// SJIS2UTF8N()
//! NULL�I�[��SJIS�̕�������ANULL�I�[��UTF8N�ɕϊ�����
/*!
���p�� http://homepage1.nifty.com/yt-works/prog/utf8/utf8.html

\param	src	[i]		�ϊ�����SJIS������|�C���^
\param	dst	[i/o]	�ϊ����UTF8N��Ԃ���̃|�C���^
\param	dst_size	[i]		wdst�̏���T�C�Y�B���̒l-1byte�܂ŋL�^�ł���B
						0���w�肵���ꍇ�Adst�͖�������ANULL�I�[���݂̕K�v�ȗe�ʂ��Ԃ�
\retval	1�ȏ�	�ϊ���̃o�C�g��
\retval	0		�G���[
\retval	-1		��ƃ������̊m�ێ��s
*/
//---------------------------------------------------------------------------
int SJIStoUTF8N(const char* src,char* dst,int dst_size);

//---------------------------------------------------------------------------
// UTF8N2SJIS()
//! NULL�I�[��UTF8N�������SJIS�֕ϊ�����
/*!
���p�� http://homepage1.nifty.com/yt-works/prog/utf8/utf8.html

\param	src	[i]		�ϊ�����UTF8N������|�C���^
\param	dst	[i/o]	�ϊ����SJIS��Ԃ���̃|�C���^
\param	dst_size	[i]		wdst�̏���T�C�Y�B���̒l-1byte�܂ŋL�^�ł���
						0���w�肵���ꍇ�Adst�͖�������ANULL�I�[���݂̕K�v�ȗe�ʂ��Ԃ�
\retval	0		�G���[
\retval	1�ȏ�	�ϊ���̃o�C�g��
\retval	-1		��ƃ������̊m�ێ��s
*/
//---------------------------------------------------------------------------
int UTF8NtoSJIS(const char* src,char* dst,int dst_size);


#include <windows.h>
#include <string>

//!�����R�[�h�ϊ��N���X
/*!
���p�� http://kamoland.com/wiki/wiki.cgi?ShiftJIS%A1%A4Unicode(UTF-8)
*/
class CUnicodeF {
public:
    static char* utf8_to_sjis(const char *pUtf8Str, int *nBytesOut);
    static char* sjis_to_utf8(const char *pAnsiStr, int *nBytesOut);
    static wchar_t* sjis_to_utf16be(const char *pAnsiStr, int *nBytesOut);
    static char* utf16be_to_utf8(const wchar_t *pUcsStr, int *nBytesOut);
    static char* utf16be_to_sjis(const wchar_t *pUcsStr, int *nBytesOut);
    static wchar_t* utf8_to_utf16be(const char *pUtf8Str, int *nNumOut, BOOL bBigEndian);

private:
    static int utf16be_to_utf8_sub( char *pUtf8, const wchar_t *pUcs2, int nUcsNum, BOOL bCountOnly);
    static int utf8_to_utf16be_sub( wchar_t *pUcs2, const char *pUtf8, int nUtf8Num, BOOL bCountOnly, BOOL bBigEndian);
};


//���̑��̕ϊ��֐�
void sjis_to_jis(const uchar* src,uchar* dst);
void jis_to_sjis(const uchar* src,uchar* dst);

//���O�֐�
std::wstring utf8_to_utf16(const char* src);
void utf8_to_utf16(const char* src,wchar_t* dst,size_t dst_count);
