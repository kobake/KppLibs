#include <BaseLibOld.h>
#include <windows.h>
#include "c_Registry.h"
#include <StringLib.h>
#include <BaseLibOld.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
using namespace util;


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- --  [struct] REG_ROOT_KEY  -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

wchar *REG_ROOT_KEY::getRootKeyName()
{
	static wchar ret[512];
	wcscpy(ret,root_name);
	wcscat(ret,L"\\");
	wcscat(ret,key_name);
	return ret;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- --  [struct] REG_DATA   -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

const wchar *REG_DATA::toString()
{
	static wchar *ret=NULL; //### プログラム終了時に開放したほうがいいかな？
	if(mode==REG_SZ || REG_EXPAND_SZ){
		return (wchar*)data;
	}else if(mode==REG_BINARY){
		ret=(wchar*)realloc(ret,ndata*3);
		wchar *p=ret;
		BYTE *q=data;
		unsigned int i;
		for(i=0;i<ndata;i++){
			wcsncpy(p,itow16(*q++,2),2);
			p+=2;
			*p++=L' ';
		}
		if(ndata>0){
			*(p-1)=L'\0';
		}else{
			*p=L'\0';
		}
		return ret;
	}else if(mode==REG_DWORD){
		ret=(wchar*)realloc(ret,16);
		_itow(*((DWORD*)data),ret,10);
		return ret;
	}else{
		return NULL;
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- [HASH] レジストリ値モード  -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

static NAME_VALUE_PAIR<DWORD> _reg_mode_table[]={
	L"REG_SZ",			REG_SZ,
	L"REG_BINARY",		REG_BINARY,
	L"REG_DWORD",		REG_DWORD,
	L"REG_EXPAND_SZ",	REG_EXPAND_SZ,
	NULL,-1,
	//全種類: REG_NONE,REG_SZ,REG_EXPAND_SZ,REG_BINARY,REG_DWORD,REG_DWORD_LITTLE_ENDIAN,REG_DWORD_BIG_ENDIAN,REG_LINK,REG_MULTI_SZ,REG_RESOURCE_LIST,REG_FULL_RESOURCE_DESCRIPTOR,REG_RESOURCE_REQUIREMENTS_LIST
};
DWORD regGetModeWord(const wchar *name)
{
	return name_to_value<DWORD>(name,-1,_reg_mode_table);
}
const wchar *regGetModeName(DWORD mode)
{
	return value_to_name<DWORD>(mode,L"REG_UNDEFINED",_reg_mode_table);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- [HASH] レジストリルートキー   -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

static NAME_VALUE_PAIR<HKEY> _reg_root_table[]={
	L"HKEY_CLASSES_ROOT",		HKEY_CLASSES_ROOT,
	L"HKEY_CURRENT_USER",		HKEY_CURRENT_USER,
	L"HKEY_LOCAL_MACHINE",		HKEY_LOCAL_MACHINE,
	L"HKEY_USERS",				HKEY_USERS,
	L"HKEY_PERFORMANCE_DATA",	HKEY_PERFORMANCE_DATA,
	L"HKEY_CURRENT_CONFIG",		HKEY_CURRENT_CONFIG,
	L"HKEY_DYN_DATA",			HKEY_DYN_DATA,
//	L"HKEY_PERF_ROOT",			HKEY_PERF_ROOT,
	NULL,						NULL,
};

HKEY regGetRootHandle(const wchar *name)
{
	return name_to_value<HKEY>(name,NULL,_reg_root_table);
}
const wchar *regGetRootName(HKEY handle)
{
	return value_to_name<HKEY>(handle,L"HKEY_UNDEFINED",_reg_root_table);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- レジストリキー表記文字列解析  -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

bool regSplitRootKey(const wchar *key,REG_ROOT_KEY *reg)
{
	const wchar* p;
	int n;
	p=wcschr(key,L'\\');
	if(p==NULL){
		wcssafecpy(reg->root_name,key,countof(reg->root_name));
		reg->key_name[0]=L'\0';
		return true;
	}else{
		wcsnsafecpy(reg->root_name,key,n=p-key,countof(reg->root_name));
		n=tmin<int>(n,countof(reg->root_name)-1); reg->root_name[n]=L'\0';
		//
		wcssafecpy(reg->key_name,p+1,countof(reg->key_name));
		return true;
	}
}
bool regSplitRootKeyValue(const wchar *key,REG_ROOT_KEY_VALUE *reg)
{
	const wchar* p;
	const wchar* q;
	int n;
	p=wcschr(key,L'\\');
	q=wcsrchr(key,L'\\');
	if(p==NULL){
		return false;
	}
	if(p==q){
		wcsnsafecpy(reg->root_name,key,n=p-key,countof(reg->root_name));
		n=tmin<int>(n,countof(reg->root_name)-1); reg->root_name[n]=L'\0';
		reg->key_name[0]=L'\0';
		wcssafecpy(reg->value_name,q+1,countof(reg->value_name));
		return true;
	}else{
		wcsnsafecpy(reg->root_name,key,n=p-key,countof(reg->root_name));
		if(n<countof(reg->root_name))reg->root_name[n]=L'\0';
		//
		wcsnsafecpy(reg->key_name,p+1,n=q-(p+1),countof(reg->key_name));
		n=tmin<int>(n,countof(reg->key_name)-1); reg->key_name[n]=L'\0';
		//
		wcssafecpy(reg->value_name,q+1,countof(reg->value_name));
		return true;
	}
}
bool regSplitKeyValue(const wchar *key,REG_KEY_VALUE *reg)
{
	const wchar* p;
	int n;
	p=wcsrchr(key,L'\\');
	if(p==NULL){
		reg->key_name[0]=L'\0';
		wcssafecpy(reg->value_name,key,countof(reg->value_name));
		return true;
	}else{
		wcsnsafecpy(reg->key_name,key,n=p-key,countof(reg->key_name));
		n=tmin<int>(n,countof(reg->key_name)-1); reg->key_name[n]=L'\0';
		//
		wcssafecpy(reg->value_name,p+1,countof(reg->value_name));
		return true;
	}
}

REG_ROOT_KEY *tmp_regSplitRootKey(const wchar *key)
{
	static REG_ROOT_KEY reg;
	bool ret=regSplitRootKey(key,&reg);
	if(ret){
		return &reg;
	}else{
		return NULL;
	}
}

//拡張atoi
//0x～を16進変換
//その他は10進変換
#define is_09(C) ((C)>=L'0' && (C)<=L'9')
#define is_AF(C) ((C)>=L'A' && (C)<=L'Z')
#define is_af(C) ((C)>=L'a' && (C)<=L'z')
#define is_09AFaf(C) ( is_09(C) || is_AF(C) || is_af(C) )

int atoi_ex(const wchar *str)
{
	if(str[0]==L'0' && (str[1]==L'x' || str[1]==L'X')){
		int n=0;
		const wchar *p=&str[2];
		while(1){
			if(is_09(*p)){
				n<<=4;
				n+=*p-'0';
			}else if(is_af(*p)){
				n<<=4;
				n+=*p-'a';
			}else if(is_AF(*p)){
				n<<=4;
				n+=*p-'A';
			}else{
				return n;
			}
			p++;
		}
	}else{
		return _wtoi(str);
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- レジストリ値表記文字列REG_DATA化  -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

REG_DATA *regConvertData(const wchar *source,DWORD mode)
{
	REG_DATA *ret=NULL;
	int n;
	if(mode==REG_SZ || mode==REG_EXPAND_SZ){
		n=wcslen(source)+1;
		ret=(REG_DATA*)malloc(sizeof(REG_DATA)-1+n);
		ret->mode=mode;
		ret->ndata=n;
		memcpy(ret->data,source,n);
	}else if(mode==REG_BINARY){
#define UNIT_SIZE 16
#define IS_SEP(C) ((C)==L' ')
		//メモリ仮確保
		n=0;
		ret=NULL;
		//
		BYTE *p=NULL;
		const wchar *s,*t;
		wchar tmp[16];
		int nn;
		t=source-1;
		do{
			if(n%UNIT_SIZE==0){
				ret=(REG_DATA*)realloc(ret,sizeof(REG_DATA)-1+n+UNIT_SIZE); //バッファ追加確保
				p=&ret->data[n];
			}
			//要素先頭
			s=t+1;
			while(IS_SEP(*s)){
				if(*s==L'\0')break;
				s++;
			}
			while(*s==L'0')s++; //tmpバッファ節約のため、先頭のゼロはあらかじめ削って計算する
			//要素後尾
			t=s;
			while(!IS_SEP(*t) && *t!=L'\0')t++;
			//要素変換
			nn=tmin<int>(countof(tmp)-1,t-s);
			if(nn==0){
				*p++=0;
			}else{
				wcsncpy(tmp,s,nn);
				tmp[nn]=L'\0';
				//2桁以上ある場合は、下2桁のみを用いる
				nn=wcslen(tmp);
				if(nn>2)memmove(tmp,&tmp[nn-2],3);
				*p++=wtoi16(tmp);
			}
			n++;
		}while(*t!=L'\0');
		ret->mode=mode;
		ret->ndata=n;
#undef IS_SEP
#undef UNIT_SIZE
	}else if(mode==REG_DWORD){
		n=sizeof(DWORD);
		ret=(REG_DATA*)malloc(sizeof(REG_DATA)-1+n);
		ret->mode=mode;
		ret->ndata=n;
		DWORD d=atoi_ex(source);
		memcpy(ret->data,&d,sizeof(DWORD));
	}
	return ret;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- [class] Registry  -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

Registry::Registry()
{
	hkeyCurrent=NULL;
}
Registry::~Registry()
{
	close();
}

bool Registry::exist(const wchar *key)
{
	return exist(tmp_regSplitRootKey(key));
}
bool Registry::vexistf(const wchar *key,va_list mark)
{
	return exist(tmp_vswprintf(key,mark));
}
bool Registry::exist(REG_ROOT_KEY *reg)
{
	if(reg==NULL)return false;
	HKEY hkeyBackup=hkeyCurrent;
	hkeyCurrent=NULL;
	bool ret=open(false,reg);
	if(ret)
		close();
	hkeyCurrent=hkeyBackup;
	return true;
}



bool Registry::open(bool create,const wchar *key)
{
	return open(create,tmp_regSplitRootKey(key));
}
bool Registry::vopenf(bool create,const wchar *key,va_list mark)
{
	return open(create,tmp_vswprintf(key,mark));
}
bool Registry::open(bool create,REG_ROOT_KEY *reg)
{
	if(reg==NULL)return false;
	close();
	HKEY hkeyRoot=regGetRootHandle(reg->root_name);
	if(hkeyRoot==NULL)
		return false;
	LONG ret;
	if(!create){
		ret=RegOpenKeyEx(hkeyRoot,reg->key_name,0,KEY_ALL_ACCESS,&hkeyCurrent);
	}else{
		DWORD d; //作成されたか既存のを開いたかが格納される
		ret=RegCreateKeyEx(hkeyRoot,reg->key_name,0,L"",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hkeyCurrent,&d);
	}
	return (ret==ERROR_SUCCESS);
}

bool Registry::openf(bool create,const wchar *key,...)
{
	va_list mark;
	va_start(mark,key);
	bool ret=vopenf(create,key,mark);
	va_end(mark);
	return ret;
}

bool Registry::existf(const wchar *key,...)
{
	va_list mark;
	va_start(mark,key);
	bool ret=vexistf(key,mark);
	va_end(mark);
	return ret;
}


bool Registry::close()
{
	if(hkeyCurrent!=NULL){
		LONG ret=RegCloseKey(hkeyCurrent);
		if(ret==ERROR_SUCCESS){
			hkeyCurrent=NULL;
			return true;
		}else{
			return false;
		}
	}else{
		return true;
	}
}
bool Registry::set(wchar *name,REG_DATA *value)
{
	if(hkeyCurrent!=NULL){
		if(value->mode==REG_SZ || value->mode==REG_EXPAND_SZ){
			LONG ret=RegSetValueEx(hkeyCurrent,name,0,value->mode,(BYTE*)value->data,value->ndata);
			return (ret==ERROR_SUCCESS);
		}else if(value->mode==REG_BINARY){
			LONG ret=RegSetValueEx(hkeyCurrent,name,0,value->mode,(BYTE*)value->data,value->ndata);
			return (ret==ERROR_SUCCESS);
		}else if(value->mode==REG_DWORD){
			LONG ret=RegSetValueEx(hkeyCurrent,name,0,value->mode,(BYTE*)value->data,value->ndata);
			return (ret==ERROR_SUCCESS);
		}else{
			return false;
		}
	}else{
		return false;
	}
}
bool Registry::setText(wchar *name,wchar *text)
{
	REG_DATA *data=regConvertData(text,REG_SZ);
	bool ret=set(name,data);
	free(data);
	return ret;
}


