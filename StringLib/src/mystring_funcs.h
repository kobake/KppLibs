#pragma once

#include <stdarg.h>
#include <string>
#include <vector>

#include "common/_required.h"

inline bool str_empty(const char* p)
{
	return p==NULL || *p=='\0';
}

namespace util{

//検索
char* strstr_skipliteral(const char* str1,const char* str2,const char* brackets);
char* strristr(const char* str1,const char* str2,int begin);
char* strristr(const char* str1,const char* str2);
char* stristr(const char* str1,const char* str2);
char* strchrs(const char* str,int c1,int c2);
char* strchrs(const char* str,const char* chrs);
char* strchrs2(const char* str,const char* chrs);

//安全コピー
char* strsafecpy(char* str1, const char* str2, int str1_len);
char* strsafecat(char* str1, const char* str2, int str1_size);
char* strrsafecat(char* str1, const char* str2, int str1_size);
char* strnsafecpy(char* str1, const char* str2, int copy_size, int str1_size);

//調整
char* chomp(char* str);
std::string cpp_chomp(const char* str);
char* strtrim(char* str,const char* chs);
char* strltrim(char* str,const char* chs);
char* strrtrim(char* str,const char* chs);
std::string strtrim(const char* str, const char* chs);
int strlpad(char* str, int size);
int strrpad(char* str, int size);

//複製
char* strndup(const char* str, int size);
char* strdup_cpp(const char* str);

//バッファ付き変換
char* tmp_itoa(int i);
char* tmp_vsprintf(const char* format, va_list mark);
char* tmp_sprintf(const char* format, ...);
char* buf_vsprintf(char* buf,const char* format, va_list mark);
char* buf_sprintf(char* buf,const char* format, ...);
char* tmp_strcat(const char* str1, const char* str2);
char* tmp_itoa_comma(int num,int step);

//変換・置換
char* uppercpy(char* str1, const char* str2);
std::vector<std::string> strsplit(const char* str,char sep);
int strreplace(char* str,int from,int to);
int strreplace(char* str,const char* from,const char* to);
int strreplace(char* str,const char* from,const char* to);
int strsafereplace(char* str,const char* from,const char* to,int str_size);
int strreplace_once(char* str,const char* from,const char* to);
std::string cpp_strreplace(const std::string& src,const char* from,const char* to);

//文字列←→数値
int atoi_radix(const char* str,int radix);
const char* itoa_radix(int n,int radix,int ketapad=1);
inline int atoi2(const char* str){ return atoi_radix(str,2); }
inline int atoi4(const char* str){ return atoi_radix(str,4); }
inline int atoi8(const char* str){ return atoi_radix(str,8); }
inline int atoi16(const char* str){ return atoi_radix(str,16); }
inline int atoi10(const char* str){ return atoi_radix(str,10); }
inline const char* itoa2(int n,int ketapad=1){ return itoa_radix(n,2,ketapad); }
inline const char* itoa4(int n,int ketapad=1){ return itoa_radix(n,4,ketapad); }
inline const char* itoa8(int n,int ketapad=1){ return itoa_radix(n,8,ketapad); }
inline const char* itoa16(int n,int ketapad=1){ return itoa_radix(n,16,ketapad); }
inline const char* itoa10(int n,int ketapad=1){ return itoa_radix(n,10,ketapad); }

//パス
bool ext_check(const char* path,const char* ext);
void remove_ext(char* path,bool repeat=false);
void remove_ftitle(char* path);
void fextract(char* str);

//トークン
int get_token(char* str,char** token,int maxtoken);
int alloc_token(const char* str,char** token,int maxtoken,const char* delimiters,const char* brackets);

//日本語処理
inline bool KANJI1(unsigned char c){ return c>=0x81 && c<=0x9F || c>=0xE0 && c<=0xFC; }
inline bool KANJI2(unsigned char c){ return c>=0x40 && c<=0xFC && c!=0x7F; }
char* make_kanji(const char* str,char* kanji=0);
char* kanji_strchr(const char* str,char* kanji,int c);
char* kanji_strrchr(const char* str,char* kanji,int c);

//ワイルドカード
bool wildmatch(const char* test,const char* pattern);

// URLエンコード //
std::string urlencode(const std::string& str);
std::string urldecode(const std::string& str);

// SHA-1ハッシュ //
std::string sha1(const void* pData, int iLen);

} //namespace util

using namespace util;


#include "md5.h"
