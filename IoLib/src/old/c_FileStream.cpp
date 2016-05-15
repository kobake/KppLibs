#include <windows.h>
#include <cstdio>
#include <string>
using namespace std;
#include "common/_required.h"
#include "c_FileStream.h"
#include "t_base.h"


//       ※注意事項※         //
//                            //
// ・インスタンス間の代入禁止 //
//                            //

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
FileStream::FileStream(FILE *_fp)
{
	fp=_fp;
	to_close=false;
	tmpname=NULL;
}
FileStream::FileStream()
{
	fp=NULL;
	to_close=false;
	tmpname=NULL;
}
FileStream::FileStream(FileStream &left)
{
	//動作未確認、使用非推奨
	this->to_close=false;
	this->fp=left.fp;
	this->tmpname=NULL;
}

FileStream::~FileStream()
{
	if(to_close)close();
	if(tmpname!=NULL)free(tmpname);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- --   オープン・クローズ  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
bool FileStream::open(const wchar *fpath,const wchar *mode)
{
	if(to_close)close();
	fp=_wfopen(fpath,mode);
	if(fp!=NULL){
		to_close=true;
	}else{
		to_close=false;
	}
	return (fp!=NULL);
}
void FileStream::close()
{
	if(fp!=NULL){
		fclose(fp);
		fp=NULL;
	}
}

bool FileStream::tmp_open(const wchar *mode)
{
	if(tmpname!=NULL)m_free(tmpname);
	tmpname=wcsdup(_wtmpnam(NULL));
	return open(tmpname,mode);
}

//###removeに失敗した場合などの処理も加える
bool FileStream::tmp_close(const wchar *replacefile)
{
	if(replacefile!=NULL){
		//###ここで生成時刻、更新時刻などの情報を保持
		_wremove(replacefile);
	}
	close();
	if(replacefile!=NULL){
		_wrename(tmpname,replacefile);
		//###ここで生成時刻、更新時刻などの復元を行う
	}
	return true; //###エラーが起きた場合は false
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- --  入出力  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void FileStream::writeInt(int n)
{
	fwrite(&n,sizeof(int),1,fp);
}
int FileStream::readInt()
{
	int ret;
	fread(&ret,sizeof(int),1,fp);
	return ret;
}
void FileStream::writeByte(int n)
{
	putc(n,fp);
}
int FileStream::readByte()
{
	return getc(fp);
}
void FileStream::writeShort(short n)
{
	fwrite(&n,sizeof(short),1,fp);
}
short FileStream::readShort()
{
	short ret;
	fread(&ret,sizeof(short),1,fp);
	return ret;
}
int FileStream::read(void *buf,int size)
{
	return fread(buf,1,size,fp);
}
int FileStream::write(const void *buf,int size)
{
	return fwrite(buf,1,size,fp);
}


//int [サイズ] int [ランダム] int [(サイズ+ランダム)] wchar[] [テキスト]
wchar *FileStream::readTextFormatAlloc()
{
	int size,r,crc;
	size_t ret;
	wchar *p=NULL;
	//
	LONG begin=ftell(fp);
	//サイズ情報読み込み
	ret=fread(&size, sizeof(int), 1, fp); if(ret!=1)goto err;
	ret=fread(&r,    sizeof(int), 1, fp); if(ret!=1)goto err;
	ret=fread(&crc,  sizeof(int), 1, fp); if(ret!=1)goto err;
	//CRCチェック
	if(size+r!=crc){
		goto err;
	}
	//テキスト読み込み
	p=(wchar*)calloc(size+1,sizeof(wchar));
	ret=fread(p,sizeof(wchar),size,fp);
	p[ret]=L'\0';
	return p;
err:
	free(p);
	fseek(fp,begin,SEEK_SET);
	return NULL;
}

bool FileStream::writeTextFormat(const wchar *text)
{
	int size,r,crc;
	size_t ret;
	size=wcslen(text);
	r=rand();
	crc=size+r;
	//
	LONG begin=ftell(fp);
	//
	ret=fwrite(&size, sizeof(int), 1, fp); if(ret!=1)goto err;
	ret=fwrite(&r,    sizeof(int), 1, fp); if(ret!=1)goto err;
	ret=fwrite(&crc,  sizeof(int), 1, fp); if(ret!=1)goto err;
	ret=fwrite(text, sizeof(wchar), size, fp); if((int)ret!=size)goto err;
	return true;
err:
	fseek(fp,begin,SEEK_SET);
	return false;
}

wchar *FileStream::readAllAlloc(long *_size)
{
	long size=getLength();
	wchar *buf=(wchar*)calloc(size+1,sizeof(wchar));
	size=read(buf,size);
	buf[size]=L'\0';
	if(_size!=NULL)*_size=size;
	return buf;	
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- --  シーク  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void FileStream::seekSet(long n)
{
	fseek(fp,n,SEEK_SET);
}

void FileStream::seekCur(long n)
{
	fseek(fp,n,SEEK_CUR);
}

void FileStream::seekEnd(long n)
{
	fseek(fp,n,SEEK_END);
}

long FileStream::getLength()
{
	long begin=ftell(fp);
	fseek(fp,0,SEEK_END);
	long end=ftell(fp);
	fseek(fp,begin,SEEK_SET);
	return end-begin;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- --  ex_io  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


void FileStream::ex_write(const int& t)
{
	writeInt(t);
}

void FileStream::ex_read(int& t)
{
	t=readInt();
}

void FileStream::ex_write(const wstring& t)
{
	const wchar *p=t.c_str();
	int n=wcslen(p);
	writeInt(n);
	write(p,n*sizeof(wchar));
}

void FileStream::ex_read(wstring& t)
{
	int n;
	n=readInt();
	wchar *p=new wchar[n];
	read(p,n*sizeof(wchar));
	t.assign(p[0],p[n]);
	delete[] p;
}
