#include "common/_required.h"
#include <windows.h>
#include "CDcGraphics2.h"
//#include "CImage.h"
//#include "DibTransfer.h"
//#include "DcTransfer.h"
#include "CFont.h"
#include <gr/Type.h> //WinSystemLib
#include <StringLib.h>
#include "CImage.h"
#include <Cast.h> //WinSystemLib
#include "std/myexception.h"
using namespace util;
using namespace math;

	//#########���������F������g���ƁASetPixel���g���Ȃ��Ȃ�H�H�H
void CDcGraphics2::SetMathViewport(const math::CBoxSize& size)
{
	::SetMapMode(m_hdc,MM_ANISOTROPIC);
	::SetWindowOrgEx(m_hdc,-size.w/2,size.h/2,NULL);
	::SetWindowExtEx(m_hdc,1,-1,NULL);
}

FontInfo::FontInfo(const wchar_t* _face,const math::CBoxSize& _size)
: face(_face), size(_size)
{
	log_w=size.w/2;
	log_h=size.h;
}

FontInfo::FontInfo(const wchar_t* _face,int point_size)
: face(_face)
{
	HDC hdc=GetDC(NULL);
	int pix=GetDeviceCaps(hdc,LOGPIXELSY);
	ReleaseDC(NULL,hdc);

	log_h=-MulDiv(point_size,pix,72);
	log_w=0;

	size.Assign(log_w>=0?log_w*2:log_h,log_h);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CDcGraphics2::CDcGraphics2(const math::CBoxSize& _size,const wchar_t* model)
: type(TYPE_COMPATIBLE), hwnd(NULL), m_hdc(NULL), hbmp(NULL), hbmp_org(NULL), ps(NULL), hold(true), size(_size)
{
	hwnd=NULL;
	hbmp=NULL;
	//
//	CreateDC(model,NULL,NULL,NULL);
	HDC hdcModel=GetDC(NULL);
	m_hdc=CreateCompatibleDC(hdcModel);
	hbmp=CreateCompatibleBitmap(hdcModel,size.w,size.h);
	hbmp_org=(HBITMAP)SelectObject(m_hdc,hbmp);
//	DeleteDC(hdcModel);
	ReleaseDC(NULL,hdcModel);
	hold=true;
	//
	//�����h��Ԃ�
	::FillRect(m_hdc,&Rect2RECT(math::CRect(size)),(HBRUSH)GetStockObject(BLACK_BRUSH));
	//
	_init_object();
}

CDcGraphics2::CDcGraphics2(const math::CBoxSize& _size,HDC hdcSample)
: type(TYPE_COMPATIBLE), hwnd(NULL), m_hdc(NULL), hbmp(NULL), hbmp_org(NULL), ps(NULL), hold(true), size(_size)
{
	hwnd=NULL;
	hbmp=NULL;
	//
	m_hdc=CreateCompatibleDC(hdcSample);
	hbmp=CreateCompatibleBitmap(hdcSample,size.w,size.h);
	hbmp_org=(HBITMAP)SelectObject(m_hdc,hbmp);
	hold=true;
	//
	//�����h��Ԃ�
	::FillRect(m_hdc,&Rect2RECT(math::CRect(size)),(HBRUSH)GetStockObject(BLACK_BRUSH));
	//
	_init_object();
}

CDcGraphics2::CDcGraphics2(HDC _hdc,bool _hold)
: type(TYPE_WRAP), hwnd(NULL), m_hdc(NULL), hbmp(NULL), hbmp_org(NULL), ps(NULL), hold(_hold), size(0,0)
{
	m_hdc=_hdc;
	hold=_hold;
	//�T�C�Y�擾
	size.Assign(GetDeviceCaps(m_hdc,HORZRES),GetDeviceCaps(m_hdc,VERTRES));
	//�I�u�W�F�N�g������
	_init_object();
}

CDcGraphics2::CDcGraphics2(HWND _hwnd)
: type(TYPE_GETDC), hwnd(NULL), m_hdc(NULL), hbmp(NULL), hbmp_org(NULL), ps(NULL), hold(true), size(0,0)
{
	hwnd=_hwnd;
	m_hdc=GetDC(hwnd);
	hold=true;
	//�T�C�Y�擾
	RECT wrc;
	GetClientRect(hwnd,&wrc);
	t_Rect rc=RECT2Rect(wrc);
	size = CBoxSize(rc);
	//�I�u�W�F�N�g������
	_init_object();
}

CDcGraphics2::CDcGraphics2(HWND _hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
: type(TYPE_PAINT), hwnd(NULL), m_hdc(NULL), hbmp(NULL), hbmp_org(NULL), ps(NULL), hold(true), size(0,0)
{
	hwnd=_hwnd;
	ps=new PAINTSTRUCT;
	m_hdc=BeginPaint(hwnd,ps);
	//�T�C�Y�擾
	RECT wrc;
	GetClientRect(hwnd,&wrc);
	t_Rect rc=RECT2Rect(wrc);
	size=CBoxSize(rc);
	//�����̈�擾
	setInvalidRect(RECT2Rect(ps->rcPaint));
	//�I�u�W�F�N�g������
	_init_object();
}

CDcGraphics2::~CDcGraphics2()
{
	_end_object();
	if(type==TYPE_COMPATIBLE){
		SelectObject(m_hdc,hbmp_org);
		DeleteObject(hbmp);
		DeleteDC(m_hdc);
	}else if(type==TYPE_WRAP){
		if(hold)DeleteDC(m_hdc);
	}else if(type==TYPE_GETDC){
		ReleaseDC(hwnd,m_hdc);
	}else if(type==TYPE_PAINT){
		EndPaint(hwnd,ps);
		delete ps;
	}
}

void CDcGraphics2::_init_object()
{
	SetBkMode(m_hdc,TRANSPARENT);
	//�쐬
	hbrSelect=(HBRUSH)GetStockObject(BLACK_BRUSH);
	hfntSelect=(HFONT)GetStockObject(SYSTEM_FONT);
	//�I��
	hbrOrg=(HBRUSH)SelectObject(m_hdc,hbrSelect);
	hfntOrg=(HFONT)SelectObject(m_hdc,hfntSelect);
	//�F
	m_cSelectedColor.set(255,255,255);
	//�}�X�N
	hdcMask=NULL;
	hbmpMask=NULL;
	hbmpMask_org=NULL;
	//Transfer
//	transfer=new DcTransfer(m_hdc);

	pen=new CDcPen(m_hdc);
}

void CDcGraphics2::_end_object()
{
	//Transfer
//	m_delete(transfer);
	//�I�u�W�F�N�g����
	SelectObject(m_hdc,hbrOrg);
	SelectObject(m_hdc,hfntOrg);
	//�I�u�W�F�N�g�j��
	DeleteObject(hbrSelect);
	DeleteObject(hfntSelect);
	//�}�X�N�j��
	maskDispose();

	m_delete(pen);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �͈�                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//���
CBoxSize CDcGraphics2::GetSize() const
{
	return size;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         �t�H���g                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CBoxSize CDcGraphics2::fontSize(const wchar_t* str,size_t str_len)
{
	SIZE size;
	GetTextExtentPoint32(m_hdc,str,(int)str_len,&size);
	return SIZE2Size(size);
}

void CDcGraphics2::setFont(const FontInfo& font_info)
{
	//��{�ް�
	LOGFONT lf;
	lf.lfHeight=font_info._logH();
	lf.lfWidth=font_info._logW();
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
	wcssafecpy(lf.lfFaceName,font_info.getFace(),countof(lf.lfFaceName));
	//�쐬
	HFONT hfntNew=CreateFontIndirect(&lf);
	//�I��
	SelectObject(m_hdc,hfntNew);
	DeleteObject(hfntSelect);
	hfntSelect=hfntNew;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         �`�摮��                            //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

#include "CColor.h"

void CDcGraphics2::SetColor(const CColor& color)
{
	COLORREF win_color=color.win_color();
	//�e�L�X�g
	SetTextColor(m_hdc,win_color);
	//�u���V
	HBRUSH hbrNew=CreateSolidBrush(win_color);
	SelectObject(m_hdc,hbrNew);
	DeleteObject(hbrSelect);
	hbrSelect=hbrNew;
	//�y��
	pen->SetColor(color);
	//�F
	m_cSelectedColor=color;
}

bool CDcGraphics2::setInvert(bool is_invert)
{
	int ret;
	if(is_invert){
		ret=SetROP2(m_hdc,R2_NOT);
	}else{
		ret=SetROP2(m_hdc,R2_COPYPEN);
	}
	if(ret==0){ return false; } //���s

	//�ȑO�̃��[�h��Ԃ�
	return (ret & R2_NOT) != 0;
}



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �C���[�W                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDcGraphics2::drawHDC(HDC _hdc,const t_Point& dst,const t_Rect& src)
{
	BitBlt(m_hdc,dst.x,dst.y,src.Width(),src.Height(),_hdc,src.Left(),src.Top(),SRCCOPY);
}

void CDcGraphics2::drawImage(const t_Point& ptDst,const CImage& imgSrc,const t_Rect& rcSrc)
{
	//DC�]�����s
	CImage::DcCarrier dc=imgSrc.GetDcCarrier();
	if(dc.isValid()){
		BitBlt(
			m_hdc,
			ptDst.x,ptDst.y,rcSrc.Width(),rcSrc.Height(),
			dc.hdc,
			rcSrc.Left(),rcSrc.Top(),
			SRCCOPY
		);
		return;
	}

	//CDib�]�����s
	CImage::DibCarrier dib=imgSrc.GetBitmapCarrier();
	if(dib.isValid()){
		StretchDIBits(
			m_hdc,
			ptDst.x,ptDst.y,rcSrc.Width(),rcSrc.Height(),
			rcSrc.Left(),rcSrc.Top(),rcSrc.Width(),rcSrc.Height(),
			dib.bits,
			dib.bmpinfo,
			DIB_RGB_COLORS,
			SRCCOPY
		);
		return;
	}

	//Bmp�]�����s
	CImage::BmpCarrier bmp=imgSrc.GetBmpCarrier();
	if(bmp.isValid()){
		//Bmp���i�[����������DC��p��
		HDC hdcSrc=CreateCompatibleDC(m_hdc);
		HBITMAP hbmpOld=(HBITMAP)SelectObject(hdcSrc,bmp.hbmp);

		//�]��
		BitBlt(
			m_hdc,
			ptDst.x,ptDst.y,rcSrc.Width(),rcSrc.Height(),
			hdcSrc,
			rcSrc.Left(),rcSrc.Top(),
			SRCCOPY
		);

		//������DC�j��
		SelectObject(hdcSrc,hbmpOld);
		DeleteDC(hdcSrc);
		return;
	}

	throw myexception(L"CDcGraphics2::drawImage failed");

	/*
	const ImageTransfer* trans=img->getTransfer();
	//
	// -- -- DibTransfer -- -- //
	const DibTransfer* dib=trans->getDibTransfer();
	if(dib){
		if(!dib->maskSupported()){
			//�ʏ�`��
			const BITMAPINFO* bmpinfo=dib->GetBitmapInfo();
			StretchDIBits(m_hdc,
				dst.x,dst.y,src.Width(),src.Height(), //dst
				src.x(),bmpinfo->bmiHeader.biHeight-src.bottom(),src.Width(),src.Height(),dib->GetBits(),bmpinfo, //src
				DIB_RGB_COLORS,SRCCOPY);
			return;
		}else{
			//�}�X�N
			const BITMAPINFO* bmpinfoMask=dib->getBitmapInfoMask();
			StretchDIBits(m_hdc,
				dst.x,dst.y,src.Width(),src.Height(), //dst
				src.x(),bmpinfoMask->bmiHeader.biHeight-src.bottom(),src.Width(),src.Height(),dib->getBitsMask(),bmpinfoMask, //src
				DIB_RGB_COLORS,SRCAND);
			//�{��
			const BITMAPINFO* bmpinfo=dib->GetBitmapInfo();
			StretchDIBits(m_hdc,
				dst.x,dst.y,src.Width(),src.Height(), //dst
				src.x(),bmpinfo->bmiHeader.biHeight-src.bottom(),src.Width(),src.Height(),dib->GetBits(),bmpinfo, //src
				DIB_RGB_COLORS,SRCPAINT);
			return;
		}
	}
	//
	// -- -- DcTransfer -- -- //
	const DcTransfer* dc=trans->getDcTransfer();
	if(dc){
		if(!dc->maskSupported()){
			//�ʏ�`��
			BitBlt(m_hdc,
				dst.x,dst.y,src.Width(),src.Height(), //dst
				dc->getHDC(),src.x(),src.y(), //src
				SRCCOPY);
		}else{
			//�F�ݒ�
			COLORREF text_org=SetTextColor(m_hdc,0x000000);
			COLORREF bk_org=SetBkColor(m_hdc,0xFFFFFF);
			//�}�X�N
			BitBlt(m_hdc,
				dst.x,dst.y,src.Width(),src.Height(), //dst
				dc->getMaskedHDC(),src.x(),src.y(), //src
				SRCAND);
			//�{��
			BitBlt(m_hdc,
				dst.x,dst.y,src.Width(),src.Height(), //dst
				dc->getHDC(),src.x(),src.y(), //src
				SRCPAINT);
			//�F�ݒ�
			SetTextColor(m_hdc,text_org);
			SetBkColor(m_hdc,bk_org);
		}
		return;
	}
	*/
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           �N���b�v                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDcGraphics2::setClip(const t_Rect& rc)
{
	HRGN hRgn=CreateRectRgnIndirect(&Rect2RECT(rc));
	SelectClipRgn(m_hdc,hRgn);
	DeleteObject(hRgn);
}

void CDcGraphics2::removeClip()
{
	SelectClipRgn(m_hdc,NULL);
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                             �_                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CDcGraphics2::SetPixel(const t_Point& pt)
{
	::SetPixel(m_hdc, pt.x, pt.y, m_cSelectedColor.win_color());
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                            ����                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDcGraphics2::DrawLine(const t_Point& pt0,const t_Point& pt1)
{
	MoveToEx(m_hdc,pt0.x,pt0.y,NULL);
	LineTo(m_hdc,pt1.x,pt1.y);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                            ��`                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDcGraphics2::FillRect(const t_Rect& rc)
{
	::FillRect(m_hdc,&Rect2RECT(rc),hbrSelect);
}

void CDcGraphics2::drawRect(const t_Rect& rc)
{
	SelectObject(m_hdc,GetStockObject(NULL_BRUSH));
	Rectangle(m_hdc,rc.Left(),rc.Top(),rc.Right(),rc.Bottom());
	SelectObject(m_hdc,hbrSelect);
}

	
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                            �~                               //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void CDcGraphics2::fillEllipse(const t_Rect& rc)
{
	Ellipse(m_hdc,rc.GetLeft(),rc.GetTop(),rc.GetRight(),rc.GetBottom());
}

void CDcGraphics2::drawEllipse(const t_Rect& rc)
{
	SelectObject(m_hdc,GetStockObject(NULL_BRUSH));
	Ellipse(m_hdc,rc.GetLeft(),rc.GetTop(),rc.GetRight(),rc.GetBottom());
	SelectObject(m_hdc,hbrSelect);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                          ������                             //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDcGraphics2::drawString(const wchar_t* str,const t_Point& pos)
{
	TextOut(m_hdc,pos.x,pos.y,str,(int)wcslen(str));
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                         CImage����                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


//�]��
/*
const ImageTransfer* CDcGraphics2::getTransfer() const
{
	return transfer;
}
*/

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           ����                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDcGraphics2::maskCreate(const t_Point& pos)
{
	/*
	maskCreate(CColor(GetPixel(m_hdc,pos.x,pos.y),Win).java_color());
	*/
}

void CDcGraphics2::maskCreate(int color)
{
	/*
	//�Â��}�X�N�j��
	maskDispose();

	//WIN�F�擾
	COLORREF win_color=CColor(color,Java).win_color();

	//�w�i�F�ꎞ�ύX
	COLORREF win_color_org=SetBkColor(m_hdc,win_color);
	
	//�}�X�N�쐬(�w�i���A������)
	math::CBoxSize size=GetSize();
	hdcMask=CreateCompatibleDC(m_hdc);
	hbmpMask=CreateBitmap(size.w,size.h,1,1,NULL);
	hbmpMask_org=(HBITMAP)SelectObject(hdcMask,hbmpMask);
	BitBlt(hdcMask,0,0,size.w,size.h,m_hdc,0,0,SRCCOPY);

	//�}�X�N�̔��]�C���[�W���쐬(�w�i���A������)
	HDC hdcWork=CreateCompatibleDC(m_hdc);
	HBITMAP hbmpWork=CreateBitmap(size.w,size.h,1,1,NULL);
	HBITMAP hbmpWork_org=(HBITMAP)SelectObject(hdcWork,hbmpWork);
	BitBlt(hdcWork,0,0,size.w,size.h,hdcMask,0,0,NOTSRCCOPY);

	//�w�i�F�߂�
	SetBkColor(m_hdc,win_color_org);

	//�{�̂̔w�i�����ɂ���
	BitBlt(m_hdc,0,0,size.w,size.h,hdcWork,0,0,SRCAND);

	//��n��
	SelectObject(hdcWork,hbmpWork_org);
	DeleteObject(hbmpWork);
	DeleteDC(hdcWork);

	//transfer
	delete transfer;
	transfer=new DcTransfer(m_hdc,hdcMask);
	*/
}

void CDcGraphics2::maskDispose()
{
	/*
	if(hdcMask!=NULL){
		SelectObject(hdcMask,hbmpMask_org);
		DeleteObject(hbmpMask);
		DeleteDC(hdcMask);
		hdcMask=NULL;
		hbmpMask=NULL;
		hbmpMask_org=NULL;
		//transfer���ւ�
		if(transfer){
			delete transfer;
			transfer=new DcTransfer(m_hdc);
		}
	}
	*/
}
