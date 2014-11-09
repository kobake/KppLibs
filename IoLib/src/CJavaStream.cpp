#include "_required.h"
#include "CJavaStream.h"
#include <windows.h>
#include <vector>
using namespace std;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       CJavaInputStream                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int CJavaInputStream::ReadInt()
{
	byte buf[4];
	Read(&buf,4);
	return (buf[0]<<24) | (buf[1]<<16) | (buf[2]<<8) | (buf[3]<<0);
}

short CJavaInputStream::ReadShort()
{
	byte buf[2];
	Read(&buf,2);
	return (buf[0]<<8) | (buf[1]<<0);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      CJavaOutputStream                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CJavaOutputStream::WriteInt(int n)
{
	byte* p=(byte*)&n;
	Write(&p[3],1);
	Write(&p[2],1);
	Write(&p[1],1);
	Write(&p[0],1);
}

void CJavaOutputStream::WriteShort(short n)
{
	byte* p=(byte*)&n;
	Write(&p[1],1);
	Write(&p[0],1);
}


void CJavaOutputStream::WriteString(const wchar_t* str)
{
	//�K�v�ȃo�b�t�@�T�C�Y���v�Z
	int nvalue=WideCharToMultiByte(
		CP_UTF8, //UTF-8�ɕϊ�
		0,       //�I�v�V�����Ȃ�
		str,     //�ϊ����o�b�t�@
		-1,      //�ϊ�������
		NULL,    //�ϊ���o�b�t�@
		0,       //�ϊ��撷��
		NULL,    //�f�t�H���g����
		NULL     //�f�t�H���g�����t���O
	);

	//�o�b�t�@�m��
	vector<char> buf(nvalue+1);

	//�ϊ�: Unicode(UTF-16) �� UTF-8(BOM�Ȃ�)
	int ret=WideCharToMultiByte(
		CP_UTF8, //UTF-8�ɕϊ�
		0,       //�I�v�V�����Ȃ�
		str,     //�ϊ����o�b�t�@
		-1,      //�ϊ�������
		&buf[0], //�ϊ���o�b�t�@
		nvalue,  //�ϊ��撷��
		NULL,    //�f�t�H���g����
		NULL     //�f�t�H���g�����t���O
	);

	//����
	if(ret>0){
		WriteShort(ret-1);
		Write(&buf[0],ret-1);
	}
	else{
		throw myexception(L"WideCharToMultiByte failed");
	}
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ����                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //



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


