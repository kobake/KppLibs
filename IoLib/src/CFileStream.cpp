#include "_required.h"
#include <DebugLib.h>
#include "CFileStream.h"
#include <string>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      CFileInputStream                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

// -- -- コンストラクタ・デストラクタ -- -- //

CFileInputStream::CFileInputStream(const wchar_t* path,const wchar_t* mode)
{
	fp=_wfopen(path,mode);
	fp_hold=true;
	if(!fp)throw Io::OpenException(L"OpenException(%s)",path);
}

CFileInputStream::CFileInputStream(FILE* _fp,bool _hold)
{
	fp=_fp;
	fp_hold=_hold;
}

CFileInputStream::~CFileInputStream()
{
	if(fp_hold){
		Close();
	}
}


// -- -- インターフェース -- -- //

bool CFileInputStream::good() const
{
	return fp!=NULL;
}

bool CFileInputStream::Eof() const
{
	return feof(fp)!=0;
}

int CFileInputStream::Read(void* p,int size)
{
	return (int)fread(p,1,size,fp);
}

void CFileInputStream::peek(void* p,int size)
{
	size_t r=fread(p,1,size,fp);
	fseek(fp,(long)r*-1,SEEK_CUR);
}

void CFileInputStream::skip(uint n)
{
	fseek(fp,n,SEEK_CUR);
}

void CFileInputStream::seek(int n,SeekMode mode)
{
	if(mode==ESEEK_CUR){
		fseek(fp,n,SEEK_CUR);
	}else if(mode==ESEEK_BEGIN){
		fseek(fp,n,SEEK_SET);
	}else if(mode==ESEEK_END){
		fseek(fp,n,SEEK_END);
	}
}

uint CFileInputStream::tell() const
{
	return (uint)ftell(fp);
}

uint CFileInputStream::Available() const
{
	long cur=ftell(fp);
	fseek(fp,0,SEEK_END);
	long end=ftell(fp);
	fseek(fp,cur,SEEK_SET);
	return end-cur;
}

void CFileInputStream::Close()
{
	if(fp){
		fclose(fp);
		fp=NULL;
	}
}

int CFileInputStream::get()
{
	return getc(fp);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      CFileOutputStream                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CFileOutputStream::CFileOutputStream(const wchar_t* path,const wchar_t* mode)
{
	fp=_wfopen(path,mode);
	if(!fp)throw Io::OpenException(L"OpenException(%s)",path);
}

CFileOutputStream::~CFileOutputStream()
{
	Close();
}

bool CFileOutputStream::good() const
{
	return fp!=NULL;
}

int CFileOutputStream::Write(const void* p,int size)
{
	return (uint)fwrite(p,1,size,fp);
}

void CFileOutputStream::skip(uint n)
{
	fseek(fp,n,SEEK_CUR);
}

void CFileOutputStream::seek(int n,SeekMode mode)
{
	if(mode==ESEEK_CUR){
		fseek(fp,n,SEEK_CUR);
	}else if(mode==ESEEK_BEGIN){
		fseek(fp,n,SEEK_SET);
	}else if(mode==ESEEK_END){
		fseek(fp,n,SEEK_END);
	}
}

uint CFileOutputStream::tell() const
{
	return (uint)ftell(fp);
}

void CFileOutputStream::Close()
{
	if(fp){
		fclose(fp);
		fp=NULL;
	}
}

void CFileOutputStream::put(int n)
{
	putc(n,fp);
}

