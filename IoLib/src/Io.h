#pragma once


//enum IoFlag;

//IoFlag operator | (IoFlag n,IoFlag m);

#include "std/myexception.h"

class Io{
public:
	//�萔
	enum OpenMode{
		in		=0x1,
		out		=0x2,
		binary	=0x4,
		text	=0x8,
	};
	enum SeekMode{
		ESEEK_CUR,
		ESEEK_BEGIN,
		ESEEK_END,
	};
//	static const IoFlag in;
//	static const IoFlag out;
//	static const IoFlag binary;
//	static const IoFlag text;
protected:
	//�c�[��
//	static int toStdFlag(IoFlag flag);
public:
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                  static�C���^�[�t�F�[�X                     //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//!�t�@�C�����̕ύX�B���s�����ꍇ�͗�O�𓊂���B
	static void rename(
		const wchar_t* oldpath, //!<�Â��p�X
		const wchar_t* newpath, //!<�V�����p�X
		bool overwrite=false  //!<�㏑��������
	);

public:
	//��O
	//���o�͂Ɏ��s�����Ƃ��ɔ��s
	GEN_ARGSEXCEPTION(IoException, myexception);

	//�I�[�v�����s
	GEN_ARGSEXCEPTION(OpenException,IoException);

	//�t�@�C���̏I���ɒB�����Ƃ��ɔ��s
	GEN_ARGSEXCEPTION(EofException, IoException);

	//�V�[�N���s
	GEN_ARGSEXCEPTION(SeekException, IoException);

	//�N���[�Y���s
	GEN_ARGSEXCEPTION(CloseException, IoException);

	GEN_ARGSEXCEPTION(InvalidArguments, IoException);

	//bad_bit
//	class BadException : public IoException{
//	public:
//		BadException(const wchar_t* msg=L"unknown") : IoException(msg) { }
//	};

	//fail_bit
//	class FailException : public IoException{
//	public:
//		FailException(const wchar_t* msg=L"unknown") : IoException(msg) { }
//	};

};
/*
*/
