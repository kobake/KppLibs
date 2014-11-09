#include "charset.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>	// for mbstowcs(), wcstombs()
#include <locale.h>	// for setlocale()


//---------------------------------------------------------------------------
// ���[�J���֐�
//---------------------------------------------------------------------------
static int doconv(const char * src,char * dst,int dst_size,int stoutf);

//---------------------------------------------------------------------------
// SJIS2UTF8N()
//! NULL�I�[��SJIS�̕�������ANULL�I�[��UTF8N�ɕϊ�����
/*!
\param	src	[i]		�ϊ�����SJIS������|�C���^
\param	dst	[i/o]	�ϊ����UTF8N��Ԃ���̃|�C���^
\param	dst_size	[i]		wdst�̏���T�C�Y�B���̒l-1byte�܂ŋL�^�ł���B
						0���w�肵���ꍇ�Adst�͖�������ANULL�I�[���݂̕K�v�ȗe�ʂ��Ԃ�
\retval	1�ȏ�	�ϊ���̃o�C�g��
\retval	0		�G���[
\retval	-1		��ƃ������̊m�ێ��s
*/
//---------------------------------------------------------------------------
int SJIStoUTF8N(const char * src,char * dst,int dst_size)
{
	return doconv(src,dst,dst_size,1);
}

//---------------------------------------------------------------------------
// UTF8N2SJIS()
//! NULL�I�[��UTF8N�������SJIS�֕ϊ�����
/*!
\param	src	[i]		�ϊ�����UTF8N������|�C���^
\param	dst	[i/o]	�ϊ����SJIS��Ԃ���̃|�C���^
\param	dst_size	[i]		wdst�̏���T�C�Y�B���̒l-1byte�܂ŋL�^�ł���
						0���w�肵���ꍇ�Adst�͖�������ANULL�I�[���݂̕K�v�ȗe�ʂ��Ԃ�
\retval	0		�G���[
\retval	1�ȏ�	�ϊ���̃o�C�g��
\retval	-1		��ƃ������̊m�ێ��s
*/
//---------------------------------------------------------------------------
int UTF8NtoSJIS(const char * src,char * dst,int dst_size)
{
	return doconv(src,dst,dst_size,0);
}

//---------------------------------------------------------------------------
// doconv()
//! ���ۂ̕ϊ������B
/*!
\param	src	[i]		�ϊ����̕�����|�C���^
\param	dst	[i/o]	�ϊ���̕������Ԃ���̃|�C���^
\param	dst_size	[i]		wdst�̏���T�C�Y�B���̒l-1byte�܂ŋL�^�ł���
\retval	0		�G���[
\retval	1�ȏ�	�ϊ���̃o�C�g��
\retval	-1		��ƃ������̊m�ێ��s
*/
//---------------------------------------------------------------------------
int doconv(const char * src,char * dst,int dst_size,int stoutf)
{
	int i;
    int ires;
    LPWSTR wbuf;
    LPSTR  putf;

    // unicode�֕ϊ�
	//// ���O�`�F�b�N
	ires = MultiByteToWideChar(
		(stoutf!=0) ? CP_ACP : CP_UTF8,
        				// �����R�[�h
		0,				// �t���O�Ȃ�
		src,			// �ϊ���������
        -1,				// �ϊ�������o�C�g��
        NULL,			// �ϊ���i�[��
        0				// �i�[�̈�擾
		);
	if (ires == 0)
    {
    	return 0;
    }
    //// ���[�N�擾
    wbuf = new WCHAR[ires+1];
    if (wbuf == NULL)
    {
    	return -1;
    }
	//// ���ϊ�
	ires = MultiByteToWideChar(
		(stoutf!=0) ? CP_ACP : CP_UTF8,
        				// �����R�[�h
		0,				// �t���O�Ȃ�
		src,			// �ϊ���������
        -1,				// �ϊ�������o�C�g��
        wbuf,			// �ϊ���i�[��
        ires			// �i�[�̈�擾
		);
	wbuf[ires]  = 0;
    if (ires == 0)
    {
    	return 0;
    }

    // UTF8�ւ̕ϊ�
	//// ���O�`�F�b�N
	ires = WideCharToMultiByte(
		(stoutf==0) ? CP_ACP : CP_UTF8,
						// UTF8
		0,				// �t���O�Ȃ�
		wbuf,			// �ϊ���������
        -1,				// �ϊ�������o�C�g��
        NULL,			// �ϊ���i�[��
        0,				// �i�[�̈�
		NULL,NULL
		);
	if (ires == 0)
    {
		delete [] wbuf;
    	return 0;
    }
	// �e�ʃ`�F�b�N
    if (dst_size == 0)
    {
    	delete [] wbuf;
        return ires+1;
    }

    //// �̈�m��
    putf = new char[ires+1];
    if (putf == NULL)
    {
		delete [] wbuf;
		return -1;
    }
    //// ���ϊ�
	ires = WideCharToMultiByte(
		(stoutf==0) ? CP_ACP : CP_UTF8,
						// UTF8
		0,				// �t���O�Ȃ�
		wbuf,			// �ϊ���������
        -1,				// �ϊ�������o�C�g��
        putf,			// �ϊ���i�[��
        ires,			// �i�[�̈�
		NULL,NULL
		);
	if (ires == 0)
    {
	    delete [] wbuf;
    	delete [] putf;
        return 0;
    }

    // �R�s�[
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
 *  �֐���  �F  CUnicodeF::utf8_to_sjis
 *  �@�\�T�v�F  UTF-8 -> SJIS �֕�����̃R�[�h�ϊ�
 *  ����    �F  (I)  char *pUtf8Str - �ϊ���UTF-8������ւ̃|�C���^
 *              (O)  int *nBytesOut - �ϊ��㕶����̃o�C�g��
 *  �߂�l  �F  char* �ϊ���SJIS������ւ̃|�C���^
 *
 *  �����T�v�F
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
 *  �֐���  �F  CUnicodeF::sjis_to_utf8
 *  �@�\�T�v�F  SJIS -> UTF-8 �֕�����̃R�[�h�ϊ�
 *  ����    �F  (I)  char *pAnsiStr - �ϊ���SJIS������ւ̃|�C���^
 *              (O)  int *nBytesOut - �ϊ��㕶����̃o�C�g��
 *  �߂�l  �F  char* �ϊ���UTF-8������ւ̃|�C���^
 *
 *  �����T�v�F
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
 *  �֐���  �F  CUnicodeF::utf16be_to_sjis
 *  �@�\�T�v�F  UTF-16BE -> SJIS �֕�����̃R�[�h�ϊ�
 *  ����    �F  (I)  wchar_t *pUcsStr - �ϊ���UTF16BE������ւ̃|�C���^
 *              (O)  int *nBytesOut - �ϊ��㕶����̃o�C�g��
 *  �߂�l  �F  char* �ϊ���SJIS������ւ̃|�C���^
 *
 *  �����T�v�F
 * -----------------------------------------------------------------------
 */
char *CUnicodeF::utf16be_to_sjis(const wchar_t *pUcsStr, int *nBytesOut)
{
    char *pAnsiStr = NULL;
    int nLen;

    if (!pUcsStr) return NULL;

    setlocale(LC_ALL, "Japanese");// ���ꂪ�Ȃ���Unicode�ɕϊ�����Ȃ��I

    nLen = (int)wcslen( pUcsStr);

    if ( pUcsStr[0] == 0xfeff || pUcsStr[0] == 0xfffe) {
        pUcsStr++; // �擪��BOM(byte Order Mark)������΁C�X�L�b�v����
        nLen--;
    }

    pAnsiStr = (char *)calloc((nLen+1), sizeof(wchar_t));
    if (!pAnsiStr) return NULL;

    // 1�������ϊ�����B
    // �܂Ƃ߂ĕϊ�����ƁA�ϊ��s�\�����ւ̑Ή�������Ȃ̂�
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

        default: // �ϊ��s�\
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
 *  �֐���  �F  CUnicodeF::sjis_to_utf16be
 *  �@�\�T�v�F  SJIS -> UTF-16 �֕�����̃R�[�h�ϊ�
 *  ����    �F  (I)    char *pAnsiStr - �ϊ���SJIS������ւ̃|�C���^
 *              (O)    int *nBytesOut - �ϊ��㕶����̃o�C�g��
 *  �߂�l  �F  wchar_t* �ϊ���UTF-16BE������ւ̃|�C���^
 *
 *  �����T�v�F
 * -----------------------------------------------------------------------
 */
wchar_t *CUnicodeF::sjis_to_utf16be(const char *pAnsiStr, int *nBytesOut)
{
    int len;
    wchar_t *pUcsStr = NULL;

    if (!pAnsiStr) return NULL;

    setlocale(LC_ALL, "Japanese");  // ���ꂪ�Ȃ���Unicode�ɕϊ�����Ȃ��I

    len = (int)strlen( pAnsiStr);
    *nBytesOut = sizeof(wchar_t)*(len);

    pUcsStr = (wchar_t *)calloc(*nBytesOut + 2, 1);
    if (!pUcsStr) return NULL;

    mbstowcs(pUcsStr, pAnsiStr, len+1);

    return pUcsStr;
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CUnicodeF::utf16be_to_utf8
 *  �@�\�T�v�F  UTF-16 -> UTF-8 �֕�����̃R�[�h�ϊ�
 *  ����    �F  (I) wchar_t *pUcsStr - �ϊ���UTF-16BE������ւ̃|�C���^
 *              (O) int *nBytesOut - �ϊ��㕶����̃o�C�g��
 *  �߂�l  �F  char* �ϊ���UTF-8������ւ̃|�C���^
 *
 *  �����T�v�F
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

// Unicode(UTF-16) -> UTF-8 ������
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
                pUtf8[nUtf8] = ((pUcs2[nUcs2] & 0x07C0) >> 6 ) | 0xc0; // 2002.08.17 �C��
                pUtf8[nUtf8+1] = (pUcs2[nUcs2] & 0x003f) | 0x80;
            }
            nUtf8 += 2;
        } else {
            if ( bCountOnly == FALSE) {
                pUtf8[nUtf8] = ((pUcs2[nUcs2] & 0xf000) >> 12) | 0xe0; // 2002.08.04 �C��
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
 *  �֐���  �F  CUnicodeF::utf8_to_utf16be
 *  �@�\�T�v�F  UTF-8 -> UTF-16(BE/LE) �֕�����̃R�[�h�ϊ�
 *  ����    �F  (I) char *pUtf8Str - �ϊ���UTF-8������ւ̃|�C���^
 *              (O) int *nNumOut - �ϊ����ʂ�UTF-16�������DByte���ł͂Ȃ�
 *              (I) BOOL bBigEndian - �r�b�O�G���f�B�A���ɕϊ�����Ȃ�TRUE
 *
 *  �߂�l  �F  wchar_t* �ϊ��㕶����ւ̃|�C���^
 *
 *  �����T�v�F
 * -----------------------------------------------------------------------
 */
wchar_t *CUnicodeF::utf8_to_utf16be(const char *pUtf8Str, int *nNumOut, BOOL bBigEndian)
{
    int nUtf8Num;
    wchar_t *pUcsStr;

    nUtf8Num = (int)strlen(pUtf8Str); // UTF-8������ɂ́C'\0' ���Ȃ�
    *nNumOut = utf8_to_utf16be_sub( NULL, pUtf8Str, nUtf8Num, TRUE, bBigEndian);

    pUcsStr = (wchar_t *)calloc((*nNumOut + 1), sizeof(wchar_t));
    utf8_to_utf16be_sub( pUcsStr, pUtf8Str, nUtf8Num, FALSE, bBigEndian);

    return pUcsStr;
}

// UTF-8 -> Unicode(UCS-2) ������
int CUnicodeF::utf8_to_utf16be_sub( wchar_t *pUcs2, const char *pUtf8, int nUtf8Num,
                          BOOL bCountOnly, BOOL bBigEndian)
{
    int nUtf8, nUcs2 = 0;
    char cHigh, cLow;

    for ( nUtf8=0; nUtf8 < nUtf8Num;) {
        if ( ( pUtf8[nUtf8] & 0x80) == 0x00) { // �ŏ�ʃr�b�g = 0
            if ( bCountOnly == FALSE) {
                pUcs2[nUcs2] = ( pUtf8[nUtf8] & 0x7f);
            }
            nUtf8 += 1;
        } else if ( ( pUtf8[nUtf8] & 0xe0) == 0xc0) { // ���3�r�b�g = 110
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
                // ���g���G���f�B�A���ɂ��鏈��
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
//                      ���̑��̕ϊ��֐�                       //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
#include <mbstring.h>

//###������
void sjis_to_jis(const uchar* src,uchar* dst)
{
	const uchar* p=src;
	uchar* q=dst;
	unsigned int c,c2;
	bool start=false;

	while(1){
		//�J�n���ޑ}��
		if(!start && _ismbblead(p[0])){
			start=true;
			*q++=0x1b;
			*q++=0x24;
			*q++=0x42;
			continue;
		}		
		//�I�����ޑ}��
		if(start && !_ismbblead(p[0])){
			start=false;
			*q++=0x1b;
			*q++=0x28;
			*q++=0x42;
			continue;
		}
		if(start){
			//�ϊ�
			c=MAKEWORD(p[1],p[0]); p+=2;
			c2=_mbcjmstojis(c);
			*q++=(uchar)HIBYTE(c2);
			*q++=(uchar)LOBYTE(c2);
		}else{
			//�ϊ��Ȃ�
			*q++=*p++;
			if(q[-1]=='\0')break;
		}
	}
}

//###������
void jis_to_sjis(const uchar* src,uchar* dst)
{
	int i=0,iR=0;
	unsigned int c,c2;
	bool start=false; //JIS -> SJIS�ϊ����K�v���ǂ���
	const uchar* p=src;
	uchar* q=dst;

	//2�޲ĕ����̊J�n
	uchar tb0[]={3,	0x1b,0x24,0x40};
	uchar tb1[]={3,	0x1b,0x24,0x42};
	uchar tb2[]={6,	0x1b,0x26,0x40,0x1b,0x24,0x42};
	uchar tb3[]={4,	0x1b,0x24,0x28,0x44};
	uchar* tb[]={tb0,tb1,tb2,tb3};
	//ASCII�����^JIS۰ώ��̊J�n
	uchar as0[]={3,	0x1b,0x28,0x42};
	uchar as1[]={3,	0x1b,0x28,0x4a};
	uchar* as[]={as0,as1};

	while(1){
		//2�޲ĕ����̊J�n
		for(int i=0;i<4;i++){
			uchar* t=tb[i];
			if(memcmp(p,&t[1],t[0])==0){
				start=true;
				p+=t[0];
				continue;
			}
		}
		//ASCII�����̊J�n
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
		//�I�[����
		if(p[0]=='\0'){
			q[0]=p[0];
			break;
		}
		//ASCII�͂��̂܂�
		*q++=*p++;
	}
}


//###������
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
//                         ���O�֐�                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

std::wstring utf8_to_utf16(const char* src)
{
	size_t srclen=strlen(src);

	//�K�v�ȃo�b�t�@�e�ʂ𒲂ׂ�
	int dstlen=MultiByteToWideChar(
		CP_UTF8,
		0,
		src,
		(int)srclen,
		NULL,
		0
	);

	//�o�b�t�@�m��
	std::wstring dst(dstlen,'X');

	//�ϊ�
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
