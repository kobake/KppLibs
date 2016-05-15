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
//                     �f�B���N�g������                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!�f�B���N�g���̑��݃`�F�b�N�B���݂��Ă���� true �B
bool dexist(const wchar_t* path)
{
	return fexist(path);
	/*
	if(_wmkdir(path)!=0 && errno==EEXIST){
		return true; //���ɑ��݂��Ă���
	}else{
		_wrmdir(path);
		return false;
	}
	*/
}

//!�f�B���N�g�������B�K�v�Ȃ�ċA�I�ɍ��B
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
				goto end; //�����ȃp�X
			}
		}else{
			//�t�H���_�쐬�P�X�e�b�v����
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
//                        �����̎擾                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!�X�g���[������ǂݎ���T�C�Y���擾
int getfpsize(FILE *fp)
{
	long pos1=ftell(fp);
	fseek(fp,0,SEEK_END);
	long pos2=ftell(fp);
	fseek(fp,pos1,SEEK_SET);
	return pos2-pos1;
}

//!�t�@�C���T�C�Y���擾
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
//                    �t�@�C���P�ʂ̑���                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!�t�@�C���̑��݃`�F�b�N�B���݂��Ă���� true �B
bool fexist(const wchar_t* path)
{
	struct _stat s;
	return _wstat(path,&s)==0;
}


//!�t�@�C�������
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

//!�X�g���[������X�g���[���փR�s�[�BEOF�ɒB������I���B
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

//!�t�@�C������t�@�C���փR�s�[�B
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

//! �t�@�C���̃o�b�N�A�b�v���쐬����B�o�b�N�A�b�v�t�@�C�����́u(���̃t�@�C����).(�C���f�b�N�X)�v�Ƃ���B
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
	throw myexception(L"�o�b�N�A�b�v���쐬�ł��܂���ł���");
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        ���e�̕ҏW                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//!�X�g���[������X�g���[���֒u�����Ȃ���]��
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
			//�]������փV�t�g
			memmove(buf,q,(rest=(int)(nbuf-(p-buf)))*sizeof(wchar_t));
		}else{
			//�]����o��
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

//!�t�@�C�����u��
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

//!�t�@�C������t�@�C���֒u�����Ȃ���]��
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
//                       �������ϊ�                            //
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

