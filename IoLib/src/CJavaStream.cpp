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
	//必要なバッファサイズを計算
	int nvalue=WideCharToMultiByte(
		CP_UTF8, //UTF-8に変換
		0,       //オプションなし
		str,     //変換元バッファ
		-1,      //変換元長さ
		NULL,    //変換先バッファ
		0,       //変換先長さ
		NULL,    //デフォルト文字
		NULL     //デフォルト文字フラグ
	);

	//バッファ確保
	vector<char> buf(nvalue+1);

	//変換: Unicode(UTF-16) → UTF-8(BOMなし)
	int ret=WideCharToMultiByte(
		CP_UTF8, //UTF-8に変換
		0,       //オプションなし
		str,     //変換元バッファ
		-1,      //変換元長さ
		&buf[0], //変換先バッファ
		nvalue,  //変換先長さ
		NULL,    //デフォルト文字
		NULL     //デフォルト文字フラグ
	);

	//書込
	if(ret>0){
		WriteShort(ret-1);
		Write(&buf[0],ret-1);
	}
	else{
		throw myexception(L"WideCharToMultiByte failed");
	}
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           実装                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //



/*
void java_writeString(FILE *fp,const char *_value)
{
	int widesize;
	char *widebuf;
	int utfsize;
	char *utfbuf;

	// -- -- Multi→Wide メモリ確保 -- -- //
	widesize = MultiByteToWideChar(
		CP_ACP,
        				// 文字コード
		0,				// フラグなし
		_value,			// 変換元文字列
        -1,				// 変換文字列バイト数
        NULL,			// 変換後格納先
        0				// 格納領域取得
		);
	if(widesize == 0){
		utfsize=0;
		goto writea;
    }
	widebuf=(char*)malloc(widesize+1);
	
	// -- -- Multi→Wide 実変換 -- -- //
	widesize = MultiByteToWideChar(
		CP_ACP,
        				// 文字コード
		0,				// フラグなし
		_value,			// 変換元文字列
        -1,				// 変換文字列バイト数
        widebuf,			// 変換後格納先
        widesize			// 格納領域取得
		);
	wbuf[widesize]  = 0;
    if (widesize == 0){
		utfsize=0;
		goto writea;
    }

    // -- -- Wide→UTF8 メモリ確保 -- -- //
	utfsize = WideCharToMultiByte(
		CP_UTF8,
						// UTF8
		0,				// フラグなし
		widebuf,		// 変換元文字列
        -1,				// 変換文字列バイト数
        NULL,			// 変換後格納先
        0,				// 格納領域
		NULL,NULL
		);
	if (utfsize == 0){
		goto writea;
    }
	// 容量チェック
    if (dst_size == 0)
    {
    	delete [] wbuf;
        return ires+1;
    }

    //// 領域確保
    putf = new char[ires+1];
    if (putf == NULL)
    {
		delete [] wbuf;
		return -1;
    }
    //// 実変換
	ires = WideCharToMultiByte(
		(stoutf==0) ? CP_ACP : CP_UTF8,
						// UTF8
		0,				// フラグなし
		wbuf,			// 変換元文字列
        -1,				// 変換文字列バイト数
        putf,			// 変換後格納先
        ires,			// 格納領域
		NULL,NULL
		);
	if (ires == 0)
    {
	    delete [] wbuf;
    	delete [] putf;
        return 0;
    }

    // コピー
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


