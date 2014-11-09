#pragma once

#include "Graphics.h"
#include <windef.h>

#include "CDcPen.h"
#include "_fwd.h"
#include "CImage.h"


//�t�H���g
class FontInfo{
public:
	//!�t�H���g���ƃs�N�Z���T�C�Y�ŏ�����
	/*!
	@param face �t�H���g��
	@param size �S�p�̃s�N�Z���T�C�Y
	*/
	FontInfo(const wchar_t* face,const math::CBoxSize& size);

	//!�t�H���g���ƃ|�C���g�T�C�Y�ŏ�����
	/*!
	@param face       �t�H���g��
	@param point_size �|�C���g�T�C�Y
	*/
	FontInfo(const wchar_t* face,int point_size);

	const wchar_t* getFace() const{ return face.c_str(); } //!<�t�H���g��
	math::CBoxSize GetSize() const{ return size; }              //!<�S�p�ł̃s�N�Z���T�C�Y

	int _logW() const{ return log_w; }
	int _logH() const{ return log_h; }
private:
	std::wstring face;
	math::CBoxSize size; //�S�p�̃s�N�Z���T�C�Y
	int log_w;
	int log_h;
};

class CDcGraphics2 : public ::gr::Graphics, public CImage{
public:
	enum Type{
		TYPE_COMPATIBLE,
		TYPE_WRAP,
		TYPE_GETDC,
		TYPE_PAINT,
	};
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CDcGraphics2(const math::CBoxSize& _size,const wchar_t* model=L"DISPLAY");
	CDcGraphics2(const math::CBoxSize& _size,HDC hdcSample); //!< hdcSample�ƌ݊����̂��郁�����f�o�C�X�R���e�L�X�g���쐬����B
	CDcGraphics2(HDC _hdc,bool _hold);
	CDcGraphics2(HWND _hwnd);
	CDcGraphics2(HWND _hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
	virtual ~CDcGraphics2();

	//����
	//���_: �L�����o�X�̒��S
	//�w��: �E��������
	//�x��: ���������
	void SetMathViewport(const math::CBoxSize& size);

	//�͈�
	math::CBoxSize GetSize() const;
	//�t�H���g
	math::CBoxSize fontSize(const wchar_t* str,size_t str_len);
	void setFont(const FontInfo& font_info);

	//�`�摮��
	void SetColor(const CColor& color);
	bool setInvert(bool is_invert);
	Pen* getPen(){ return pen; }
	//�C���[�W
	void drawImage(const t_Point& ptDst,const CImage& imgSrc,const t_Rect& rcSrc);
	//�N���b�v
	void setClip(const t_Rect& rc);
	void removeClip();


	//�_
	void SetPixel(const t_Point& pt0);
	//����
	void DrawLine(const t_Point& pt0,const t_Point& pt1);
	//��`
	void FillRect(const t_Rect& rc);
	void drawRect(const t_Rect& rc);
	//�~
	void fillEllipse(const t_Rect& rc);
	void drawEllipse(const t_Rect& rc);
	//������
	void drawString(const wchar_t* str,const t_Point& pos);
	//###HDC
	void drawHDC(HDC hdc,const t_Point& dst,const t_Rect& src);


	//���b�p�[
	using Graphics::drawImage;
	// -- -- CImage���� -- -- //
	//�]��
	DcCarrier GetDcCarrier() const{ return DcCarrier(m_hdc); }

	// -- -- ���� -- -- //
	void maskCreate(int color);
	void maskCreate(const t_Point& pos);
	void maskDispose();
private:
	//����
	void _init_object();
	void _end_object();
public:
	//�J���p
	HDC _getHDC(){ return m_hdc; }
private:
	//�\��
	Type			type;
	HWND			hwnd;
	HDC				m_hdc;
	HBITMAP			hbmp;
	HBITMAP			hbmp_org;
	PAINTSTRUCT*	ps;
	bool			hold;
	math::CBoxSize			size;
	//�`��I�u�W�F�N�g
	CDcPen*			pen;
	HBRUSH			hbrSelect;
	HBRUSH			hbrOrg;
	HFONT			hfntSelect;
	HFONT			hfntOrg;
	CColor			m_cSelectedColor;
	//Transfer
//	DcTransfer* transfer;
	//�}�X�N
	HDC				hdcMask;
	HBITMAP			hbmpMask;
	HBITMAP			hbmpMask_org;
};

