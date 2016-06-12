#include "mystring_funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <BaseLib.h>
#include "mywstring_funcs.h"
#include "wchar.h"

#include <vector>
#include <string>
//using namespace std;


//##########���Ƃŕʂ̃��C�u������ԂɈڊǗ\��
#include "../../ThreadLib/src/CCriticalSection.h"
class CRotationBuffer{
public:
	CRotationBuffer(size_t nBufferSize, int nBufferCount)
	: m_nBufferSize(nBufferSize)
	, m_nBufferCount(nBufferCount)
	{
		m_p = new wchar_t*[m_nBufferCount];
		for(int i=0;i<m_nBufferCount;i++){
			m_p[i] = new wchar_t[m_nBufferSize];
		}
		m_nCurrent = 0;
	}
	~CRotationBuffer()
	{
		for(int i=0;i<m_nBufferCount;i++){
			delete[] m_p[i];
		}
		delete[] m_p;
		m_p = NULL;
	}
	wchar_t* Get()
	{
		CRITICAL_ENTER(m_sync);

		m_nCurrent = (m_nCurrent+1) % m_nBufferCount;
		return m_p[m_nCurrent];
	}
private:
	size_t				m_nBufferSize;
	int					m_nBufferCount;
	wchar_t**			m_p;
	int					m_nCurrent;
	CCriticalSection	m_sync;
};

static CRotationBuffer g_buffers(
	512,	//�o�b�t�@�T�C�Y
	256		//�o�b�t�@��
);

wchar_t* tmp_buffer()
{
	return g_buffers.Get();
}

namespace util{



//#####���S�ł͂Ȃ�
void wcscat_c(wchar_t* pBuf, wchar_t c)
{
	wchar_t* p = wcschr(pBuf, L'\0');
	*p++=c;
	*p++=0;
}

class SafeBuffer{
public:
	SafeBuffer(wchar_t* _buf,size_t _count)
	: buf(_buf), count(_count)
	{
		buf[count-1]=L'\0';
		len=wcslen(buf);
	}
	//�C���^�[�t�F�[�X
	void slide(int off)
	{
		//�E�ɃX���C�h
		if(off>0){
			if(off<=(int)count-1){
				size_t new_len=len+off;
				//�͂ݏo�Ȃ�
				if(new_len<=count-1){
					memmove(buf+off,buf,(len+1)*sizeof(wchar_t));
					len=new_len;
				}
				//�͂ݏo��
				else{
					size_t move_len=count-off-1;
					memmove(buf+off,buf,move_len*sizeof(wchar_t));
					buf[count-1]=L'\0';
					len=count-1;
				}
			}
			else{
				//�S���Ȃ��Ȃ�̂ŁA�������Ȃ�
				buf[count-1]=L'\0';
				len=count-1;
			}
		}

		//���ɃX���C�h
		else if(off<0){
			off=tabs(off);
			if(off<=(int)count-1){
				int new_len=(int)len-off;
				//�����Ȃ�
				if(new_len>0){
					memmove(buf,buf+off,(new_len+1)*sizeof(wchar_t));
					len=new_len;
				}
				//������
				else{
					buf[0]='\0';
					len=0;
				}
			}
			else{
				//�S���Ȃ��Ȃ�̂ŁA�������Ȃ�
				buf[count-1]='\0';
				len=0;
			}
		}
	}
	//!���
	void Assign(const wchar_t* src)
	{
		size_t src_count=wcslen(src)+1;
		//�͂ݏo���Ȃ�
		if(src_count<=count){
			memmove(buf,src,src_count*sizeof(wchar_t));
			len=src_count-1;
		}
		//�͂ݏo��
		else{
			memmove(buf,src,(count-1)*sizeof(wchar_t)); buf[count-1]=L'\0';
			len=count-1;
		}
	}
	//!���������w�肵�đ�� (�r���܂ł����R�s�[���Ȃ��ꍇ�́A'\0'�͕t�����Ȃ�)
	void nassign(const wchar_t* src,size_t src_len)
	{
		//�R�s�[�� ����
		if(src_len>count-1)src_len=count-1;

		//�R�s�[
		memmove(buf,src,src_len*sizeof(wchar_t));

		//������ �g��
		if(src_len>len){ len=src_len; buf[len]=L'\0'; }
	}
	//!�I�[�ɑ}��
	void cat(const wchar_t* src)
	{
		size_t src_count=wcslen(src)+1;
		wchar_t* p=&buf[len];          //�ǉ��ꏊ
		int restcount=(int)count-(int)(p-buf); //buf�ɒǉ��ł���v�f��

		SafeBuffer rest(p,restcount);

		rest.Assign(src);
	}
	//!�擪�ɑ}��
	void rcat(const wchar_t* src)
	{
		int src_len=(int)wcslen(src);
		wchar_t* p=&buf[len];      //�ǉ��ꏊ
		int restlen=(int)count-1-(int)len; //str1�ɒǉ��ł��镶����

		//�X���C�h
		slide(src_len);

		//���
		nassign(src,src_len);
	}
private:
	wchar_t* buf;   //�o�b�t�@
	size_t count; //�v�f���B�Œ�ł�1�B
	size_t len;   //�������B�Œ�ł�0�B
};


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ���`                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//__int64�̍ő�l�F9,223,372,036,854,775,808 = 9 EBytes (�R���}�����25���A�R���}���ꂸ��19��)
//��unsigned __int64 �̏ꍇ�͂����1�������邱�Ƃɒ��ӁB

void itow_comma(wchar_t* pBuf, __int64 num, int step)
{
	_i64tow(num, pBuf, 10);
	if(*pBuf==L'-')pBuf++; //�}�C�i�X�L���̓R���}�������珜�O
	wchar_t* p = wcschr(pBuf, L'\0');
	int p_len = 0;
	while(1){
		p-=step;
		p_len+=step;
		if(p>=pBuf+1){
			memmove(p+1,p,(p_len+1)*sizeof(wchar_t));
			*p=L',';
			p_len++;
		}else{
			break;
		}
	}
}

wchar_t* tmp_itow_comma(__int64 num,int step)
{
	wchar_t* buf = g_buffers.Get();
	itow_comma(buf,num,step);
	return buf;
}

//�����ɃX�y�[�X��}�����邱�ƂŒ�������
void wcslpad(wchar_t* szBuf, int nLen)
{
	int nOldLen = (int)wcslen(szBuf);
	int nGap = nLen - nOldLen;
	if(nGap>0){
		//���ɃX�y�[�X��}��
		wmemmove(szBuf+nGap,szBuf,nOldLen+1);
		wchar_t* p = szBuf;
		for(int i=0;i<nGap;i++)*p++=L' ';
	}
	else{
		//��������ꍇ�͉������Ȃ�
	}
}

//�E���ɃX�y�[�X��}�����邱�ƂŒ�������
void wcsrpad(wchar_t* szBuf, int nLen)
{
	int nOldLen = (int)wcslen(szBuf);
	int nGap = nLen - nOldLen;
	if(nGap>0){
		//�E�ɃX�y�[�X��}��
		wchar_t* p = szBuf + nOldLen;
		for(int i=0;i<nGap;i++)*p++=L' ';
		*p++=L'\0';
	}
	else{
		//��������ꍇ�͉������Ȃ�
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ����                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//������q�ɂ͑Ή����Ȃ�
wchar_t* wcswcs_skipliteral(const wchar_t* str1,const wchar_t* str2,const wchar_t* brackets)
{
	int nbrackets=(int)wcslen(brackets);
	wchar_t* p=(wchar_t*)str1,*q; int str2len=(int)wcslen(str2);
	while(*p!=L'\0'){
		if(q=wcschr((wchar_t*)brackets,*p)){
			q=wcschr(p+1,*q);
			if(q==NULL){
				break;
			}else{
				p=q+1;
			}
		}else{
			if(wcsncmp(p,str2,str2len)==0){
				return p;
			}else{
				p++;
			}
		}
	}
	return NULL;
}

wchar_t* wcsriwcs(const wchar_t* str1,const wchar_t* str2,int begin)
{
	if(begin==-1)return wcsriwcs(str1,str2);
	int c=str2[0];
	int len_str2=(int)wcslen(str2);
	wchar_t* p=(wchar_t*)&str1[begin]-len_str2;
	while(p>=str1){
		if(wcsnicmp(p,str2,len_str2)==0)
			return p;
		p--;
	}
	return NULL;
}

wchar_t* wcsriwcs(const wchar_t* str1,const wchar_t* str2)
{
	int c=str2[0];
	int len_str2=(int)wcslen(str2);
	wchar_t* p=wcschr((wchar_t*)str1,L'\0')-len_str2;
	while(p>=str1){
		if(wcsnicmp(p,str2,len_str2)==0)
			return p;
		p--;
	}
	return NULL;
}

wchar_t* wcsiwcs(const wchar_t* str1,const wchar_t* str2)
{
	int c=str2[0];
	int len_str2=(int)wcslen(str2);
	wchar_t* p=(wchar_t*)str1;
	while(*p!=L'\0'){
		if(wcsnicmp(p,str2,len_str2)==0)
			return p;
		p++;
	}
	return NULL;
}

wchar_t* wcschrs(const wchar_t* str,int c1,int c2)
{
	const wchar_t* p=str;
	while(1){
		if(*p==c1)return (wchar_t*)p;
		if(*p==c2)return (wchar_t*)p;
		if(*p==L'\0')return NULL;
		p++;
	}
	return NULL;
}

//������Ȃ�������NULL��Ԃ�
wchar_t* wcschrs(const wchar_t* str,const wchar_t* chrs)
{
	const wchar_t* p=str;
	const wchar_t* q;
	while(*p){
		q=chrs;
		while(*q){
			if(*p==*q)return (wchar_t*)p;
			q++;
		}
		p++;
	}
	return NULL;
}

//������Ȃ�������I�[��Ԃ�
wchar_t* wcschrs2(const wchar_t* str,const wchar_t* chrs)
{
	const wchar_t* p=str;
	const wchar_t* q;
	while(*p){
		q=chrs;
		while(*q){
			if(*p==*q)return (wchar_t*)p;
			q++;
		}
		p++;
	}
	return (wchar_t*)p;
}

//! �Ō�����猟�����Ă����A��Ɍ������������̈ʒu��Ԃ�
wchar_t* wcsrchrs(const wchar_t* str,int c1,int c2)
{
	const wchar_t* p=wcschr(str,L'\0')-1;
	while(p>=str){
		if(*p==c1 || *p==c2)return const_cast<wchar_t*>(p);
		p--;
	}
	return NULL;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       ���S�R�s�[                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!str1��str2��]���B
/*!
@param str1       �]����o�b�t�@
@param str2       �]�����o�b�t�@
@param str1_count str1�̃o�b�t�@������(�I�[NULL���܂�)
*/
wchar_t* wcssafecpy(wchar_t* str1, const wchar_t* str2, size_t str1_count)
{
	SafeBuffer(str1,str1_count).Assign(str2);
	return str1;
}

//!str1�I�[��str2��]���B
/*!
@param str1       �]����o�b�t�@
@param str2       �]�����o�b�t�@
@param str1_count str1�̃o�b�t�@�v�f��(�I�[NULL���܂�)
*/
wchar_t* wcssafecat(wchar_t* str1, const wchar_t* str2, size_t str1_count)
{
	SafeBuffer(str1,str1_count).cat(str2);
	return str1;
}


//!str1�擪��str2��}���B
/*!
@param str1
@param str2
@param str1_count
*/
wchar_t* wcsrsafecat(wchar_t* str1, const wchar_t* str2, size_t str1_count)
{
	SafeBuffer(str1,str1_count).rcat(str2);
	return str1;
}


//!�R�s�[���������w�肵�āA�R�s�[
/*!
�r���}���͔�Ή�
�I�[��'\0'��t���邱�Ƃ�ۏ؂���
��ꂽ�㔼�͎̂Ă�
��copy_len �� wcslen(str2)���傫���Ă� wcslen(str2)�ȍ~�̃[���N���A�͍s��Ȃ�
@param str1
@param str2
@param copy_len
@param str1_count
*/
wchar_t* wcsnsafecpy(wchar_t* str1, const wchar_t* str2, size_t copy_len, size_t str1_count)
{
	str1[0]=L'\0';
	SafeBuffer b(str1,str1_count);
	b.nassign(str2,copy_len);
	return str1;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                            ����                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//�I�[�̉��s�����邾�����
wchar_t* chomp(wchar_t* wcs)
{
	wchar_t* p=wcschr(wcs, L'\0')-1;
	while(p>=wcs && *p==L'\n')*p--=L'\0';
	return wcs;
}

inline bool IS_EOL(wchar_t c){ return c==L'\n' || c==L'\r'; }

wstring& chomp(wstring& wcs)
{
	while(wcs.length()>0 && IS_EOL(wcs[wcs.length()-1]))wcs.resize(wcs.length()-1);
	return wcs;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                            ����                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

wchar_t* wcsndup(const wchar_t* str,size_t len)
{
	wchar_t* ret=(wchar_t*)malloc(sizeof(wchar_t)*(len+1));
	wcsncpy(ret,str,len); ret[len]=L'\0';
	return ret;
}

//new[]��
wchar_t* wcsdup_cpp(const wchar_t* str)
{
	int len=(int)wcslen(str);
	wchar_t* ret=new wchar_t[len+1];
	wcscpy(ret,str);
	return ret;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     �o�b�t�@�t���ϊ�                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

wchar_t* tmp_wcscat(const wchar_t* str1, const wchar_t* str2)
{
	wchar_t* tmp = g_buffers.Get();
	wcssafecpy(tmp, str1, countof(tmp));
	wcssafecat(tmp, str2, countof(tmp));
	return tmp;
}

std::wstring cpp_itow(int i)
{
	wchar_t buf[32];
	_itow(i, buf, 10);
	return buf;
}

wchar_t* tmp_itow(long long i)
{
	wchar_t* tmp = g_buffers.Get();
	_i64tow(i, tmp, 10);
	return tmp;
}

wchar_t* tmp_vswprintf(const wchar_t* format, va_list mark)
{
	wchar_t* buf = g_buffers.Get();
	vswprintf(buf, format, mark);
	return buf;
}

wchar_t* tmp_swprintf(const wchar_t* format, ...)
{
	va_list mark;
	va_start(mark, format);
	wchar_t* ret=tmp_vswprintf(format, mark);
	va_end(mark);
	return ret;
}

wchar_t* buf_vswprintf(wchar_t* buffer,const wchar_t* format, va_list mark)
{
	vswprintf(buffer, format, mark);
	return buffer;
}

wchar_t* buf_swprintf(wchar_t* buffer,const wchar_t* format, ...)
{
	va_list mark;
	va_start(mark, format);
	wchar_t* ret=buf_vswprintf(buffer, format, mark);
	va_end(mark);
	return ret;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      ���C���h�J�[�h                         //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//�Q�lURL
//http://www.hcn.zaq.ne.jp/no-ji/lib/fnamecmp.c

//!���C���h�J�[�h�Ƃ̈�v����
/*!
@param test    �Ώە�����
@param pattern ���C���h�J�[�h
@retval        ��v���Ă����true
*/
bool wildmatch(const wchar_t* test,const wchar_t* pattern)
{
//	printf("Filename_compare(%s %s)\n", test,pattern);

	const wchar_t* p=test;
	const wchar_t* q=pattern;

	//	�p�^�[���̍ŏ��̕����ɂ��ꍇ����
	switch( *q ) {
	case L'\0':
		// �p�^�[������̂Ƃ��A�t�@�C��������Ȃ�(TRUE)�B
		// �����łȂ��Ȃ�A(FALSE)��Ԃ��B
		return *p==L'\0';
	case L'*':
		// * �̂Ƃ��A *�ȍ~�̕�����ƁA���������ǂݔ�΂���
		// �����t�@�C�����Ŕ�r����B�����A����ȍ~����v���Ă���
		// �Ȃ�(TRUE)��Ԃ��B
		while(*p){
			if(wildmatch(p,q+1))return true;
			p++;
		}
		// �����ɗ����Ƃ��Atest�͂��ׂēǂݔ�΂��Ă���B
		// * �̌�ɕ������Ȃ���΁A������Ȃ̂�(TRUE)��Ԃ��B
		// * �̌�ɕ������c���Ă���΁A��v���Ȃ��������ƂɂȂ�A
		// (FALSE)��Ԃ��B
		return *(q+1)==L'\0';
	case L'?':
		// �I�[�ɒB���Ă���_��
		if(*p=='\0')return false;
		// ���ꂼ��P�������ǂݔ�΂��A��r����B
		return wildmatch(p+1,q+1);
	default:
		// ���C���h�J�[�h�ȊO�̒ʏ�̕����̂Ƃ��ɂ́A�啶����������
		// �������Ĕ�r����B����Ă����(FALSE)��Ԃ��B
		if(toupper(*p)!=toupper(*q))return false;
		// �����Ȃ�A���̕������r����B
		return wildmatch(p+1,q+1);
	}
	//	�����ɗ��邱�Ƃ͂Ȃ��B
	return false;
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         ���ݕϊ�                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

char* safe_wcstombs(
	const wchar_t*	src,
	char*			dst,
	size_t			dst_cnt
)
{
	wcstombs(dst,src,dst_cnt);
	dst[dst_cnt-1]='\0';
	return dst;
}

wchar_t* safe_mbstowcs(
	const char*	src,
	wchar_t*	dst,
	size_t		dst_cnt
)
{
	mbstowcs(dst,src,dst_cnt);
	dst[dst_cnt-1]=L'\0';
	return dst;
}

//���I�[��'\0'��t���邱�Ƃ�ۏ؂���
const char* tmp_wcstombs(const wchar_t* wcs)
{
	static char buf[1024];
	wcstombs(buf,wcs,countof(buf));
	buf[countof(buf)-1]='\0';
	return buf;
}

//���I�[��'\0'��t���邱�Ƃ�ۏ؂���
const wchar_t* tmp_mbstowcs(const char* str)
{
	static wchar_t buf[1024];
	mbstowcs(buf,str,countof(buf));
	buf[countof(buf)-1]=L'\0';
	return buf;
}

char* alloc_wcstombs(const wchar_t* wcs)
{
	size_t cnt=wcstombs(NULL,wcs,0);
	char* buf=(char*)malloc(cnt*sizeof(char));
	wcstombs(buf,wcs,cnt);
	return buf;
}

wchar_t* alloc_mbstowcs(const char* str)
{
	size_t cnt=mbstowcs(NULL,str,0);
	wchar_t* buf=(wchar_t*)malloc(cnt*sizeof(wchar_t));
	mbstowcs(buf,str,cnt);
	return buf;
}

std::string cpp_wcstombs(const std::wstring& wcs, UINT uiCodePage)
{
	if(wcs.length()==0)return "";
	//�o�b�t�@���Z�o
	int cnt = ::WideCharToMultiByte(
		uiCodePage,
		0,
		wcs.c_str(),
		(int)wcs.length(),
		NULL,
		0,
		NULL,
		NULL
	);
	//�o�b�t�@�m��
	std::vector<char> buf(cnt+1);
	//�ϊ�
	cnt = ::WideCharToMultiByte(
		uiCodePage,
		0,
		wcs.c_str(),
		(int)wcs.length(),
		&buf[0],
		cnt,
		NULL,
		NULL
	);
	buf[cnt] = 0;
	return &buf[0];
	/*
	size_t cnt=wcstombs(NULL,wcs.c_str(),0);
	vector<char> buf(cnt+1);
	wcstombs(&buf[0],wcs.c_str(),cnt);
	buf[cnt] = 0;
	return &buf[0];
	*/
}

std::wstring cpp_mbstowcs(const std::string& str, UINT uiCodePage)
{
	if(str.length()==0)return L"";
	//�o�b�t�@���Z�o
	int cnt = ::MultiByteToWideChar(
		uiCodePage,
		0,
		str.c_str(),
		(int)str.length(),
		NULL,
		0
	);
	//�o�b�t�@�m��
	std::vector<wchar_t> buf(cnt+1);
	//�ϊ�
	cnt = ::MultiByteToWideChar(
		uiCodePage,
		0,
		str.c_str(),
		(int)str.length(),
		&buf[0],
		cnt
	);
	buf[cnt] = 0;
	return &buf[0];
}

/*
std::wstring cpp_mbstowcs(const std::string& str)
{
	if(str.length()==0)return L"";
	//�o�b�t�@���Z�o
	size_t cnt=mbstowcs(NULL,str.c_str(),0);
	//�o�b�t�@�m��
	vector<wchar_t> buf(cnt+1);
	mbstowcs(&buf[0],str.c_str(),cnt);
	buf[cnt] = 0;
	return &buf[0];
}
*/

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         �ϊ��E�u��                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
/*
wchar_t* uppercpy(wchar_t* str1, const wchar_t* str2)
{
	wchar_t* p=str1;
	const wchar_t* q=str2;
	while(*q!='\0')*p++=toupper(*q++);
	*p++='\0';
	return str1;
}
*/
//����
vector<wstring> wcssplit(const wchar_t* str,wchar_t sep)
{
	vector<wstring> ret;
	const wchar_t* p=str;
	const wchar_t* q;
	int wcs_len=(int)wcslen(str);
	while(1){
		q=wcschr(p,sep);
		if(q==0)q=wcschr(p,L'\0');
		ret.push_back(wstring(p,q));
		if(*q==L'\0')break;
		p=q+1;
	}
	return ret;
}

vector<wstring> wcssplit(const wchar_t* str, const wchar_t* seps)
{
	vector<wstring> ret;
	const wchar_t* p=str;
	const wchar_t* q;
	int wcs_len=(int)wcslen(str);
	while(*p){
		q=wcschrs(p, seps);
		if(q==0)q=wcschr(p,L'\0');
		ret.push_back(wstring(p,q));
		if(*q==L'\0')break;
		p=q+1;
	}
	return ret;
}

vector<wstring> wcssplit_byword(const wchar_t* str, const wchar_t* sep)
{
	vector<wstring> ret;
	const wchar_t* p=str;
	const wchar_t* q;
	int wcs_len=(int)wcslen(str);
	while(*p){
		q=wcsstr(p,sep);
		if(q==0)q=wcschr(p,L'\0');
		ret.push_back(wstring(p,q));
		if(*q==L'\0')break;
		p=q+wcslen(sep);
	}
	return ret;
}

vector<wstring> wcssplit_emptyskip(const wchar_t* str, const wchar_t* seps)
{
	vector<wstring> ret;
	const wchar_t* p=str;
	const wchar_t* q;
	int wcs_len=(int)wcslen(str);
	while(*p){
		// �擪�󔒃X�L�b�v //
		while(*p && wcschr(seps, *p))p++;
		// �������o //
		q = p;
		while(*q && !wcschr(seps, *q))q++;
		// �v�f�o�^ //
		if(p != q){
			ret.push_back(wstring(p,q));
		}
		// �� //
		p = q;
	}
	return ret;
}

int wcsreplace(wchar_t* str,int from,int to)
{
	int ret=0;
	wchar_t* p=str;
	while((p=wcschr(p,from))!=NULL){
		*p++=to;
		ret++;
	}
	return ret;
}

//���o�b�t�@�T�C�Y�͋C�ɂ��Ȃ��̂Œ���
/*
int strreplace(wchar_t* str,const wchar_t* from,const wchar_t* to)
{
	int cnt=0;
	size_t nfrom=strlen(from);
	size_t nto=strlen(to);
	wchar_t* p=str;
	wchar_t* q0;
	wchar_t* q1;
	while(1){
		p=strstr(p,from);
		if(p){
			//�q�b�g�����Z
			cnt++;
			//�������ړ�
			q0=p+nfrom;
			q1=p+nto;
			memmove(q1,q0,strlen(q0)+1);
			//�u��
			memcpy(p,to,nto);
			//���̌����ʒu
			p=q1;
		}else{
			break;
		}
	}
	return cnt;
}
*/
int wcsreplace(wchar_t* str,const wchar_t* from,const wchar_t* to)
{
	int ret=0;
	wchar_t* p=str;
	int str_len=(int)wcslen(str);
	int from_len=(int)wcslen(from);
	int to_len=(int)wcslen(to);
	while(p=wcsstr(p,from)){
		wmemmove(p+to_len,p+from_len,str_len-(p+from_len-str)+1);
		str_len-=from_len-to_len;
		wmemmove(p,to,to_len);
		p+=to_len;
		ret++;
	}
	return ret;
}


std::wstring cpp_wcsreplace(const std::wstring& src,const wchar_t* from,const wchar_t* to)
{
	const wchar_t* p=src.c_str();
	int src_len=(int)src.length();
	int from_len=(int)wcslen(from);
	int to_len=(int)wcslen(to);
	std::wstring ret;
	while(1){
		const wchar_t* q=wcsstr(p,from);
		if(q){
			//p�`q��ret�ɒǉ�
			ret.append(p,q);
			//to��ret�ɒǉ�
			ret.append(to,to+to_len);
			//p��i�߂�
			p=q+from_len;
		}
		else{
			//p�`\0��ret�ɒǉ�
			ret.append(p);
			break;
		}
	}
	return ret;
}

std::wstring cpp_wcsreplace(const std::wstring& src, const std::map<std::wstring, std::wstring>& mapTable)
{
	const wchar_t* p=src.c_str();
	int src_len=(int)src.length();
	std::wstring ret;
	while(*p){
		int				nKey = 0;
		const wchar_t*	szKey = NULL;
		const wchar_t*	szValue = NULL;
		for(std::map<std::wstring, std::wstring>::const_iterator itr = mapTable.begin(); itr != mapTable.end(); itr++){
			if(wcsncmp(p, itr->first.c_str(), itr->first.length()) == 0){
				szKey = itr->first.c_str();
				szValue = itr->second.c_str();
				nKey = (int)itr->first.length();
				break;
			}
		}
		if(szKey){
			ret.append(szValue);
			p += nKey;
		}
		else{
			ret.append(1, *p++);
		}
	}
	return ret;
}

/*
int strsafereplace(wchar_t* str,const wchar_t* from,const wchar_t* to,int str_size)
{
	wchar_t* p=str;
	int str_len=strlen(str);
	int from_len=strlen(from);
	int to_len=strlen(to);
	int e;
	int ret=0;
	while(p=strstr(p,from)){
		ret++;
		if(from_len>=to_len){ //����
			memmove(p+to_len,p+from_len,str_len-(p+from_len-str)+1);
			str_len-=from_len-to_len;
			memmove(p,to,to_len);
			p+=to_len;
		}else{ //������
			e=0;
			if(str_len+to_len-from_len>str_size-1)e=str_len+to_len-from_len-(str_size-1);
			memmove(p+to_len,p+from_len,str_len-(p+from_len-str)+1-e);
			str_len+=to_len-from_len-e;
			if(p+to_len-str>str_size-1)e=p+to_len-str-(str_size-1);
			memmove(p,to,to_len-e);
			p+=to_len-e;
			if(p-str>=str_size)break;
		}
	}
	str[str_size-1]='\0';
	return ret;
}
*/


int wcsreplace_once(wchar_t* str,const wchar_t* from,const wchar_t* to)
{
	int ret=0;
	wchar_t* p=str;
	int str_len=(int)wcslen(str);
	int from_len=(int)wcslen(from);
	int to_len=(int)wcslen(to);
	if(p=wcsstr(p,from)){
		wmemmove(p+to_len,p+from_len,str_len-(p+from_len-str)+1);
		str_len-=from_len-to_len;
		wmemmove(p,to,to_len);
		p+=to_len;
		ret++;
	}
	return ret;
}

//!< ��������̉p��������ԑ啶���ɂ���
void wcsupper(wchar_t* wcs)
{
	wchar_t* p=wcs;
	while(*p){
		*p=::toupper(*p);
		p++;
	}
}

//! ��������̉p��������ԏ������ɂ���
void wcslower(wchar_t* wcs)
{
	wchar_t* p=wcs;
	while(*p){
		*p=::tolower(*p);
		p++;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      �����񁩁����l                         //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//���ʌ����𐔂���
static int count_keta(int n,int radix)
{
	int ret=0;
	while(n!=0){
		n/=radix;
		ret++;
	}
	return ret;
}

//�����̓���͖���`
int wtoi_radix(const wchar_t* str,int radix)
{
	const wchar_t* p=str;
	int ret=0;
	int i;
	while(*p){
		i=wctoi_radix(*p++,radix);
		if(i==-1)break;
		ret*=radix;
		ret+=i;
	}
	return ret;
}

//!�i�����������肵�āA�����񂩂琔�l�֕ϊ��B"0x�`"�c16�i, "0�`"�c8�i, "[1-9]�`"�c10�i
int wtoi_auto(const wchar_t* str)
{
	if(str[0]=='0'){
		if(str[1]=='x' || str[1]=='X'){
			return wtoi_radix(&str[2],16);
		}
		else{
			return wtoi_radix(&str[1],8);
		}
	}
	else{
		return wtoi_radix(str,10);
	}
	return wtoi_radix(str,10);
}

//
const wchar_t* itow_radix(int n,int radix,int ketapad)
{
	wchar_t* buf = g_buffers.Get();
	//�������ׂ�
	int keta=count_keta(n,radix);
	if(keta==0)keta=1;
	if(keta<ketapad)keta=ketapad;
	//�ϊ�
	int k;
	wchar_t* p=&buf[keta];
	*p--='\0';
	for(int i=0;i<keta;i++){
		k=n%radix;
		*p--=itowc_radix(k,radix);
		n/=radix;
	}
	return buf;
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                            �p�X                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
bool ext_check(const wchar_t* path,const wchar_t* ext)
{
	const wchar_t* p=wcschr(path,L'\0')-wcslen(ext);
	if(p<path)return false;
	if(wcsicmp(p,ext)!=0)return false; //�啶������������ʂ��Ȃ�
	return true;
}

void remove_ext(wchar_t* path,bool repeat)
{
	//���ׂĂ̊g���q������
	if(repeat){
		wchar_t* p=wcschr(path,L'.');
		if(p)*p=L'\0';
	}
	//�Ō�̊g���q�̂ݏ���
	else{
		wchar_t* p=wcsrchr(path,L'.');
		if(p)*p=L'\0';
	}
}

void remove_ftitle(wchar_t* path)
{
	wchar_t* p=wcsrchr(path,L'/');
	if(p){
		*(p+1)=L'\0';
	}
}

//! "..", "."��W�J
/*
	�A���S���Y��
	"/../", "/..\0", "/./", "/.\0" ��T��
*/
void fextract(wchar_t* str)
{
	class CSlashStack{
	public:
		CSlashStack(){ m_ps[0]=m_ps[1]=NULL; }
		void Push(wchar_t* p){ m_ps[0]=m_ps[1]; m_ps[1]=p; }
		wchar_t* Get(int nIdx){ return m_ps[nIdx]; }
	private:
		wchar_t* m_ps[2];
	};

	int str_len=(int)wcslen(str);
	int state=0;
	wchar_t* p=str;
	CSlashStack slashes;
	while(1){
		wchar_t c=*p;
		switch(state){
		case 0:
			if(c==L'\0')goto end;
			else if(c==L'/' || c==L'\\'){ slashes.Push(p); state=1; }
			else state=0;

			p++;
			break;
		case 1:
			if(c==L'\0')goto end;
			else if(c==L'.')state=2;
			else{ state=0; p--; }

			p++;
			break;
		case 2:
			if(c==L'.')state=3;
			else if(c==L'/' || c==L'\\' || c==L'\0'){ state=5; p--; }
			else{ state=0; p--; }

			p++;
			break;
		case 3:
			if(c==L'/' || c==L'\\' || c==L'\0'){ state=4; p--; }
			else{ state=0; p--; }
			
			p++;
			break;
		case 4:
			//�u..�v�̓W�J
			{
				wchar_t* ps[2]={
					slashes.Get(0),
					slashes.Get(1)
				};

				if(*p!=L'\0'){
					if(ps[0]==NULL)ps[0]=str-1;
					wmemmove(ps[0]+1,p+1,str_len-(p-ps[0])+1);
					str_len-=(int)(p-ps[0]);

					p=ps[0]+1;
				}
				else{
					if(ps[0]==NULL){ state=0; continue; } //���ȑO�́u/�v�������ꍇ�́A�W�J�����s���Ȃ� (p��i�߂��ɁAstate0�̃t�F�[�Y��)
					*(ps[0])=L'\0';
					str_len-=(int)(p-ps[0]);

					p=ps[0];
				}

				state=0;
			}
			break;
		case 5:
			//�u.�v�̓W�J
			{
				wchar_t* q=slashes.Get(1);
				wmemmove(q,p,str_len-(p-q)+1); //���u+1�v�͏I�[�k����
				str_len-=(int)(p-q);

				p=q;
				state=0;
			}
			break;
		}
	}
end:
	;
}

wstring fextract2(const wchar_t* szPath, wchar_t cDelimiter)
{
	vector<wstring> items;
	//items�\�z
	const wchar_t* p = szPath;
	const wchar_t* begin = p;
	const wchar_t* end = p;
	while(1){
		if(*p==L'\0' || *p==cDelimiter){
			end = p;
			//�P��L�^
			wstring word(begin,end);
			if(word.compare(L"..")==0){
				if(items.size())items.pop_back();	//�|�b�v
			}
			else if(word.compare(L".")==0){
				//�������Ȃ�
			}
			else{
				items.push_back(word);				//�v�b�V��
			}
			//����
			if(*p==L'\0')break;
			p++;
			begin=p;
		}
		else{
			p++;
		}
	}
	//�Ō�̋󕶎���͏��� (��: abc/def/ghi// �� abc/def/ghi
	while(items.size() && items.back().length()==0){
		items.pop_back();
	}
	//�p�X������\�z
	wstring ret;
	for(int i=0;i<(int)items.size();i++){
		if(i>0)ret += cDelimiter;
		ret+=items[i];
	}
	return ret;
}


wchar_t* wcsltrim(wchar_t* str,const wchar_t* chs)
{
	wchar_t* p=str;
	while(*p){
		if(wcschr(chs,*p))p++;
		else break;
	}
	if(p>str){
		wmemmove(str,p,wcslen(p)+1);
	}
	return str;
}

wchar_t* wcsrtrim(wchar_t* str,const wchar_t* chs)
{
	wchar_t* p=wcschr(str,L'\0')-1;
	while(p>=str){
		if(wcschr(chs,*p))p--;
		else break;
	}
	p++;
	*p=L'\0';
	return str;
}

wchar_t* wcstrim(wchar_t* str,const wchar_t* chs)
{
	wcsrtrim(str,chs);
	wcsltrim(str,chs);
	return str;
}

std::wstring cpp_wcstrim(const wchar_t* str, const wchar_t* chs)
{
	std::vector<wchar_t> v(wcslen(str)+1);
	wcscpy(&v[0], str);
	wcstrim(&v[0], chs);
	return &v[0];
}


} //namespace util
