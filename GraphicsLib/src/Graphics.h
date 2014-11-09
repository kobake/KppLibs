#pragma once

#include <windows.h> //###
#include <vector>
#include <string>
#include "common/_required.h"
using namespace std;

#include "Type.h"
#include "Pen.h"
#include "_fwd.h"
class FontInfo;

namespace gr{

//����
enum Decoration{
	DEC_NORMAL	=(1<<6),
	DEC_BOLD	=(1<<7),
	DEC_AROUND	=(1<<8),
	DEC_SHADOW	=(1<<9),
};


class Graphics{
public:
	//�W���^�C�v
	typedef math::CVec2 t_Point;
	typedef math::RectBase<long> t_Rect;
public:
	//�f�o�b�O���ʒm
	virtual void not_support(const wchar_t* method) const;
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	Graphics();
	virtual ~Graphics(){ }
	//�͈�
	virtual math::CBoxSize GetSize() const{ not_support(L"GetSize"); return math::CBoxSize(0,0); }
	//�t�H���g
	virtual math::CBoxSize fontSize(const wchar_t* str,size_t str_len)				{ not_support(L"fontSize"); return math::CBoxSize(0,0); }
	virtual void setFont(const FontInfo& font_info)							{ not_support(L"setFont"); }
	//�`�摮��
	virtual void SetColor(const CColor& color)			{ not_support(L"SetColor");                } //!< �`��F��ݒ�
	virtual bool setInvert(bool is_invert)				{ not_support(L"setInvert"); return false; } //!< ���]������ݒ�
	virtual Pen* getPen()								{ not_support(L"getPen"); return NULL;     } //!< �y�����擾�A�ݒ�

	//�N���b�v
	virtual void setClip(const t_Rect& rc)									{ not_support(L"setClip"); }
	virtual void removeClip()												{ not_support(L"removeClip"); }

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                           �`��                              //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//�C���[�W
	virtual void drawImage(const t_Point& ptDst,const gr::CImage& imgSrc,const t_Rect& rcSrc)		{ not_support(L"drawImage"); }

	//�_
	virtual void SetPixel(const t_Point& pt0)								{ not_support(L"SetPixel"); }
	//����
	virtual void DrawLine(const t_Point& pt0,const t_Point& pt1)			{ not_support(L"DrawLine"); }
	//��`
	virtual void FillRect(const t_Rect& rc)									{ not_support(L"FillRect"); }
	virtual void drawRect(const t_Rect& rc)									{ not_support(L"drawRect"); }
	//�~
	virtual void fillEllipse(const t_Rect& rc)								{ not_support(L"fillEllipse"); }
	virtual void drawEllipse(const t_Rect& rc)								{ not_support(L"drawEllipse"); }
	//������
	virtual void drawString(const wchar_t* str,const t_Point& pos)				{ not_support(L"drawString"); }
	//###HDC
	virtual void drawHDC(HDC hdc,const t_Point& dst,const t_Rect& src)		{ not_support(L"drawHDC"); }

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//                         ���b�p�[                            //
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
	//
	//���b�p�[�ݒ�
	void setLineHeight(int _line_height);
	void setColor2(const CColor& color0,const CColor& color1);
	void setAreaWidth(int _width);
	//���b�p�[
	int fontHeight();
	int fontWidth(const wchar_t* str);
	math::CBoxSize fontSize(const wchar_t* str);
	void drawImage(t_Point& ptDst,const gr::CImage& imgSrc);
	void drawImage(const t_Point& ptDst,const gr::CImage& imgSrc,const t_Rect& ptSrc,const math::Align& align);
	//�����񃉃b�p�[
	void drawString(const wchar_t* str,const t_Point& pos,const math::Align& align);
	void drawStringEx(const wchar_t* str,const t_Point& pos,Decoration dec,const math::Align& align);
	void _drawStringDecImp(const wchar_t* str,const t_Point& pos,Decoration dec);
	void _drawStringAlignImp(const wchar_t* str,const t_Point& pos,int area_width,Decoration dec,const math::Align& align);
	vector<wstring> _splitString(const wchar_t* str,int area_width);


	//�ǉ�����
	const math::CRect& getInvalidRect(){ return rc_invalid; }
	void setInvalidRect(const math::CRect& rc){ rc_invalid=rc; }
private:
	CColor reserve_color[2];
	int line_height;
	int area_width;
	math::CRect rc_invalid;
};





} //namespace gr
