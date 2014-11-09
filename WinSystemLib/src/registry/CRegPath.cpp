#include "_required.h"
#include "CRegPath.h"
using namespace std;

CRegPath::CRegPath(const wchar_t* path,bool to_create)
{
	//ハイブ
	const wchar_t* p;
	const wchar_t* q;
	p=wcschr(path,L'\\');
	q=wcsrchr(p+1,L'\\');
	if(p==NULL){
		//ハイブあり、キーなし、値名なし
		hive=CRegHive(path);
		return;
	}
	hive=CRegHive(path);

	//キー
	wstring keypath;
	if(q){
		keypath.assign(p+1,q-(p+1));
	}else{
		keypath.assign(p+1);
	}
	key.Open(hive,keypath.c_str(),to_create);

	//値名
	if(q){
		value_name.assign(q+1);
	}
}

/*
bool regSplitRootKeyValue(const wchar_t *key,REG_ROOT_KEY_VALUE *reg)
{
	const wchar_t* p;
	const wchar_t* q;
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
*/
