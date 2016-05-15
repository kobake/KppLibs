#include "common/_required.h"
#include <cstdio>
#include <errno.h>
#include <direct.h>
#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "util/std.h"
#include "util/file.h"
#include <StringLib.h>

#include "std/myexception.h"

namespace util{

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     ディレクトリ操作                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!ディレクトリの存在チェック。存在していれば true 。
bool dexist(const wchar_t* path)
{
	return fexist(path);
	/*
	if(_wmkdir(path)!=0 && errno==EEXIST){
		return true; //既に存在していた
	}else{
		_wrmdir(path);
		return false;
	}
	*/
}

//!ディレクトリを作る。必要なら再帰的に作る。
int dtouch(const wchar_t* _path)
{
	int ret=0;
	wchar_t* path=wcsdup(_path);
	wchar_t* p=path;
	while(1){
		p=wcschr(p,L'\\');
		if(p!=NULL)*p=L'\0';
		if(_wmkdir(path)!=0){
			if(errno==EEXIST){
			}else if(errno==ENOENT){
				ret=-1;
				goto end; //無効なパス
			}
		}else{
			//フォルダ作成１ステップ成功
		}
		if(p!=NULL){
			*p++=L'\\';
		}else{
			break;
		}
	}
end:
	free(path);
	return ret;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        属性の取得                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!ストリームから読み取れるサイズを取得
int getfpsize(FILE *fp)
{
	long pos1=ftell(fp);
	fseek(fp,0,SEEK_END);
	long pos2=ftell(fp);
	fseek(fp,pos1,SEEK_SET);
	return pos2-pos1;
}

//!ファイルサイズを取得
__int64 getfsize(const wchar_t* szPath)
{
	/*
	int ret;
	FILE *fp=_wfopen(path,L"rb");
	if(fp==NULL)return -1;
	ret=getfpsize(fp);
	fclose(fp);
	return ret;
	*/
	struct _stati64 s;
	if(0==_wstati64(szPath, &s)){
		return s.st_size;
	}
	else{
		return -1;
	}
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                    ファイル単位の操作                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!ファイルの存在チェック。存在していれば true 。
bool fexist(const wchar_t* path)
{
	struct _stat s;
	return _wstat(path,&s)==0;
}


//!ファイルを作る
int ftouch(const wchar_t* path)
{
	if(!fexist(path)){
		FILE *fp=_wfopen(path,L"ab");
		if(fp==NULL)return -1;
		fclose(fp);
	}
	return 0;
}


#define BUFSIZE 1024

//!ストリームからストリームへコピー。EOFに達したら終了。
int fcopy(FILE *fpin,FILE *fpout)
{
	int ret=0;
	const int nbuf=BUFSIZE;
	byte buf[nbuf];
	unsigned int r;
	while(1){
		r=(unsigned int)fread(buf,sizeof(byte),nbuf,fpin);
		if(r==0){
			if(feof(fpin)){
				break;
			}else{
				ret=-1; goto end;
			}
		}else{
			if(fwrite(buf,sizeof(byte),r,fpout)<r){
				ret=-1; goto end;
			}
		}
	}
	ret=0;
end:
	return ret;
}

//!ファイルからファイルへコピー。
int fcopy(const wchar_t* from,const wchar_t* to)
{
	int ret=0;
	FILE *fpin=NULL;
	FILE *fpout=NULL;
	fpin=_wfopen(from,L"rb"); if(fpin==NULL) { ret=-1; goto end; }
	fpout=_wfopen(to,L"wb");  if(fpout==NULL){ ret=-1; goto end; }
	fcopy(fpin,fpout);
	ret=0;
end:
	if(fpin!=NULL)fclose(fpin);
	if(fpout!=NULL)fclose(fpout);
	return ret;
}

//! ファイルのバックアップを作成する。バックアップファイル名は「(元のファイル名).(インデックス)」とする。
void fbackup(const wchar_t* fpath)
{
	vector<wchar_t> buf;
	buf.resize(wcslen(fpath)+1+5);
	wchar_t* backup=&buf[0];
	wcscpy(backup,fpath);
	wchar_t* ext=wcschr(backup,L'\0');
	for(int i=0;i<=9999;i++){
		_swprintf(ext,L".%04d",i);
		if(fexist(backup))continue;

		fcopy(fpath,backup);
		return;
	}
	throw myexception(L"バックアップを作成できませんでした");
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        内容の編集                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!ストリームからストリームへ置換しながら転送
int fstrreplace(FILE *fpin,FILE *fpout,const wchar_t* from,const wchar_t* to)
{
	int ret=0;
	int nfrom=(int)wcslen(from);
	int nto=(int)wcslen(to);
	int nbuf=BUFSIZE+nfrom;
	wchar_t* buf=new wchar_t[nbuf];
	//
	int r; wchar_t* p,*q,*mark;
	int rest,size;
	rest=0;
	while(1){
		r=(int)fread(&buf[rest],sizeof(wchar_t),nbuf-rest,fpin);
		if(r+rest<nbuf){
			if(!feof(fpin)){
				ret=-1;
				goto end;
			}
		}
		size=r+rest;
		mark=&buf[size-nfrom];
		q=buf;
		for(p=buf;p<=mark;){
			if(wcsncmp(p,from,nfrom)==0){
				fwrite(q,sizeof(wchar_t),p-q,fpout);
				fwrite(to,sizeof(wchar_t),nto,fpout);
				p+=nfrom;
				q=p;
			}else{
				p++;
			}
		}
		fwrite(q,sizeof(wchar_t),p-q,fpout);
		q=p;
		if(r+rest==nbuf){
			//余りを左へシフト
			memmove(buf,q,(rest=(int)(nbuf-(p-buf)))*sizeof(wchar_t));
		}else{
			//余りを出力
			fwrite(q,sizeof(wchar_t),size-(p-buf),fpout);
			break;
		}
	}
	//
	ret=0;
end:
	delete[] buf;
	return ret;
}

//!ファイル内置換
int fstrreplace(const wchar_t* path,const wchar_t* from,const wchar_t* to)
{
	int ret=0;
	FILE *fptmp=NULL,*fpin=NULL,*fpout=NULL;
	//
	fptmp=tmpfile(); if(fptmp==NULL){ ret=-1; goto end; }
	fpin=_wfopen(path,L"rb"); if(fpin==NULL){ ret=-1; goto end; }
	fstrreplace(fpin,fptmp,from,to);
	fclose(fpin); fpin=NULL;
	//
	fpout=_wfopen(path,L"wb"); if(fpout==NULL){ ret=-1; goto end; }
	fseek(fptmp,0,SEEK_SET);
	fcopy(fptmp,fpout);
	//
	ret=0;
end:
	if(fptmp!=NULL){ if(fpout==fptmp)fpout=NULL; _rmtmp(); }
	if(fpin!=NULL)fclose(fpin);
	if(fpout!=NULL)fclose(fpout);
	return ret;
}

//!ファイルからファイルへ置換しながら転送
int fstrreplace(const wchar_t* pathfrom,const wchar_t* pathto,const wchar_t* from,const wchar_t* to)
{
	int ret=0;
	FILE *fpin=NULL,*fpout=NULL;
	//
	fpin= _wfopen(pathfrom,L"rb"); if(fpin==NULL) { ret=-1; goto end; }
	fpout=_wfopen(pathto,L"wb");   if(fpout==NULL){ ret=-1; goto end; }
	fstrreplace(fpin,fpout,from,to);
	//
	ret=0;
end:
	if(fpin!=NULL)fclose(fpin);
	if(fpout!=NULL)fclose(fpout);
	return ret;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       メモリ変換                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void* file_alloc(const wchar_t* path,const wchar_t* mode)
{
	FILE* fp=_wfopen(path,mode);
	if(fp){
		fseek(fp,0,SEEK_END);
		long size=ftell(fp);
		fseek(fp,0,SEEK_SET);
		byte* buf=new byte[size+1];
		size=(int)fread(buf,sizeof(byte),size,fp);
		buf[size]='\0';
		fclose(fp);
		return buf;
	}
	return NULL;
}

void file_free(void* _p)
{
	if(_p){
		byte* p=(byte*)_p;
		delete[] p;
	}
}


} //namespace util

