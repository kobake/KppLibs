#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "f_java.h"

int SJIStoUTF8N(const char * src,char * dst,int dst_size);
int UTF8NtoSJIS(const char * src,char * dst,int dst_size);

void java_writeShort(FILE *fp,const short _value)
{
	BYTE *value=(BYTE*)&_value;
	fwrite(&value[1],sizeof(BYTE),1,fp);
	fwrite(&value[0],sizeof(BYTE),1,fp);
}

void java_writeInt(FILE *fp,const int _value)
{
	BYTE *value=(BYTE*)&_value;
	fwrite(&value[3],sizeof(BYTE),1,fp);
	fwrite(&value[2],sizeof(BYTE),1,fp);
	fwrite(&value[1],sizeof(BYTE),1,fp);
	fwrite(&value[0],sizeof(BYTE),1,fp);
}

int java_readInt(FILE *fp)
{
	BYTE bt[4];
	fread(bt,sizeof(BYTE),4,fp);
	return (bt[0]<<24) | (bt[1]<<16) | (bt[2]<<8) | (bt[3]<<0);
}


void java_writeString(FILE *fp,const char *_value)
{
	int nvalue=strlen(_value)*3+2;
	char *value=(char*)malloc(nvalue+1);
	int ret=SJIStoUTF8N(_value,value,nvalue);
	if(ret>0){
		java_writeShort(fp,ret-1);
		fwrite(value,sizeof(char),ret-1,fp);
	}
	free(value);
}
/*
void java_writeString(FILE *fp,const char *_value)
{
	int widesize;
	char *widebuf;
	int utfsize;
	char *utfbuf;

	// -- -- Multi��Wide �������m�� -- -- //
	widesize = MultiByteToWideChar(
		CP_ACP,
        				// �����R�[�h
		0,				// �t���O�Ȃ�
		_value,			// �ϊ���������
        -1,				// �ϊ�������o�C�g��
        NULL,			// �ϊ���i�[��
        0				// �i�[�̈�擾
		);
	if(widesize == 0){
		utfsize=0;
		goto writea;
    }
	widebuf=(char*)malloc(widesize+1);
	
	// -- -- Multi��Wide ���ϊ� -- -- //
	widesize = MultiByteToWideChar(
		CP_ACP,
        				// �����R�[�h
		0,				// �t���O�Ȃ�
		_value,			// �ϊ���������
        -1,				// �ϊ�������o�C�g��
        widebuf,			// �ϊ���i�[��
        widesize			// �i�[�̈�擾
		);
	wbuf[widesize]  = 0;
    if (widesize == 0){
		utfsize=0;
		goto writea;
    }

    // -- -- Wide��UTF8 �������m�� -- -- //
	utfsize = WideCharToMultiByte(
		CP_UTF8,
						// UTF8
		0,				// �t���O�Ȃ�
		widebuf,		// �ϊ���������
        -1,				// �ϊ�������o�C�g��
        NULL,			// �ϊ���i�[��
        0,				// �i�[�̈�
		NULL,NULL
		);
	if (utfsize == 0){
		goto writea;
    }
	// �e�ʃ`�F�b�N
    if (dst_size == 0)
    {
    	delete [] wbuf;
        return ires+1;
    }

    //// �̈�m��
    putf = new char[ires+1];
    if (putf == NULL)
    {
		delete [] wbuf;
		return -1;
    }
    //// ���ϊ�
	ires = WideCharToMultiByte(
		(stoutf==0) ? CP_ACP : CP_UTF8,
						// UTF8
		0,				// �t���O�Ȃ�
		wbuf,			// �ϊ���������
        -1,				// �ϊ�������o�C�g��
        putf,			// �ϊ���i�[��
        ires,			// �i�[�̈�
		NULL,NULL
		);
	if (ires == 0)
    {
	    delete [] wbuf;
    	delete [] putf;
        return 0;
    }

    // �R�s�[
	ires = ((dst_size-1) > ires) ? ires : dst_size-1;
    for (i=0 ; i<ires ; i++)
    {
		dst[i] = putf[i];
    }
    dst[i] = 0;

    delete [] wbuf;
   	delete [] putf;
}
*/



//---------------------------------------------------------------------------
// ���[�J���֐�
//---------------------------------------------------------------------------
static int doconv(const char * src,char * dst,int dst_size,int stoutf);

//---------------------------------------------------------------------------
// SJIS2UTF8N()
//! NULL�I�[��SJIS�̕�������ANULL�I�[��UTF8N�ɕϊ�����
/*!
\param	src	[i]		�ϊ�����SJIS������|�C���^
\param	dst	[i/o]	�ϊ����UTF8N��Ԃ���̃|�C���^
\param	dst_size	[i]		wdst�̏���T�C�Y�B���̒l-1byte�܂ŋL�^�ł���B
						0���w�肵���ꍇ�Adst�͖�������ANULL�I�[���݂̕K�v�ȗe�ʂ��Ԃ�
\retval	1�ȏ�	�ϊ���̃o�C�g��
\retval	0		�G���[
\retval	-1		��ƃ������̊m�ێ��s
*/
//---------------------------------------------------------------------------
int SJIStoUTF8N(const char * src,char * dst,int dst_size)
{
	return doconv(src,dst,dst_size,1);
}

//---------------------------------------------------------------------------
// UTF8N2SJIS()
//! NULL�I�[��UTF8N�������SJIS�֕ϊ�����
/*!
\param	src	[i]		�ϊ�����UTF8N������|�C���^
\param	dst	[i/o]	�ϊ����SJIS��Ԃ���̃|�C���^
\param	dst_size	[i]		wdst�̏���T�C�Y�B���̒l-1byte�܂ŋL�^�ł���
						0���w�肵���ꍇ�Adst�͖�������ANULL�I�[���݂̕K�v�ȗe�ʂ��Ԃ�
\retval	0		�G���[
\retval	1�ȏ�	�ϊ���̃o�C�g��
\retval	-1		��ƃ������̊m�ێ��s
*/
//---------------------------------------------------------------------------
int UTF8NtoSJIS(const char * src,char * dst,int dst_size)
{
	return doconv(src,dst,dst_size,0);
}

//---------------------------------------------------------------------------
// doconv()
//! ���ۂ̕ϊ������B
/*!
\param	src	[i]		�ϊ����̕�����|�C���^
\param	dst	[i/o]	�ϊ���̕������Ԃ���̃|�C���^
\param	dst_size	[i]		wdst�̏���T�C�Y�B���̒l-1byte�܂ŋL�^�ł���
\retval	0		�G���[
\retval	1�ȏ�	�ϊ���̃o�C�g��
\retval	-1		��ƃ������̊m�ێ��s
*/
//---------------------------------------------------------------------------
int doconv(const char * src,char * dst,int dst_size,int stoutf)
{
	int i;
    int ires;
    LPWSTR wbuf;
    LPSTR  putf;

    // unicode�֕ϊ�
	//// ���O�`�F�b�N
	ires = MultiByteToWideChar(
		(stoutf!=0) ? CP_ACP : CP_UTF8,
        				// �����R�[�h
		0,				// �t���O�Ȃ�
		src,			// �ϊ���������
        -1,				// �ϊ�������o�C�g��
        NULL,			// �ϊ���i�[��
        0				// �i�[�̈�擾
		);
	if (ires == 0)
    {
    	return 0;
    }
    //// ���[�N�擾
    wbuf = new WCHAR[ires+1];
    if (wbuf == NULL)
    {
    	return -1;
    }
	//// ���ϊ�
	ires = MultiByteToWideChar(
		(stoutf!=0) ? CP_ACP : CP_UTF8,
        				// �����R�[�h
		0,				// �t���O�Ȃ�
		src,			// �ϊ���������
        -1,				// �ϊ�������o�C�g��
        wbuf,			// �ϊ���i�[��
        ires			// �i�[�̈�擾
		);
	wbuf[ires]  = 0;
    if (ires == 0)
    {
    	return 0;
    }

    // UTF8�ւ̕ϊ�
	//// ���O�`�F�b�N
	ires = WideCharToMultiByte(
		(stoutf==0) ? CP_ACP : CP_UTF8,
						// UTF8
		0,				// �t���O�Ȃ�
		wbuf,			// �ϊ���������
        -1,				// �ϊ�������o�C�g��
        NULL,			// �ϊ���i�[��
        0,				// �i�[�̈�
		NULL,NULL
		);
	if (ires == 0)
    {
		delete [] wbuf;
    	return 0;
    }
	// �e�ʃ`�F�b�N
    if (dst_size == 0)
    {
    	delete [] wbuf;
        return ires+1;
    }

    //// �̈�m��
    putf = new char[ires+1];
    if (putf == NULL)
    {
		delete [] wbuf;
		return -1;
    }
    //// ���ϊ�
	ires = WideCharToMultiByte(
		(stoutf==0) ? CP_ACP : CP_UTF8,
						// UTF8
		0,				// �t���O�Ȃ�
		wbuf,			// �ϊ���������
        -1,				// �ϊ�������o�C�g��
        putf,			// �ϊ���i�[��
        ires,			// �i�[�̈�
		NULL,NULL
		);
	if (ires == 0)
    {
	    delete [] wbuf;
    	delete [] putf;
        return 0;
    }

    // �R�s�[
	ires = ((dst_size-1) > ires) ? ires : dst_size-1;
    for (i=0 ; i<ires ; i++)
    {
		dst[i] = putf[i];
    }
    dst[i] = 0;

    delete [] wbuf;
   	delete [] putf;
	return i;
}

