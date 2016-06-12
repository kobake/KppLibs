#include "mystring_funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <BaseLib.h>
#include "mystring_funcs.h"
#include "char.h"
#include <vector>
#include <string>
#include <boost/uuid/sha1.hpp>

using namespace std;

namespace util{


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           検索                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//※入れ子には対応しない
char* strstr_skipliteral(const char* str1,const char* str2,const char* brackets)
{
	int nbrackets=(int)strlen(brackets);
	char* p=(char*)str1,*q; int str2len=(int)strlen(str2);
	while(*p!='\0'){
		if(q=strchr((char*)brackets,*p)){
			q=strchr(p+1,*q);
			if(q==NULL){
				break;
			}else{
				p=q+1;
			}
		}else{
			if(strncmp(p,str2,str2len)==0){
				return p;
			}else{
				p++;
			}
		}
	}
	return NULL;
}

char* strristr(const char* str1,const char* str2,int begin)
{
	if(begin==-1)return strristr(str1,str2);
	int c=str2[0];
	int len_str2=(int)strlen(str2);
	char* p=(char*)&str1[begin]-len_str2;
	while(p>=str1){
		if(strnicmp(p,str2,len_str2)==0)
			return p;
		p--;
	}
	return NULL;
}

char* strristr(const char* str1,const char* str2)
{
	int c=str2[0];
	int len_str2=(int)strlen(str2);
	char* p=strchr((char*)str1,'\0')-len_str2;
	while(p>=str1){
		if(strnicmp(p,str2,len_str2)==0)
			return p;
		p--;
	}
	return NULL;
}

char* stristr(const char* str1,const char* str2)
{
	int c=str2[0];
	int len_str2=(int)strlen(str2);
	char* p=(char*)str1;
	while(*p!='\0'){
		if(strnicmp(p,str2,len_str2)==0)
			return p;
		p++;
	}
	return NULL;
}

char* strchrs(const char* str,int c1,int c2)
{
	const char* p=str;
	while(1){
		if(*p==c1)return (char*)p;
		if(*p==c2)return (char*)p;
		if(*p=='\0')return NULL;
		p++;
	}
	return NULL;
}

//見つからなかったらNULLを返す
char* strchrs(const char* str,const char* chrs)
{
	const char* p=str;
	const char* q;
	while(*p){
		q=chrs;
		while(*q){
			if(*p==*q)return (char*)p;
			q++;
		}
		p++;
	}
	return NULL;
}

//見つからなかったら終端を返す
char* strchrs2(const char* str,const char* chrs)
{
	const char* p=str;
	const char* q;
	while(*p){
		q=chrs;
		while(*q){
			if(*p==*q)return (char*)p;
			q++;
		}
		p++;
	}
	return (char*)p;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       安全コピー                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

char* strsafecpy(char* str1, const char* str2, int str1_size)
{
	int str2_size=(int)strlen(str2)+1;
	if(str2_size<=str1_size){
		memmove(str1, str2, str2_size);
	}else{
		memmove(str1, str2, str1_size-1); str1[str1_size-1]='\0';
	}
	return str1;
}

char* strsafecat(char* str1, const char* str2, int str1_size)
{
	int str2_size=(int)strlen(str2)+1;
	char* p=strchr(str1, '\0');
	int str1_rest=str1_size-(int)(p-str1);
	if(str2_size<=str1_rest){ //収まる
		memmove(p, str2, str2_size);
	}else{ //収まらない
		memmove(p, str2, str1_rest-1); p[str1_rest-1]='\0';
	}
	return str1;
}

char* strrsafecat(char* str1, const char* str2, int str1_size)
{
	int str2_len=(int)strlen(str2);
	char* p=strchr(str1, '\0');
	int str1_len=(int)(p-str1);
	int str1_rest=str1_size-1-str1_len;
	//収まる
	if(str2_len<=str1_rest){
		memmove(str1+str2_len,str1,str1_len+1); //スライド
		memmove(str1, str2, str2_len);
	}
	//収まらない(後ろを削る)
	else{
		int slide_len=str1_size-1-str2_len;
		//元のstr1の右端を削る
		if(slide_len>0){
			memmove(str1+str2_len,str1,slide_len+1); //スライド
			memmove(str1,str2,str2_len);
			str1[str1_size-1]='\0';
		}
		//元のstr1がすべてはみ出る場合
		else{
			memmove(str1,str2,str1_size-1);
			str1[str1_size-1]='\0';
		}
	}
	return str1;
}

//###copy_size が strlen(str2)より大きくても strlen(str2)以降のゼロクリアは行わない
char* strnsafecpy(char* str1, const char* str2, int copy_size, int str1_size)
{
	if(copy_size<str1_size){ //終端に'\0を付けることを保証する
		memmove(str1, str2, copy_size);
		str1[copy_size]='\0';
	}else{
		memmove(str1, str2, str1_size-1); //溢れた終端は捨てる
		str1[str1_size-1]='\0';
	}
	return str1;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                            調整                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//終端の改行を取れるだけ取る
char* chomp(char* str)
{
	char* p=strchr(str, '\0');
	while(p-1>=str && (*(p-1)=='\n' || *(p-1)=='\r'))p--;
	*p=0;
	return str;
}

std::string cpp_chomp(const char* str)
{
	vector<char> buf(str, str+strlen(str)+1);
	char* pStart = &buf[0];
	char* p = &buf[buf.size()-1];
	while(p-1>=pStart && (*(p-1)=='\r' || *(p-1)=='\n'))p--;
	if(p>=pStart)*p='\0';
	return &buf[0];
}

std::string strtrim(const char* str,const char* chs)
{
	std::vector<char> v(strlen(str)+1);
	strcpy(&v[0], str);
	strtrim(&v[0], chs);
	return &v[0];
}

char* strtrim(char* str,const char* chs)
{
	strrtrim(str,chs);
	strltrim(str,chs);
	return str;
}

char* strltrim(char* str,const char* chs)
{
	char* p=str;
	while(*p){
		if(strchr(chs,*p))p++;
		else break;
	}
	if(p>str){
		memmove(str,p,strlen(p)+1);
	}
	return str;
}

char* strrtrim(char* str,const char* chs)
{
	char* p=strchr(str,'\0')-1;
	while(p>=str){
		if(strchr(chs,*p))p--;
		else break;
	}
	p++;
	*p='\0';
	return str;
}

int strlpad(char* str, int length)
{
	int sp=length-(int)strlen(str);
	if(sp>0){
		memmove(str+sp, str, strlen(str)+1);
		char* p=str;
		for(int i=0;i<sp;i++){
			*p++=' ';
		}
	}
	return tmax(0, sp);
}

int strrpad(char* str, int length)
{
	int sp=length-(int)strlen(str);
	if(sp>0){
		char* p=strchr(str, '\0');
		for(int i=0;i<sp;i++){
			*p++=' ';
		}
		*p++='\0';
	}
	return tmax(0, sp);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                            複製                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

char* strndup(const char* str, int size)
{
	char* ret=(char*)malloc(size+1);
	strncpy(ret, str, size); ret[size]='\0';
	return ret;
}

//new[]版
char* strdup_cpp(const char* str)
{
	int n=(int)strlen(str);
	char* ret=new char[n+1];
	strcpy(ret, str);
	return ret;
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     バッファ付き変換                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

char* tmp_strcat(const char* str1, const char* str2)
{
	static char tmp[_MAX_PATH*2+2];
	strsafecpy(tmp, str1, countof(tmp));
	strsafecat(tmp, str2, countof(tmp));
	return tmp;
}

char* tmp_itoa(int i)
{
	static char tmp[32];
	itoa(i, tmp, 10);
	return tmp;
}

char* tmp_vsprintf(const char* format, va_list mark)
{
	static char buffer[1024];
	vsprintf(buffer, format, mark);
	return buffer;
}

char* tmp_sprintf(const char* format, ...)
{
	va_list mark;
	va_start(mark, format);
	char* ret=tmp_vsprintf(format, mark);
	va_end(mark);
	return ret;
}

char* buf_vsprintf(char* buffer,const char* format, va_list mark)
{
	vsprintf(buffer, format, mark);
	return buffer;
}

char* buf_sprintf(char* buffer,const char* format, ...)
{
	va_list mark;
	va_start(mark, format);
	char* ret=buf_vsprintf(buffer, format, mark);
	va_end(mark);
	return ret;
}

char* tmp_itoa_comma(int num,int step)
{
	static char str[256]; char* p; int p_len;
	itoa(num,str,10);
	p=strchr(str,'\0'); p_len=0;
	while(1){
		p-=step; p_len+=step;
		if(p>=str+1){
			memmove(p+1,p,p_len+1);
			*p=','; p_len++;
		}else{
			break;
		}
	}
	return str;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         変換・置換                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

char* uppercpy(char* str1, const char* str2)
{
	char* p=str1;
	const char* q=str2;
	while(*q!='\0')*p++=toupper(*q++);
	*p++='\0';
	return str1;
}

//分割
vector<string> strsplit(const char* str,char sep)
{
	std::vector<std::string> ret;
	const char* p=str;
	const char* q;
	int str_len=(int)strlen(str);
	while(1){
		q=strchr(p,sep);
		if(q==0)q=strchr(p, '\0');
		ret.push_back(std::string(p,q));
		if(*q == '\0')break;
		p=q+1;
	}
	return ret;
}

int strreplace(char* str,int from,int to)
{
	int ret=0;
	char* p=str;
	while((p=strchr(p,from))!=NULL){
		*p++=to;
		ret++;
	}
	return ret;
}


std::string cpp_strreplace(const std::string& src,const char* from,const char* to)
{
	const char* p=src.c_str();
	int src_len=(int)src.length();
	int from_len=(int)strlen(from);
	int to_len=(int)strlen(to);
	std::string ret;
	while(1){
		const char* q=strstr(p,from);
		if(q){
			//p～qをretに追加
			ret.append(p,q);
			//toをretに追加
			ret.append(to,to+to_len);
			//pを進める
			p=q+from_len;
		}
		else{
			//p～\0をretに追加
			ret.append(p);
			break;
		}
	}
	return ret;
}

//※バッファサイズは気にしないので注意
/*
int strreplace(char* str,const char* from,const char* to)
{
	int cnt=0;
	size_t nfrom=strlen(from);
	size_t nto=strlen(to);
	char* p=str;
	char* q0;
	char* q1;
	while(1){
		p=strstr(p,from);
		if(p){
			//ヒット数加算
			cnt++;
			//メモリ移動
			q0=p+nfrom;
			q1=p+nto;
			memmove(q1,q0,strlen(q0)+1);
			//置換
			memcpy(p,to,nto);
			//次の検索位置
			p=q1;
		}else{
			break;
		}
	}
	return cnt;
}
*/
int strreplace(char* str,const char* from,const char* to)
{
	int ret=0;
	char* p=str;
	int str_len=(int)strlen(str);
	int from_len=(int)strlen(from);
	int to_len=(int)strlen(to);
	while(p=strstr(p,from)){
		memmove(p+to_len,p+from_len,str_len-(p+from_len-str)+1);
		str_len-=from_len-to_len;
		memmove(p,to,to_len);
		p+=to_len;
		ret++;
	}
	return ret;
}

int strsafereplace(char* str,const char* from,const char* to,int str_size)
{
	char* p=str;
	int str_len=(int)strlen(str);
	int from_len=(int)strlen(from);
	int to_len=(int)strlen(to);
	int e;
	int ret=0;
	while(p=strstr(p,from)){
		ret++;
		if(from_len>=to_len){ //減る
			memmove(p+to_len,p+from_len,str_len-(p+from_len-str)+1);
			str_len-=from_len-to_len;
			memmove(p,to,to_len);
			p+=to_len;
		}else{ //増える
			e=0;
			if(str_len+to_len-from_len>str_size-1)e=str_len+to_len-from_len-(str_size-1);
			memmove(p+to_len,p+from_len,str_len-(p+from_len-str)+1-e);
			str_len+=to_len-from_len-e;
			if(p+to_len-str>str_size-1)e=(int)(p+to_len-str-(str_size-1));
			memmove(p,to,to_len-e);
			p+=to_len-e;
			if(p-str>=str_size)break;
		}
	}
	str[str_size-1]='\0';
	return ret;
}


int strreplace_once(char* str,const char* from,const char* to)
{
	int ret=0;
	char* p=str;
	int str_len=(int)strlen(str);
	int from_len=(int)strlen(from);
	int to_len=(int)strlen(to);
	if(p=strstr(p,from)){
		memmove(p+to_len,p+from_len,str_len-(p+from_len-str)+1);
		str_len-=from_len-to_len;
		memmove(p,to,to_len);
		p+=to_len;
		ret++;
	}
	return ret;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     文字列←→数値                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//###radixやbitsが0でないことをアサートする必要あるかも

//下位ビット数分のマスクを作る
int bits_mask(int bits)
{
	int n=0;
	for(int i=0;i<bits;i++){
		n<<=1;
		n|=1;
	}
	return n;
}

//下位ビット数を数える
int count_bits(int n)
{
	int ret=0;
	while(n!=0){
		n>>=1;
		ret++;
	}
	return ret;
}

//下位桁数を数える
static int count_keta(int n,int radix)
{
	int ret=0;
	while(n!=0){
		n/=radix;
		ret++;
	}
	return ret;
}


//桁溢れの動作は未定義
int atoi_radix(const char* str,int radix)
{
	const char* p=str;
	int ret=0;
	int i;
	while(*p){
		i=ctoi_radix(*p++,radix);
		if(i==-1)break;
		ret*=radix;
		ret+=i;
	}
	return ret;
}

//
const char* itoa_radix(int n,int radix,int ketapad)
{
	static char buf[64];
	//桁数調べる
	int keta=count_keta(n,radix);
	if(keta==0)keta=1;
	if(keta<ketapad)keta=ketapad;
	//変換
	int k;
	char* p=&buf[keta];
	*p--='\0';
	for(int i=0;i<keta;i++){
		k=n%radix;
		*p--=itoc_radix(k,radix);
		n/=radix;
	}
	return buf;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                            パス                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
bool ext_check(const char* path,const char* ext)
{
	const char* p=strchr(path,'\0')-strlen(ext);
	if(p<path)return false;
	if(stricmp(p,ext)!=0)return false; //大文字小文字を区別しない
	return true;
}

void remove_ext(char* path,bool repeat)
{
	//すべての拡張子を消す
	if(repeat){
		char* p=strchr(path,'.');
		if(p)*p='\0';
	}
	//最後の拡張子のみ消す
	else{
		char* p=strrchr(path,'.');
		if(p)*p='\0';
	}
}

void remove_ftitle(char* path)
{
	char* p=strrchr(path,'/');
	if(p){
		*(p+1)='\0';
	}
}

//"../"を展開
void fextract(char* str)
{
	while(1){
		char* q=strstr(str,"/../");
		if(q){
			char* q2=q-1;
			while(q2>=str && *q2!='/')q2--;
			if(q2>=str){
				memmove(q2+1,q+4,strlen(q+4)+1);
			}else{
				break;
//				memmove(str,q+4,strlen(q+4)+1);
			}
		}else{
			break;
		}
	}
}




// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         トークン                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int get_token(char* str,char** token,int maxtoken)
{
	int ntoken=0;
	char* p=str;
	char* begin,*end;
	while(*p!='\0' && ntoken<maxtoken){
		while(*p==' ')p++;
		begin=p;
		while(*p!=' ' && *p!='\0')p++;
		end=p;
		if(*end!='\0'){
			*end='\0';
			p++;
		}
		if(*begin!='\0'){
			token[ntoken++]=begin;
		}
	}
	return ntoken;
}

int alloc_token(const char* str,char** token,int maxtoken,const char* delimiters,const char* brackets)
{
	if(delimiters==NULL)delimiters=" ";
	if(brackets==NULL)brackets="";
	int ndelimiters=(int)strlen(delimiters);
	int nbrackets=(int)strlen(brackets);
	int ntoken=0;
	char* p=(char*)str,*q;
	char* begin,*end;
	while(*p!='\0' && ntoken<maxtoken){
		if(q=strchr((char*)brackets,*p)){
			begin=p;
			q=strchr(p+1,*q);
			if(q==NULL){
				break;
			}else{
				p=q+1;
				end=p;
				token[ntoken++]=strndup(begin,(int)(end-begin));
			}
		}else if(strchr(delimiters,*p)!=NULL){
			p++; //スペースをスキップ
		}else{
			begin=p;
			while(strchr(delimiters,*p)==NULL && strchr(brackets,*p)==NULL && *p!='\0')p++; //次のスペースorリテラルまで進む
			end=p;
			token[ntoken++]=strndup(begin,(int)(end-begin));
		}
	}
	return ntoken;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        日本語処理                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

char* make_kanji(const char* str,char* kanji)
{
	if(kanji==NULL)kanji=(char*)malloc(strlen(str)+1);
	const char* p=str;
	char* q=kanji;
	int k=0;
	while(*p!='\0'){
		if(k==0){
			if(KANJI1(*p))*q='\1';
			else *q='\0';
		}else if(k==1){
			if(KANJI2(*p))*q='\2';
			else *q='\0';
		}else if(k==2){
			if(KANJI1(*p))*q='\1';
			else *q='\0';
		}
		k=*q;
		p++;
		q++;
	}
	*q='\0';
	return kanji;
}

char* kanji_strchr(const char* str,char* _kanji,int c)
{
	char* ret=NULL;
	char* kanji=_kanji;
	if(kanji==NULL)kanji=make_kanji(str,NULL);
	//まず c の判定
	unsigned char c1=((unsigned char*)&c)[1];
	unsigned char c2=((unsigned char*)&c)[0];
	//検索
	const unsigned char* p=(unsigned char*)str;
	const unsigned char* k=(unsigned char*)kanji;
	if(c1==0){
		c1=c2;
		//1バイト文字
		while(*p!='\0'){
			if(*p==c1 && *k=='\0'){ ret=(char*)p; goto end; }
			p++;
			k++;
		}
	}else{
		//2バイト文字
		int flag=0;
		while(*p!='\0'){
			if(*p==c1 && *k=='\1'){ flag=1; }
			else if(flag!=0 && *p==c2 && *k=='\2'){ ret=(char*)(p-1); goto end; }
			else{ flag=0; }
			p++;
			k++;
		}
	}
end:
	if(_kanji==NULL)free(kanji);
	return ret;
}

char* kanji_strrchr(const char* str,char* _kanji,int c)
{
	char* ret=NULL;
	char* kanji=_kanji;
	if(kanji==NULL)kanji=make_kanji(str,NULL);
	//まず c の判定
	unsigned char c1=((unsigned char*)&c)[1];
	unsigned char c2=((unsigned char*)&c)[0];
	//検索
	int str_len=(int)strlen(str);
	const unsigned char* p=(unsigned char*)&str[str_len-1];
	const unsigned char* k=(unsigned char*)&kanji[str_len-1];
	if(c1==0){
		c1=c2;
		//1バイト文字
		while(p>=(const unsigned char*)str){
			if(*p==c1 && *k=='\0'){ ret=(char*)p; goto end; }
			p--;
			k--;
		}
	}else{
		//2バイト文字
		int flag=0;
		while(p>=(const unsigned char*)str){
			if(*p==c2 && *k=='\2'){ flag=1; }
			else if(flag!=0 && *p==c1 && *k=='\1'){ ret=(char*)p; goto end; }
			else{ flag=0; }
			p--;
			k--;
		}
	}
end:
	if(_kanji==NULL)free(kanji);
	return ret;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      ワイルドカード                         //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//参考URL
//http://www.hcn.zaq.ne.jp/no-ji/lib/fnamecmp.c

//!ワイルドカードとの一致検証。シングルバイトにのみ対応
/*!
@param test    対象文字列
@param pattern ワイルドカード
@retval        一致していればtrue
*/
bool wildmatch(const char* test,const char* pattern)
{
//	printf("Filename_compare(%s %s)\n", test,pattern);

	const char* p=test;
	const char* q=pattern;

	//	パターンの最初の文字により場合分け
	switch( *q ) {
	case '\0':
		// パターンが空のとき、ファイル名も空なら(TRUE)。
		// そうでないなら、(FALSE)を返す。
		return *p=='\0';
	case '*':
		// * のとき、 *以降の文字列と、何文字か読み飛ばした
		// 検査ファイル名で比較する。もし、それ以降が一致している
		// なら(TRUE)を返す。
		while(*p){
			if(wildmatch(p,q+1))return true;
			p++;
		}
		// ここに来たとき、testはすべて読み飛ばしている。
		// * の後に文字がなければ、両方空なので(TRUE)を返す。
		// * の後に文字が残っていれば、一致しなかったことになり、
		// (FALSE)を返す。
		return *(q+1)=='\0';
	case '?':
		// 終端に達してたらダメ
		if(*p=='\0')return false;
		// それぞれ１文字ずつ読み飛ばし、比較する。
		return wildmatch(p+1,q+1);
	default:
		// ワイルドカード以外の通常の文字のときには、大文字小文字を
		// 無視して比較する。違っていれば(FALSE)を返す。
		if(toupper(*p)!=toupper(*q))return false;
		// 同じなら、次の文字を比較する。
		return wildmatch(p+1,q+1);
	}
	//	ここに来ることはない。
	return false;
}


/*
	URLエンコード

	方法はいたって簡単で、ASCIIコードの[a-zA-Z0-9-_.!~*'()]に対応するバイトはそのままとし、
	それ以外で半角スペースは「+」に、それ以外を16進数表記に直して「%1f」のように表現するエンコード形式です。
*/
static bool noconvert_table[256];

static void urlencode_init()
{
	static int init = 0;
	if(init)return;
	for(int i = 0; i < 256; i++){
		char c = (char)((unsigned char)i);
		bool noconvert = false;
		if(c >= 'A' && c <= 'Z')noconvert = true;
		else if(c >= 'a' && c <= 'z')noconvert = true;
		else if(c >= '0' && c <= '9')noconvert = true;
//		else if(strchr("=&", c))noconvert = true;
//		else if(strchr(".-_*", c))noconvert = true; //「*」も無変換？？//
		noconvert_table[i] = noconvert;
	}
	init = 1;
}

inline bool NO_CONVERT(char c)
{
	return noconvert_table[(unsigned char)c];
}

std::string urlencode(const std::string& str)
{
	urlencode_init();
	std::vector<char> vBuf(str.length() * 3 + 1);
	char* pBuf = &vBuf[0];
	char* p = pBuf;
	const char* q = str.c_str();
	while(*q){
		if(NO_CONVERT(*q)){
			*p++ = *q++;
		}
		else if(*q == ' '){
			*p++ = '+';
			q++;
		}
		else{
			*p++ = '%';
			sprintf(p, "%02x", (unsigned char)*q);	//###非効率
			p += 2;
			q++;
		}
	}
	*p++ = '\0';
	return pBuf;
}

/*
	URLデコード
*/

// '0'-'9' → 0～9
// 'A'-'F' → 10～15
static int ctoi_table[256];

inline int ctoi16(char c)
{
	return ctoi_table[(unsigned char)c];
}

static void urldecode_init()
{
	static int init = 0;
	if(init)return;
	for(int i = 0; i < 256; i++){
		ctoi_table[i] = ctoi_radix((char)((unsigned char)i), 16);
	}
	init = 1;
}

std::string urldecode(const std::string& str)
{
	urldecode_init();
	std::vector<char> vBuf(str.length() + 1);
	char* pBuf = &vBuf[0];
	char* p = pBuf;
	const char* q = str.c_str();
	while(*q){
		if(*q == '%'){
			q++;
			if(q[0] && q[1]){
				char c = (ctoi16(q[0]) << 4) | (ctoi16(q[1]) << 0);
				*p++ = c;
				q += 2;
			}
			else{
				*p++ = '%';
			}
		}
		else if(*q == '+'){
			*p++ = ' ';
			q++;
		}
		else{
			*p++ = *q++;
		}
	}
	*p++ = '\0';
	return pBuf;
}

// SHA-1ハッシュ //
std::string sha1(const void* pData, int iLen)
{
	boost::uuids::detail::sha1 s;
	unsigned int d[5];
	s.process_bytes(pData, iLen);
	s.get_digest(d);
	char buf[41];
	sprintf(buf, "%08x%08x%08x%08x%08x", d[0], d[1], d[2], d[3], d[4]);
	return buf;
}

} //namespace util



