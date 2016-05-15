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
// -- -- -- -- -- -- -- -- -- -- --  �R���X�g���N�^�E�f�X�g���N�^  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void Font::_init_v()
{
	//��{�f�[�^
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
	//�I�v�V����
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
	//�쐬
	hFont=CreateFontIndirect(&lf);
}

//�T�C�Y���|�C���g�Ŏw��
Font::Font(const wchar *_face,int _point,int _option)
{
	//��{�f�[�^�R�s�[
	wcssafecpy(face,_face,LF_FACESIZE);
	option=_option;
	//�T�C�Y�Z�o
	point=_point;
	height=point_to_height(point);
	width=0;
	//�쐬
	_init_v();
}

//�S�p�̃T�C�Y���s�N�Z���Ŏw��
Font::Font(const wchar *_face,int _height,int _width,int _option)
{
	//��{�f�[�^�R�s�[
	wcssafecpy(face,_face,LF_FACESIZE);
	option=_option;
	//�T�C�Y�Z�o
	height=_height;
	width=_width;
	point=height_to_point(height);
	//�쐬
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

//### hdc��ÓI�ɂ����ق������������B���̏ꍇ�͉𑜓x�ς�����Ƃ��Ȃ�hdc�擾���Ȃ���
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
