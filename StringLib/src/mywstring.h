#pragma once

inline bool wstr_empty(const wchar_t* p)
{
	return p==NULL || *p==L'\0';
}

wchar_t* tmp_buffer();

#include <vector>
#include <string>
#include <map>
#include <winsock2.h> //windows.hより前にないとダメ
#include <windows.h> //UINT
#include "util/std.h" //tmin

#ifndef _countof
#define _countof(A) (sizeof(A) / sizeof(A[0]))
#endif

//std::wstringを模倣した固定容量の文字列クラス
template <int SIZE>
class static_string{
private:
	typedef static_string<SIZE> Me;
public:
	//初期化
	static_string(){ m_buf[0] = 0; m_len = 0; }
	//コピー
	static_string(const wchar_t* rhs){ assign(rhs); }
	static_string(const Me& rhs){ assign(rhs); }
	Me& operator = (const wchar_t* rhs){ assign(rhs); return *this; }
	Me& operator = (const Me& rhs){ assign(rhs); return *this; }
	//設定
	void assign(const wchar_t* rhs)
	{
		int rhs_len = (int)wcslen(rhs);
		int copy_len = tmin<int>(rhs_len, SIZE-1);
		wmemcpy(m_buf, rhs, copy_len);
		m_buf[copy_len] = 0;
		m_len = copy_len;
	}
	void assign(const Me& rhs)
	{
		if(this==&rhs)return;
		wmemcpy(m_buf, rhs.m_buf, rhs.m_len+1);
		m_len = rhs.m_len;
	}
	//取得
	const wchar_t*	c_str() const{ return m_buf; }
	size_t			length() const{ return m_len; }
private:
	wchar_t m_buf[SIZE];
	size_t	m_len;
};


namespace util{

// -- -- 連結 -- -- //
void wcscat_c(wchar_t* pBuf, wchar_t c);

// -- -- 整形 -- -- //
//コンマ
void itow_comma(wchar_t* pBuf, __int64 num, int step);
wchar_t* tmp_itow_comma(__int64 num,int step);

//長さ調整
void wcslpad(wchar_t* szBuf, int nLen); //左側にスペースを挿入することで長さ調整
void wcsrpad(wchar_t* szBuf, int nLen); //右側にスペースを挿入することで長さ調整

// -- -- 検索 -- -- //
wchar_t* wcswcs_skipliteral(const wchar_t* str1,const wchar_t* str2,const wchar_t* brackets);
wchar_t* wcsriwcs(const wchar_t* str1,const wchar_t* str2,int begin);
wchar_t* wcsriwcs(const wchar_t* str1,const wchar_t* str2);
wchar_t* wcsiwcs(const wchar_t* str1,const wchar_t* str2);
wchar_t* wcschrs(const wchar_t* str,int c1,int c2); //!< 先頭から検索していき、先に見つかった文字の位置を返す
wchar_t* wcschrs(const wchar_t* str,const wchar_t* chrs);
wchar_t* wcschrs2(const wchar_t* str,const wchar_t* chrs);
wchar_t* wcsrchrs(const wchar_t* str,int c1,int c2); //!< 最後尾から検索していき、先に見つかった文字の位置を返す

//安全コピー
wchar_t* wcssafecpy(wchar_t* str1, const wchar_t* str2, size_t str1_count);
wchar_t* wcssafecat(wchar_t* str1, const wchar_t* str2, size_t str1_count);
wchar_t* wcsrsafecat(wchar_t* str1, const wchar_t* str2, size_t str1_count);
wchar_t* wcsnsafecpy(wchar_t* str1, const wchar_t* str2, size_t copy_len, size_t str1_count);

//調整
wchar_t* chomp(wchar_t* wcs);
std::wstring& chomp(std::wstring& wcs);

//複製
wchar_t* wcsndup(const wchar_t* str, size_t len);
wchar_t* wcsdup_cpp(const wchar_t* str);

//バッファ付き変換
std::wstring cpp_itow(int i);
wchar_t* tmp_itow(long long i);
wchar_t* tmp_vswprintf(const wchar_t* format, va_list mark);
wchar_t* tmp_swprintf(const wchar_t* format, ...);
wchar_t* buf_vswprintf(wchar_t* buf,const wchar_t* format, va_list mark);
wchar_t* buf_swprintf(wchar_t* buf,const wchar_t* format, ...);
wchar_t* tmp_wcscat(const wchar_t* str1, const wchar_t* str2);

//ワイルドカード
bool wildmatch(const wchar_t* test,const wchar_t* pattern); //!<ワイルドカードとの一致検証

//ワイド文字←→シングルバイトの相互変換
char* safe_wcstombs(
	const wchar_t*	src,
	char*			dst,
	size_t			dst_cnt
);
wchar_t* safe_mbstowcs(
	const char*	src,
	wchar_t*	dst,
	size_t		dst_cnt
);
const char* tmp_wcstombs(const wchar_t* wcs);
const wchar_t* tmp_mbstowcs(const char* str);
char* alloc_wcstombs(const wchar_t* wcs);
wchar_t* alloc_mbstowcs(const char* str);
std::string cpp_wcstombs(const std::wstring& wcs, UINT uiCodePage = CP_ACP);
std::wstring cpp_mbstowcs(const std::string& str, UINT uiCodePage = CP_ACP);



//変換・置換
//char* uppercpy(wchar_t* str1, const wchar_t* str2);
std::vector<std::wstring> wcssplit(const wchar_t* str, wchar_t sep);
std::vector<std::wstring> wcssplit(const wchar_t* str, const wchar_t* seps);
std::vector<std::wstring> wcssplit_byword(const wchar_t* str, const wchar_t* sep);
std::vector<std::wstring> wcssplit_emptyskip(const wchar_t* str, const wchar_t* seps);
int wcsreplace(wchar_t* str,int from,int to); //!< 文字列内の文字をすべて置換する。戻り値は置換した文字数。
//int strreplace(wchar_t* str,const wchar_t* from,const wchar_t* to);
int wcsreplace(wchar_t* str,const wchar_t* from,const wchar_t* to);
std::wstring cpp_wcsreplace(const std::wstring& src,const wchar_t* from,const wchar_t* to);
std::wstring cpp_wcsreplace(const std::wstring& src, const std::map<std::wstring, std::wstring>& mapTable);
//int strsafereplace(wchar_t* str,const wchar_t* from,const wchar_t* to,int str_size);
int wcsreplace_once(wchar_t* str,const wchar_t* from,const wchar_t* to);
void wcsupper(wchar_t* wcs); //!< 文字列内の英字をぜんぶ大文字にする
void wcslower(wchar_t* wcs); //!< 文字列内の英字をぜんぶ小文字にする

// 調整 //
std::wstring cpp_wcstrim(const wchar_t* str, const wchar_t* chs);
wchar_t* strtrim(wchar_t* str,const wchar_t* chs);


//文字列→数値
int wtoi_radix(const wchar_t* str,int radix); //!<進数を指定して、文字列から数値へ変換。
int wtoi_auto(const wchar_t* str);            //!<進数を自動判定して、文字列から数値へ変換。"0x～"…16進, "0～"…8進, "[1-9]～"…10進
inline int wtoi2(const wchar_t* str){ return wtoi_radix(str,2); }
inline int wtoi4(const wchar_t* str){ return wtoi_radix(str,4); }
inline int wtoi8(const wchar_t* str){ return wtoi_radix(str,8); }
inline int wtoi16(const wchar_t* str){ return wtoi_radix(str,16); }
inline int wtoi10(const wchar_t* str){ return wtoi_radix(str,10); }

#if _MSC_VER >= 1400 //VS2005以降
inline const wchar_t* tmp_ftow(double d)
{
	static wchar_t buf[32];
	swprintf_s(buf, _countof(buf), L"%.2lf", d);
	return buf;
}
#endif


//パス
bool ext_check(const wchar_t* path,const wchar_t* ext);
void remove_ext(wchar_t* path,bool repeat=false);
void remove_ftitle(wchar_t* path);
void fextract(wchar_t* str);
std::wstring fextract2(const wchar_t* szPath, wchar_t cDelimiter);

//数値→文字列
const wchar_t* itow_radix(int n,int radix,int ketapad=1); //!<進数を指定して、数値から文字列へ変換。
inline const wchar_t* itow2(int n,int ketapad=1){ return itow_radix(n,2,ketapad); }
inline const wchar_t* itow4(int n,int ketapad=1){ return itow_radix(n,4,ketapad); }
inline const wchar_t* itow8(int n,int ketapad=1){ return itow_radix(n,8,ketapad); }
inline const wchar_t* itow16(int n,int ketapad=1){ return itow_radix(n,16,ketapad); }
inline const wchar_t* itow10(int n,int ketapad=1){ return itow_radix(n,10,ketapad); }


} //namespace util

using namespace util;
