#include <windows.h>
#include "common/_required.h"
#include "c_Font.h"
#include "f_drawobject.h"
#include <StringLib.h>
using namespace util;

static int point_to_height(int point)
{
	int height;
	HDC hdc=GetDC(NULL);
	height=-MulDiv(point,GetDeviceCaps(hdc,LOGPIXELSY),72);
	ReleaseDC(NULL,hdc);
	return height;
}

static int height_to_point(int height)
{
	int point;
	HDC hdc=GetDC(NULL);
	point=abs(-MulDiv(height,72,GetDeviceCaps(hdc,LOGPIXELSY)));
	ReleaseDC(NULL,hdc);
	return point;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void Font::_init_v()
{
	//基本データ
	LOGFONT lf;
	lf.lfHeight=height;
	lf.lfWidth=width/2;
	lf.lfEscapement=0;
	lf.lfOrientation=0;
	lf.lfWeight=FW_NORMAL;
	lf.lfItalic=0;
	lf.lfUnderline=0;
	lf.lfStrikeOut=0;
	lf.lfCharSet=SHIFTJIS_CHARSET;
	lf.lfOutPrecision=OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lf.lfQuality=DEFAULT_QUALITY;
	lf.lfPitchAndFamily=FIXED_PITCH;
	wcssafecpy(lf.lfFaceName,face,LF_FACESIZE);
	//オプション
	if(option & FONT_BOLD){
		lf.lfWeight=FW_BOLD;
	}
	if(option & FONT_ITALIC){
		lf.lfItalic=TRUE;
	}
	if(option & FONT_UNDERLINE){
		lf.lfUnderline=TRUE;
	}
	if(option & FONT_STRIKE){
		lf.lfStrikeOut=TRUE;
	}
	//作成
	hFont=CreateFontIndirect(&lf);
}

//サイズをポイントで指定
Font::Font(const wchar *_face,int _point,int _option)
{
	//基本データコピー
	wcssafecpy(face,_face,LF_FACESIZE);
	option=_option;
	//サイズ算出
	point=_point;
	height=point_to_height(point);
	width=0;
	//作成
	_init_v();
}

//全角のサイズをピクセルで指定
Font::Font(const wchar *_face,int _height,int _width,int _option)
{
	//基本データコピー
	wcssafecpy(face,_face,LF_FACESIZE);
	option=_option;
	//サイズ算出
	height=_height;
	width=_width;
	point=height_to_point(height);
	//作成
	_init_v();
}

Font::~Font()
{
	DeleteObject(hFont);
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

Font::operator HFONT() const
{
	return hFont;
}

//### hdcを静的にしたほうが効率いい。その場合は解像度変わったときなどhdc取得しなおし
int Font::getTextWidth(wchar *str)
{
	SIZE ret={0,0};
	HDC hdc=GetDC(NULL);
	SelectObject(hdc,hFont);
	GetTextExtentPoint32(hdc,str,wcslen(str),&ret);
	ReleaseDC(NULL,hdc);
	return ret.cx;
}







HFONT CreateSizeFont(wchar *face,int fh,int fw)
{
	LOGFONT lf;
	lf.lfHeight=fh;
	lf.lfWidth=fw;
	lf.lfEscapement=0;
	lf.lfOrientation=0;
	lf.lfWeight=FW_NORMAL;
	lf.lfItalic=0;
	lf.lfUnderline=0;
	lf.lfStrikeOut=0;
	lf.lfCharSet=SHIFTJIS_CHARSET;
	lf.lfOutPrecision=OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	lf.lfQuality=DEFAULT_QUALITY;
	lf.lfPitchAndFamily=FIXED_PITCH;
	wcssafecpy(lf.lfFaceName,face,LF_FACESIZE);
	return CreateFontIndirect(&lf);
}

HFONT CreatePointFont(wchar *face,int point)
{
	HDC hdc=GetDC(NULL);
	int h=-MulDiv(point, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC(NULL,hdc);
	return CreateSizeFont(face,h,0);
}
