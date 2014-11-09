#pragma once


//enum IoFlag;

//IoFlag operator | (IoFlag n,IoFlag m);

#include "std/myexception.h"

class Io{
public:
	//定数
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
	//ツール
//	static int toStdFlag(IoFlag flag);
public:
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                  staticインターフェース                     //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//!ファイル名の変更。失敗した場合は例外を投げる。
	static void rename(
		const wchar_t* oldpath, //!<古いパス
		const wchar_t* newpath, //!<新しいパス
		bool overwrite=false  //!<上書きを許す
	);

public:
	//例外
	//入出力に失敗したときに発行
	GEN_ARGSEXCEPTION(IoException, myexception);

	//オープン失敗
	GEN_ARGSEXCEPTION(OpenException,IoException);

	//ファイルの終わりに達したときに発行
	GEN_ARGSEXCEPTION(EofException, IoException);

	//シーク失敗
	GEN_ARGSEXCEPTION(SeekException, IoException);

	//クローズ失敗
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
