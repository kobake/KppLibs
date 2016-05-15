#include <windows.h>
#include <cstdio>
#include <string>
using namespace std;
#include "common/_required.h"
#include "c_FileStream.h"
#include "t_base.h"


//       �����ӎ�����         //
//                            //
// �E�C���X�^���X�Ԃ̑���֎~ //
//                            //

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  �R���X�g���N�^�E�f�X�g���N�^  -- -- -- -- -- -- -- -- -- -- -- -- -- //
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
	//���얢�m�F�A�g�p�񐄏�
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
// -- -- -- -- -- -- -- -- -- -- -- --   �I�[�v���E�N���[�Y  -- -- -- -- -- -- -- -- -- -- -- -- -- //
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

//###remove�Ɏ��s�����ꍇ�Ȃǂ̏�����������
bool FileStream::tmp_close(const wchar *replacefile)
{
	if(replacefile!=NULL){
		//###�����Ő��������A�X�V�����Ȃǂ̏���ێ�
		_wremove(replacefile);
	}
	close();
	if(replacefile!=NULL){
		_wrename(tmpname,replacefile);
		//###�����Ő��������A�X�V�����Ȃǂ̕������s��
	}
	return true; //###�G���[���N�����ꍇ�� false
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- --  ���o��  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
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


//int [�T�C�Y] int [�����_��] int [(�T�C�Y+�����_��)] wchar[] [�e�L�X�g]
wchar *FileStream::readTextFormatAlloc()
{
	int size,r,crc;
	size_t ret;
	wchar *p=NULL;
	//
	LONG begin=ftell(fp);
	//�T�C�Y���ǂݍ���
	ret=fread(&size, sizeof(int), 1, fp); if(ret!=1)goto err;
	ret=fread(&r,    sizeof(int), 1, fp); if(ret!=1)goto err;
	ret=fread(&crc,  sizeof(int), 1, fp); if(ret!=1)goto err;
	//CRC�`�F�b�N
	if(size+r!=crc){
		goto err;
	}
	//�e�L�X�g�ǂݍ���
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
// -- -- -- -- -- -- -- -- -- -- -- -- --  �V�[�N  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
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
