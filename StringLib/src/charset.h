#pragma once

#include "common/_required.h" // uchar

//---------------------------------------------------------------------------
// SJIS2UTF8N()
//! NULL終端のSJISの文字列を、NULL終端のUTF8Nに変換する
/*!
引用元 http://homepage1.nifty.com/yt-works/prog/utf8/utf8.html

\param	src	[i]		変換元のSJIS文字列ポインタ
\param	dst	[i/o]	変換後のUTF8Nを返す先のポインタ
\param	dst_size	[i]		wdstの上限サイズ。この値-1byteまで記録できる。
						0を指定した場合、dstは無視され、NULL終端込みの必要な容量が返る
\retval	1以上	変換後のバイト数
\retval	0		エラー
\retval	-1		作業メモリの確保失敗
*/
//---------------------------------------------------------------------------
int SJIStoUTF8N(const char* src,char* dst,int dst_size);

//---------------------------------------------------------------------------
// UTF8N2SJIS()
//! NULL終端のUTF8N文字列をSJISへ変換する
/*!
引用元 http://homepage1.nifty.com/yt-works/prog/utf8/utf8.html

\param	src	[i]		変換元のUTF8N文字列ポインタ
\param	dst	[i/o]	変換後のSJISを返す先のポインタ
\param	dst_size	[i]		wdstの上限サイズ。この値-1byteまで記録できる
						0を指定した場合、dstは無視され、NULL終端込みの必要な容量が返る
\retval	0		エラー
\retval	1以上	変換後のバイト数
\retval	-1		作業メモリの確保失敗
*/
//---------------------------------------------------------------------------
int UTF8NtoSJIS(const char* src,char* dst,int dst_size);


#include <windows.h>
#include <string>

//!文字コード変換クラス
/*!
引用元 http://kamoland.com/wiki/wiki.cgi?ShiftJIS%A1%A4Unicode(UTF-8)
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


//その他の変換関数
void sjis_to_jis(const uchar* src,uchar* dst);
void jis_to_sjis(const uchar* src,uchar* dst);

//自前関数
std::wstring utf8_to_utf16(const char* src);
void utf8_to_utf16(const char* src,wchar_t* dst,size_t dst_count);
