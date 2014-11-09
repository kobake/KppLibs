#pragma once

#include "CInputStream.h"
#include <string>

class CTextInputStream : public CFilterInputStream{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CTextInputStream(CInputStream* in);
	//������
	std::wstring	ReadLine();							//!< �������1�s�ǂݎ��B���s������Ή��s���܂߂�
	int				ReadLine(char* buf, int buf_size);	//!< �������1�s�ǂݎ��B���s������Ή��s���܂߂�
};


#include "COutputStream.h"

//! �����R�[�h
enum ECharEncoding{
	SJIS,	//Shift_JIS
};

class CTextOutputStream : public CFilterOutputStream{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CTextOutputStream(
		COutputStream*	out,
		ECharEncoding	eCharEncoding	= SJIS
	);
	//������
	void WriteString(const wchar_t* str);
	void WriteStringF(const wchar_t* str, ...);
private:
	ECharEncoding m_eCharEncoding;
};

