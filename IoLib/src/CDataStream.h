#pragma once

#include "CInputStream.h"
#include <string>
#include <vector>

class CDataInputStream : public CFilterInputStream{
private:
	typedef std::wstring wstring;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CDataInputStream(CInputStream* _in) : CFilterInputStream(_in) { }
	//�v���~�e�B�u
	byte ReadByte();
	short ReadShort();
	int ReadInt();
	//�܂邲��
	void ReadAll(std::vector<BYTE>* pvData);

	//�\���̓�
	template <class T> void ReadAny(T* t){ Read(t,sizeof(T)); }
	//����
//	virtual int getc(){ return 0; }     //!< 1�����ǂ�(char)
//	virtual wint_t getwc(){ return 0; } //!< 1�����ǂ�(wchar_t)
	//������
	wstring readStringWithLength(); //!< ��������ǂݎ��A����������UNICODE�������ǂݎ��B
};


#include "COutputStream.h"

class CDataOutputStream : public CFilterOutputStream{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CDataOutputStream(COutputStream* _out) : CFilterOutputStream(_out) { }
	//�v���~�e�B�u
	void WriteByte(byte n);
	void WriteShort(short n);
	void WriteInt(int n);
	//�\���̓�
	template <class T> void writeAny(const T& t){ write(&t,sizeof(T)); }
	//������
	void WriteStringWithLength(const wchar_t* str); //!< ���������o�͂��AUNICODE��������o�́B
};

