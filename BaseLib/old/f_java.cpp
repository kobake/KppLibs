#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "f_java.h"

int SJIStoUTF8N(const char * src,char * dst,int dst_size);
int UTF8NtoSJIS(const char * src,char * dst,int dst_size);

void java_writeShort(FILE *fp,const short _value)
{
	BYTE *value=(BYTE*)&_value;
	fwrite(&value[1],sizeof(BYTE),1,fp);
	fwrite(&value[0],sizeof(BYTE),1,fp);
}

void java_writeInt(FILE *fp,const int _value)
{
	BYTE *value=(BYTE*)&_value;
	fwrite(&value[3],sizeof(BYTE),1,fp);
	fwrite(&value[2],sizeof(BYTE),1,fp);
	fwrite(&value[1],sizeof(BYTE),1,fp);
	fwrite(&value[0],sizeof(BYTE),1,fp);
}

int java_readInt(FILE *fp)
{
	BYTE bt[4];
	fread(bt,sizeof(BYTE),4,fp);
	return (bt[0]<<24) | (bt[1]<<16) | (bt[2]<<8) | (bt[3]<<0);
}


void java_writeString(FILE *fp,const char *_value)
{
	int nvalue=strlen(_value)*3+2;
	char *value=(char*)malloc(nvalue+1);
	int ret=SJIStoUTF8N(_value,value,nvalue);
	if(ret>0){
		java_writeShort(fp,ret-1);
		fwrite(value,sizeof(char),ret-1,fp);
	}
	free(value);
}
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



//---------------------------------------------------------------------------
// ローカル関数
//---------------------------------------------------------------------------
static int doconv(const char * src,char * dst,int dst_size,int stoutf);

//---------------------------------------------------------------------------
// SJIS2UTF8N()
//! NULL終端のSJISの文字列を、NULL終端のUTF8Nに変換する
/*!
\param	src	[i]		変換元のSJIS文字列ポインタ
\param	dst	[i/o]	変換後のUTF8Nを返す先のポインタ
\param	dst_size	[i]		wdstの上限サイズ。この値-1byteまで記録できる。
						0を指定した場合、dstは無視され、NULL終端込みの必要な容量が返る
\retval	1以上	変換後のバイト数
\retval	0		エラー
\retval	-1		作業メモリの確保失敗
*/
//---------------------------------------------------------------------------
int SJIStoUTF8N(const char * src,char * dst,int dst_size)
{
	return doconv(src,dst,dst_size,1);
}

//---------------------------------------------------------------------------
// UTF8N2SJIS()
//! NULL終端のUTF8N文字列をSJISへ変換する
/*!
\param	src	[i]		変換元のUTF8N文字列ポインタ
\param	dst	[i/o]	変換後のSJISを返す先のポインタ
\param	dst_size	[i]		wdstの上限サイズ。この値-1byteまで記録できる
						0を指定した場合、dstは無視され、NULL終端込みの必要な容量が返る
\retval	0		エラー
\retval	1以上	変換後のバイト数
\retval	-1		作業メモリの確保失敗
*/
//---------------------------------------------------------------------------
int UTF8NtoSJIS(const char * src,char * dst,int dst_size)
{
	return doconv(src,dst,dst_size,0);
}

//---------------------------------------------------------------------------
// doconv()
//! 実際の変換処理。
/*!
\param	src	[i]		変換元の文字列ポインタ
\param	dst	[i/o]	変換後の文字列を返す先のポインタ
\param	dst_size	[i]		wdstの上限サイズ。この値-1byteまで記録できる
\retval	0		エラー
\retval	1以上	変換後のバイト数
\retval	-1		作業メモリの確保失敗
*/
//---------------------------------------------------------------------------
int doconv(const char * src,char * dst,int dst_size,int stoutf)
{
	int i;
    int ires;
    LPWSTR wbuf;
    LPSTR  putf;

    // unicodeへ変換
	//// 事前チェック
	ires = MultiByteToWideChar(
		(stoutf!=0) ? CP_ACP : CP_UTF8,
        				// 文字コード
		0,				// フラグなし
		src,			// 変換元文字列
        -1,				// 変換文字列バイト数
        NULL,			// 変換後格納先
        0				// 格納領域取得
		);
	if (ires == 0)
    {
    	return 0;
    }
    //// ワーク取得
    wbuf = new WCHAR[ires+1];
    if (wbuf == NULL)
    {
    	return -1;
    }
	//// 実変換
	ires = MultiByteToWideChar(
		(stoutf!=0) ? CP_ACP : CP_UTF8,
        				// 文字コード
		0,				// フラグなし
		src,			// 変換元文字列
        -1,				// 変換文字列バイト数
        wbuf,			// 変換後格納先
        ires			// 格納領域取得
		);
	wbuf[ires]  = 0;
    if (ires == 0)
    {
    	return 0;
    }

    // UTF8への変換
	//// 事前チェック
	ires = WideCharToMultiByte(
		(stoutf==0) ? CP_ACP : CP_UTF8,
						// UTF8
		0,				// フラグなし
		wbuf,			// 変換元文字列
        -1,				// 変換文字列バイト数
        NULL,			// 変換後格納先
        0,				// 格納領域
		NULL,NULL
		);
	if (ires == 0)
    {
		delete [] wbuf;
    	return 0;
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
	return i;
}

