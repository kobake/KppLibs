#include "charset.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>	// for mbstowcs(), wcstombs()
#include <locale.h>	// for setlocale()


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




/* -----------------------------------------------------------------------
 *  関数名  ：  CUnicodeF::utf8_to_sjis
 *  機能概要：  UTF-8 -> SJIS へ文字列のコード変換
 *  引数    ：  (I)  char *pUtf8Str - 変換元UTF-8文字列へのポインタ
 *              (O)  int *nBytesOut - 変換後文字列のバイト数
 *  戻り値  ：  char* 変換後SJIS文字列へのポインタ
 *
 *  処理概要：
 * -----------------------------------------------------------------------
 */
char *CUnicodeF::utf8_to_sjis(const char *pUtf8Str, int *nBytesOut)
{
    int nNum, nBytes;

    wchar_t *pwcWork = utf8_to_utf16be( pUtf8Str, &nNum, TRUE);
    char *pcSjis = utf16be_to_sjis( pwcWork, &nBytes);
    free( pwcWork);

    *nBytesOut = nBytes;
    return pcSjis;
}

/* -----------------------------------------------------------------------
 *  関数名  ：  CUnicodeF::sjis_to_utf8
 *  機能概要：  SJIS -> UTF-8 へ文字列のコード変換
 *  引数    ：  (I)  char *pAnsiStr - 変換元SJIS文字列へのポインタ
 *              (O)  int *nBytesOut - 変換後文字列のバイト数
 *  戻り値  ：  char* 変換後UTF-8文字列へのポインタ
 *
 *  処理概要：
 * -----------------------------------------------------------------------
 */
char *CUnicodeF::sjis_to_utf8(const char *pAnsiStr, int *nBytesOut)
{
    int nNum, nBytes;

    wchar_t *pwcWork = sjis_to_utf16be( pAnsiStr, &nNum);
    char *pcUtf8 = utf16be_to_utf8( pwcWork, &nBytes);
    free( pwcWork);

    *nBytesOut = nBytes;
    return pcUtf8;
}


/* -----------------------------------------------------------------------
 *  関数名  ：  CUnicodeF::utf16be_to_sjis
 *  機能概要：  UTF-16BE -> SJIS へ文字列のコード変換
 *  引数    ：  (I)  wchar_t *pUcsStr - 変換元UTF16BE文字列へのポインタ
 *              (O)  int *nBytesOut - 変換後文字列のバイト数
 *  戻り値  ：  char* 変換後SJIS文字列へのポインタ
 *
 *  処理概要：
 * -----------------------------------------------------------------------
 */
char *CUnicodeF::utf16be_to_sjis(const wchar_t *pUcsStr, int *nBytesOut)
{
    char *pAnsiStr = NULL;
    int nLen;

    if (!pUcsStr) return NULL;

    setlocale(LC_ALL, "Japanese");// これがないとUnicodeに変換されない！

    nLen = (int)wcslen( pUcsStr);

    if ( pUcsStr[0] == 0xfeff || pUcsStr[0] == 0xfffe) {
        pUcsStr++; // 先頭にBOM(byte Order Mark)があれば，スキップする
        nLen--;
    }

    pAnsiStr = (char *)calloc((nLen+1), sizeof(wchar_t));
    if (!pAnsiStr) return NULL;

    // 1文字ずつ変換する。
    // まとめて変換すると、変換不能文字への対応が困難なので
    int nRet, i, nMbpos = 0;
    char *pcMbchar = new char[MB_CUR_MAX];

    for ( i=0; i < nLen; i++) {
        nRet = wctomb( pcMbchar, pUcsStr[i]);
        switch ( nRet) {
        case 1:
            pAnsiStr[nMbpos++] = pcMbchar[0];
            break;

        case 2:
            pAnsiStr[nMbpos++] = pcMbchar[0];
            pAnsiStr[nMbpos++] = pcMbchar[1];
            break;

        default: // 変換不能
            pAnsiStr[nMbpos++] = ' ';
            break;
        }
    }
    pAnsiStr[nMbpos] = '\0';

    delete [] pcMbchar;

    *nBytesOut = nMbpos;

    return pAnsiStr;
}

/* -----------------------------------------------------------------------
 *  関数名  ：  CUnicodeF::sjis_to_utf16be
 *  機能概要：  SJIS -> UTF-16 へ文字列のコード変換
 *  引数    ：  (I)    char *pAnsiStr - 変換元SJIS文字列へのポインタ
 *              (O)    int *nBytesOut - 変換後文字列のバイト数
 *  戻り値  ：  wchar_t* 変換後UTF-16BE文字列へのポインタ
 *
 *  処理概要：
 * -----------------------------------------------------------------------
 */
wchar_t *CUnicodeF::sjis_to_utf16be(const char *pAnsiStr, int *nBytesOut)
{
    int len;
    wchar_t *pUcsStr = NULL;

    if (!pAnsiStr) return NULL;

    setlocale(LC_ALL, "Japanese");  // これがないとUnicodeに変換されない！

    len = (int)strlen( pAnsiStr);
    *nBytesOut = sizeof(wchar_t)*(len);

    pUcsStr = (wchar_t *)calloc(*nBytesOut + 2, 1);
    if (!pUcsStr) return NULL;

    mbstowcs(pUcsStr, pAnsiStr, len+1);

    return pUcsStr;
}

/* -----------------------------------------------------------------------
 *  関数名  ：  CUnicodeF::utf16be_to_utf8
 *  機能概要：  UTF-16 -> UTF-8 へ文字列のコード変換
 *  引数    ：  (I) wchar_t *pUcsStr - 変換元UTF-16BE文字列へのポインタ
 *              (O) int *nBytesOut - 変換後文字列のバイト数
 *  戻り値  ：  char* 変換後UTF-8文字列へのポインタ
 *
 *  処理概要：
 * -----------------------------------------------------------------------
 */
char *CUnicodeF::utf16be_to_utf8(const wchar_t *pUcsStr, int *nBytesOut)
{
    int nUcsNum;
    char *pUtf8Str;

    nUcsNum = (int)wcslen(pUcsStr);

    *nBytesOut = utf16be_to_utf8_sub( NULL, pUcsStr, nUcsNum, TRUE);

    pUtf8Str = (char *)calloc(*nBytesOut + 3, 1);
    utf16be_to_utf8_sub( pUtf8Str, pUcsStr, nUcsNum, FALSE);

    return pUtf8Str;
}

// Unicode(UTF-16) -> UTF-8 下請け
int CUnicodeF::utf16be_to_utf8_sub( char *pUtf8, const wchar_t *pUcs2, int nUcsNum, BOOL bCountOnly)
{
    int nUcs2, nUtf8 = 0;

    for ( nUcs2=0; nUcs2 < nUcsNum; nUcs2++) {
        if ( (unsigned short)pUcs2[nUcs2] <= 0x007f) {
            if ( bCountOnly == FALSE) {
                pUtf8[nUtf8] = (pUcs2[nUcs2] & 0x007f);
            }
            nUtf8 += 1;
        } else if ( (unsigned short)pUcs2[nUcs2] <= 0x07ff) {
            if ( bCountOnly == FALSE) {
                pUtf8[nUtf8] = ((pUcs2[nUcs2] & 0x07C0) >> 6 ) | 0xc0; // 2002.08.17 修正
                pUtf8[nUtf8+1] = (pUcs2[nUcs2] & 0x003f) | 0x80;
            }
            nUtf8 += 2;
        } else {
            if ( bCountOnly == FALSE) {
                pUtf8[nUtf8] = ((pUcs2[nUcs2] & 0xf000) >> 12) | 0xe0; // 2002.08.04 修正
                pUtf8[nUtf8+1] = ((pUcs2[nUcs2] & 0x0fc0) >> 6) | 0x80;
                pUtf8[nUtf8+2] = (pUcs2[nUcs2] & 0x003f) | 0x80;
            }
            nUtf8 += 3;
        }
    }
    if ( bCountOnly == FALSE) {
        pUtf8[nUtf8] = '\0';
    }

    return nUtf8;
}

/* -----------------------------------------------------------------------
 *  関数名  ：  CUnicodeF::utf8_to_utf16be
 *  機能概要：  UTF-8 -> UTF-16(BE/LE) へ文字列のコード変換
 *  引数    ：  (I) char *pUtf8Str - 変換元UTF-8文字列へのポインタ
 *              (O) int *nNumOut - 変換結果のUTF-16文字数．Byte数ではない
 *              (I) BOOL bBigEndian - ビッグエンディアンに変換するならTRUE
 *
 *  戻り値  ：  wchar_t* 変換後文字列へのポインタ
 *
 *  処理概要：
 * -----------------------------------------------------------------------
 */
wchar_t *CUnicodeF::utf8_to_utf16be(const char *pUtf8Str, int *nNumOut, BOOL bBigEndian)
{
    int nUtf8Num;
    wchar_t *pUcsStr;

    nUtf8Num = (int)strlen(pUtf8Str); // UTF-8文字列には，'\0' がない
    *nNumOut = utf8_to_utf16be_sub( NULL, pUtf8Str, nUtf8Num, TRUE, bBigEndian);

    pUcsStr = (wchar_t *)calloc((*nNumOut + 1), sizeof(wchar_t));
    utf8_to_utf16be_sub( pUcsStr, pUtf8Str, nUtf8Num, FALSE, bBigEndian);

    return pUcsStr;
}

// UTF-8 -> Unicode(UCS-2) 下請け
int CUnicodeF::utf8_to_utf16be_sub( wchar_t *pUcs2, const char *pUtf8, int nUtf8Num,
                          BOOL bCountOnly, BOOL bBigEndian)
{
    int nUtf8, nUcs2 = 0;
    char cHigh, cLow;

    for ( nUtf8=0; nUtf8 < nUtf8Num;) {
        if ( ( pUtf8[nUtf8] & 0x80) == 0x00) { // 最上位ビット = 0
            if ( bCountOnly == FALSE) {
                pUcs2[nUcs2] = ( pUtf8[nUtf8] & 0x7f);
            }
            nUtf8 += 1;
        } else if ( ( pUtf8[nUtf8] & 0xe0) == 0xc0) { // 上位3ビット = 110
            if ( bCountOnly == FALSE) {
                pUcs2[nUcs2] = ( pUtf8[nUtf8] & 0x1f) << 6;
                pUcs2[nUcs2] |= ( pUtf8[nUtf8+1] & 0x3f);
            }
            nUtf8 += 2;
        } else {
            if ( bCountOnly == FALSE) {
                pUcs2[nUcs2] = ( pUtf8[nUtf8] & 0x0f) << 12;
                pUcs2[nUcs2] |= ( pUtf8[nUtf8+1] & 0x3f) << 6;
                pUcs2[nUcs2] |= ( pUtf8[nUtf8+2] & 0x3f);
            }
            nUtf8 += 3;
        }

        if ( bCountOnly == FALSE) {
            if ( !bBigEndian) {
                // リトルエンディアンにする処理
                cHigh = (pUcs2[nUcs2] & 0xff00) >> 8;
                cLow = (pUcs2[nUcs2] & 0x00ff);
                pUcs2[nUcs2] = (cLow << 8) | cHigh;
            }
        }

        nUcs2 += 1;
    }
    if ( bCountOnly == FALSE) {
        pUcs2[nUcs2] = L'\0';
    }

    return nUcs2;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                      その他の変換関数                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
#include <mbstring.h>

//###未検証
void sjis_to_jis(const uchar* src,uchar* dst)
{
	const uchar* p=src;
	uchar* q=dst;
	unsigned int c,c2;
	bool start=false;

	while(1){
		//開始ｺｰﾄﾞ挿入
		if(!start && _ismbblead(p[0])){
			start=true;
			*q++=0x1b;
			*q++=0x24;
			*q++=0x42;
			continue;
		}		
		//終了ｺｰﾄﾞ挿入
		if(start && !_ismbblead(p[0])){
			start=false;
			*q++=0x1b;
			*q++=0x28;
			*q++=0x42;
			continue;
		}
		if(start){
			//変換
			c=MAKEWORD(p[1],p[0]); p+=2;
			c2=_mbcjmstojis(c);
			*q++=(uchar)HIBYTE(c2);
			*q++=(uchar)LOBYTE(c2);
		}else{
			//変換なし
			*q++=*p++;
			if(q[-1]=='\0')break;
		}
	}
}

//###未検証
void jis_to_sjis(const uchar* src,uchar* dst)
{
	int i=0,iR=0;
	unsigned int c,c2;
	bool start=false; //JIS -> SJIS変換が必要かどうか
	const uchar* p=src;
	uchar* q=dst;

	//2ﾊﾞｲﾄ文字の開始
	uchar tb0[]={3,	0x1b,0x24,0x40};
	uchar tb1[]={3,	0x1b,0x24,0x42};
	uchar tb2[]={6,	0x1b,0x26,0x40,0x1b,0x24,0x42};
	uchar tb3[]={4,	0x1b,0x24,0x28,0x44};
	uchar* tb[]={tb0,tb1,tb2,tb3};
	//ASCII文字／JISﾛｰﾏ字の開始
	uchar as0[]={3,	0x1b,0x28,0x42};
	uchar as1[]={3,	0x1b,0x28,0x4a};
	uchar* as[]={as0,as1};

	while(1){
		//2ﾊﾞｲﾄ文字の開始
		for(int i=0;i<4;i++){
			uchar* t=tb[i];
			if(memcmp(p,&t[1],t[0])==0){
				start=true;
				p+=t[0];
				continue;
			}
		}
		//ASCII文字の開始
		for(int i=0;i<2;i++){
			uchar* t=as[i];
			if(memcmp(p,&t[1],t[0])==0){
				start=false;
				p+=t[0];
				continue;
			}
		}
		//JIS -> SJIS
		if(start && p[0]>=0x21 && p[0]<=0x7e){
			c=MAKEWORD(p[1],p[0]);
			c2=_mbcjistojms(c);
			q[0]=HIBYTE(c2);
			q[1]=LOBYTE(c2);
			p+=2;
			q+=2;
			continue;
		}
		//終端文字
		if(p[0]=='\0'){
			q[0]=p[0];
			break;
		}
		//ASCIIはそのまま
		*q++=*p++;
	}
}


//###未検証
void MySJisToJis(unsigned char *lpszOrg, unsigned char *lpszDest)
{
    int i = 0, iR = 0, c;
    BOOL bSTART = FALSE;

    while (1) {
        if (_ismbblead(lpszOrg[i]) && bSTART == FALSE) {
            lpszDest[iR] = 0x1b;
            lpszDest[iR + 1] = 0x24;
            lpszDest[iR + 2] = 0x42;
            c = MAKEWORD(lpszOrg[i + 1], lpszOrg[i]);
            lpszDest[iR + 3] = (unsigned char)HIBYTE(_mbcjmstojis(c));
            lpszDest[iR + 4] = (unsigned char)LOBYTE(_mbcjmstojis(c));
            bSTART = TRUE;
            i += 2;
            iR += 5;
            if (!_ismbblead(lpszOrg[i])) {
                lpszDest[iR] = 0x1b;
                lpszDest[iR + 1] = 0x28;
                lpszDest[iR + 2] = 0x42;
                bSTART = FALSE;
                iR += 3;
            }
            continue;
        }
        if (_ismbblead(lpszOrg[i]) && bSTART) {
            c = MAKEWORD(lpszOrg[i + 1], lpszOrg[i]);
            lpszDest[iR] = (unsigned char)HIBYTE(_mbcjmstojis(c));
            lpszDest[iR + 1] = (unsigned char)LOBYTE(_mbcjmstojis(c));
            i += 2;
            iR += 2;
            if (!_ismbblead(lpszOrg[i])) {
                lpszDest[iR] = 0x1b;
                lpszDest[iR + 1] = 0x28;
                lpszDest[iR + 2] = 0x42;
                bSTART = FALSE;
                iR += 3;
            }
            continue;
        }
        if (lpszOrg[i] == '\r') {
            lpszDest[iR] = '\n';
            i += 2;
            iR++;
            continue;
        }
        if (lpszOrg[i] == '\0') {
            lpszDest[iR] = lpszOrg[i];
            break;
        }
        lpszDest[iR] = lpszOrg[i];
        iR++;
        i++;
    }
    return;
}




// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         自前関数                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

std::wstring utf8_to_utf16(const char* src)
{
	size_t srclen=strlen(src);

	//必要なバッファ容量を調べる
	int dstlen=MultiByteToWideChar(
		CP_UTF8,
		0,
		src,
		(int)srclen,
		NULL,
		0
	);

	//バッファ確保
	std::wstring dst(dstlen,'X');

	//変換
	int ret=MultiByteToWideChar(
		CP_UTF8,
		0,
		src,
		(int)srclen,
		&dst[0],
		dstlen
	);

	return dst;
}

void utf8_to_utf16(const char* src,wchar_t* dst,size_t dst_count)
{
	size_t srclen=strlen(src);

	int ret=MultiByteToWideChar(
		CP_UTF8,
		0,
		src,
		(int)srclen,
		dst,
		(int)dst_count-1
	);
	dst[ret]=L'\0';
}
