#include "_required.h"
#include "CDataStream.h"
#include <vector>
using namespace std;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                                     CDataInputStream                                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

byte CDataInputStream::ReadByte()
{
	byte n;
	Read((byte*)&n,sizeof(byte));
	return n;
}

short CDataInputStream::ReadShort()
{
	short n;
	Read((byte*)&n,sizeof(short));
	return n;
}

int CDataInputStream::ReadInt()
{
	int n;
	Read((byte*)&n,sizeof(int));
	return n;
}

//まるごと
void CDataInputStream::ReadAll(std::vector<BYTE>* pvData)
{
	uint n = this->Available();
	pvData->resize(n);
	BYTE* p = &(*pvData)[0];
	this->Read(p, n);
}

wstring CDataInputStream::readStringWithLength()
{
	size_t len=ReadInt();
	vector<wchar_t> buf(len+1);
	Read(&buf[0],(uint)len);
	buf[len]=L'\0';
	return &buf[0];
}

	//文字
//	virtual int getc(){ return 0; }     //!< 1文字読む(char)
//	virtual wint_t getwc(){ return 0; } //!< 1文字読む(wchar_t)


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                                     CDataOutputStream                                      //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               コンストラクタ・デストラクタ                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDataOutputStream::WriteByte(byte n)
{
	Write((byte*)&n,sizeof(byte));
}

void CDataOutputStream::WriteShort(short n)
{
	Write((byte*)&n,sizeof(short));
}

void CDataOutputStream::WriteInt(int n)
{
	Write((byte*)&n,sizeof(int));
}

void CDataOutputStream::WriteStringWithLength(const wchar_t* str)
{
	int len=(int)wcslen(str);
	WriteInt(len);
	Write(str,sizeof(wchar_t)*len);
}

