#include "_required.h"
#include "CTextStream.h"
#include <StringLib.h>
using namespace std;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           入力                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CTextInputStream::CTextInputStream(CInputStream* in)
: CFilterInputStream(in)
{
}

//! 文字列を1行読み取る。改行があれば改行も含める
wstring CTextInputStream::ReadLine()
{
	//## 効率悪いかも

	//1行読み取り→char[]
	vector<char> buf;
	while(!this->Eof()){
		char c;
		this->Read(&c,sizeof(c));
		buf.push_back(c);
		if(c=='\n')break;
	}
	buf.push_back(L'\0');

	//char[]→wchar_t[]
	size_t wlen=mbstowcs(NULL,&buf[0],strlen(&buf[0]));
	vector<wchar_t> wbuf(wlen+1,L'\0');
	size_t ret=mbstowcs(&wbuf[0],&buf[0],strlen(&buf[0]));
	wbuf[ret]=L'\0';

	//wchar_t[]→wstring
	return &wbuf[0];
}

//! 文字列を1行読み取る。改行があれば改行も含める
int CTextInputStream::ReadLine(char* ret, int ret_size)
{
	//## 効率悪いかも

	//1行読み取り→char[]
	vector<char> buf;
	while(!this->Eof()){
		char c;
		this->Read(&c,sizeof(c));
		buf.push_back(c);
		if(c=='\n')break;
	}
	buf.push_back(L'\0');

	strcpy_s(ret,ret_size,&buf[0]);

	return (int)buf.size()-1;
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           出力                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CTextOutputStream::CTextOutputStream(
	COutputStream*	out,
	ECharEncoding	eCharEncoding
)
: CFilterOutputStream(out)
, m_eCharEncoding(eCharEncoding)
{
}

void CTextOutputStream::WriteString(const wchar_t* str)
{
	string s = cpp_wcstombs(str);
	this->Write(s.c_str(), (uint)s.length());
}

void CTextOutputStream::WriteStringF(const wchar_t* str, ...)
{
	va_list v;
	va_start(v, str);
	wstring s = cpp_vswprintf(str, v);
	va_end(v);
	this->WriteString(s.c_str());
}
