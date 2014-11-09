#pragma once

inline bool wstr_empty(const wchar_t* p)
{
	return p==NULL || *p==L'\0';
}

wchar_t* tmp_buffer();

#include <vector>
#include <string>
#include <map>
#include <winsock2.h> //windows.h���O�ɂȂ��ƃ_��
#include <windows.h> //UINT
#include "util/std.h" //tmin

#ifndef _countof
#define _countof(A) (sizeof(A) / sizeof(A[0]))
#endif

//std::wstring��͕킵���Œ�e�ʂ̕�����N���X
template <int SIZE>
class static_string{
private:
	typedef static_string<SIZE> Me;
public:
	//������
	static_string(){ m_buf[0] = 0; m_len = 0; }
	//�R�s�[
	static_string(const wchar_t* rhs){ assign(rhs); }
	static_string(const Me& rhs){ assign(rhs); }
	Me& operator = (const wchar_t* rhs){ assign(rhs); return *this; }
	Me& operator = (const Me& rhs){ assign(rhs); return *this; }
	//�ݒ�
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
	//�擾
	const wchar_t*	c_str() const{ return m_buf; }
	size_t			length() const{ return m_len; }
private:
	wchar_t m_buf[SIZE];
	size_t	m_len;
};


namespace util{

// -- -- �A�� -- -- //
void wcscat_c(wchar_t* pBuf, wchar_t c);

// -- -- ���` -- -- //
//�R���}
void itow_comma(wchar_t* pBuf, __int64 num, int step);
wchar_t* tmp_itow_comma(__int64 num,int step);

//��������
void wcslpad(wchar_t* szBuf, int nLen); //�����ɃX�y�[�X��}�����邱�ƂŒ�������
void wcsrpad(wchar_t* szBuf, int nLen); //�E���ɃX�y�[�X��}�����邱�ƂŒ�������

// -- -- ���� -- -- //
wchar_t* wcswcs_skipliteral(const wchar_t* str1,const wchar_t* str2,const wchar_t* brackets);
wchar_t* wcsriwcs(const wchar_t* str1,const wchar_t* str2,int begin);
wchar_t* wcsriwcs(const wchar_t* str1,const wchar_t* str2);
wchar_t* wcsiwcs(const wchar_t* str1,const wchar_t* str2);
wchar_t* wcschrs(const wchar_t* str,int c1,int c2); //!< �擪���猟�����Ă����A��Ɍ������������̈ʒu��Ԃ�
wchar_t* wcschrs(const wchar_t* str,const wchar_t* chrs);
wchar_t* wcschrs2(const wchar_t* str,const wchar_t* chrs);
wchar_t* wcsrchrs(const wchar_t* str,int c1,int c2); //!< �Ō�����猟�����Ă����A��Ɍ������������̈ʒu��Ԃ�

//���S�R�s�[
wchar_t* wcssafecpy(wchar_t* str1, const wchar_t* str2, size_t str1_count);
wchar_t* wcssafecat(wchar_t* str1, const wchar_t* str2, size_t str1_count);
wchar_t* wcsrsafecat(wchar_t* str1, const wchar_t* str2, size_t str1_count);
wchar_t* wcsnsafecpy(wchar_t* str1, const wchar_t* str2, size_t copy_len, size_t str1_count);

//����
wchar_t* chomp(wchar_t* wcs);
std::wstring& chomp(std::wstring& wcs);

//����
wchar_t* wcsndup(const wchar_t* str, size_t len);
wchar_t* wcsdup_cpp(const wchar_t* str);

//�o�b�t�@�t���ϊ�
std::wstring cpp_itow(int i);
wchar_t* tmp_itow(long long i);
wchar_t* tmp_vswprintf(const wchar_t* format, va_list mark);
wchar_t* tmp_swprintf(const wchar_t* format, ...);
wchar_t* buf_vswprintf(wchar_t* buf,const wchar_t* format, va_list mark);
wchar_t* buf_swprintf(wchar_t* buf,const wchar_t* format, ...);
wchar_t* tmp_wcscat(const wchar_t* str1, const wchar_t* str2);

//���C���h�J�[�h
bool wildmatch(const wchar_t* test,const wchar_t* pattern); //!<���C���h�J�[�h�Ƃ̈�v����

//���C�h���������V���O���o�C�g�̑��ݕϊ�
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



//�ϊ��E�u��
//char* uppercpy(wchar_t* str1, const wchar_t* str2);
std::vector<std::wstring> wcssplit(const wchar_t* str, wchar_t sep);
std::vector<std::wstring> wcssplit(const wchar_t* str, const wchar_t* seps);
std::vector<std::wstring> wcssplit_byword(const wchar_t* str, const wchar_t* sep);
std::vector<std::wstring> wcssplit_emptyskip(const wchar_t* str, const wchar_t* seps);
int wcsreplace(wchar_t* str,int from,int to); //!< ��������̕��������ׂĒu������B�߂�l�͒u�������������B
//int strreplace(wchar_t* str,const wchar_t* from,const wchar_t* to);
int wcsreplace(wchar_t* str,const wchar_t* from,const wchar_t* to);
std::wstring cpp_wcsreplace(const std::wstring& src,const wchar_t* from,const wchar_t* to);
std::wstring cpp_wcsreplace(const std::wstring& src, const std::map<std::wstring, std::wstring>& mapTable);
//int strsafereplace(wchar_t* str,const wchar_t* from,const wchar_t* to,int str_size);
int wcsreplace_once(wchar_t* str,const wchar_t* from,const wchar_t* to);
void wcsupper(wchar_t* wcs); //!< ��������̉p��������ԑ啶���ɂ���
void wcslower(wchar_t* wcs); //!< ��������̉p��������ԏ������ɂ���

// ���� //
std::wstring cpp_wcstrim(const wchar_t* str, const wchar_t* chs);
wchar_t* strtrim(wchar_t* str,const wchar_t* chs);


//�����񁨐��l
int wtoi_radix(const wchar_t* str,int radix); //!<�i�����w�肵�āA�����񂩂琔�l�֕ϊ��B
int wtoi_auto(const wchar_t* str);            //!<�i�����������肵�āA�����񂩂琔�l�֕ϊ��B"0x�`"�c16�i, "0�`"�c8�i, "[1-9]�`"�c10�i
inline int wtoi2(const wchar_t* str){ return wtoi_radix(str,2); }
inline int wtoi4(const wchar_t* str){ return wtoi_radix(str,4); }
inline int wtoi8(const wchar_t* str){ return wtoi_radix(str,8); }
inline int wtoi16(const wchar_t* str){ return wtoi_radix(str,16); }
inline int wtoi10(const wchar_t* str){ return wtoi_radix(str,10); }

#if _MSC_VER >= 1400 //VS2005�ȍ~
inline const wchar_t* tmp_ftow(double d)
{
	static wchar_t buf[32];
	swprintf_s(buf, _countof(buf), L"%.2lf", d);
	return buf;
}
#endif


//�p�X
bool ext_check(const wchar_t* path,const wchar_t* ext);
void remove_ext(wchar_t* path,bool repeat=false);
void remove_ftitle(wchar_t* path);
void fextract(wchar_t* str);
std::wstring fextract2(const wchar_t* szPath, wchar_t cDelimiter);

//���l��������
const wchar_t* itow_radix(int n,int radix,int ketapad=1); //!<�i�����w�肵�āA���l���當����֕ϊ��B
inline const wchar_t* itow2(int n,int ketapad=1){ return itow_radix(n,2,ketapad); }
inline const wchar_t* itow4(int n,int ketapad=1){ return itow_radix(n,4,ketapad); }
inline const wchar_t* itow8(int n,int ketapad=1){ return itow_radix(n,8,ketapad); }
inline const wchar_t* itow16(int n,int ketapad=1){ return itow_radix(n,16,ketapad); }
inline const wchar_t* itow10(int n,int ketapad=1){ return itow_radix(n,10,ketapad); }


} //namespace util

using namespace util;
