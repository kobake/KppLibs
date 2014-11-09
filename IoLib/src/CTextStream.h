#pragma once

#include "CInputStream.h"
#include <string>

class CTextInputStream : public CFilterInputStream{
public:
	//コンストラクタ・デストラクタ
	CTextInputStream(CInputStream* in);
	//文字列
	std::wstring	ReadLine();							//!< 文字列を1行読み取る。改行があれば改行も含める
	int				ReadLine(char* buf, int buf_size);	//!< 文字列を1行読み取る。改行があれば改行も含める
};


#include "COutputStream.h"

//! 文字コード
enum ECharEncoding{
	SJIS,	//Shift_JIS
};

class CTextOutputStream : public CFilterOutputStream{
public:
	//コンストラクタ・デストラクタ
	CTextOutputStream(
		COutputStream*	out,
		ECharEncoding	eCharEncoding	= SJIS
	);
	//文字列
	void WriteString(const wchar_t* str);
	void WriteStringF(const wchar_t* str, ...);
private:
	ECharEncoding m_eCharEncoding;
};

