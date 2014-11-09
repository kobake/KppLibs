#pragma once

#include "CInputStream.h"

class CJavaInputStream : public CFilterInputStream{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CJavaInputStream(CInputStream* _in) : CFilterInputStream(_in) { }
	virtual ~CJavaInputStream(){ }
	//����
	virtual int ReadInt();
	virtual short ReadShort();
};


#include "COutputStream.h"

class CJavaOutputStream : public CFilterOutputStream{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CJavaOutputStream(COutputStream* _out) : CFilterOutputStream(_out) { }
	virtual ~CJavaOutputStream(){ }
	//����
	virtual void WriteInt(int n);
	virtual void WriteShort(short n);
	virtual void WriteString(const wchar_t* str);
};


void java_writeShort(FILE *fp,const short value);
void java_writeInt(FILE *fp,const int value);
void java_writeString(FILE *fp,const char *value);
int java_readInt(FILE *fp);
