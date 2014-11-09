#pragma once

#include "Graphics.h"

namespace gr{

class FilterGraphics : public Graphics{
public:
	//�f�o�b�O���ʒm
	virtual void not_support(const wchar_t* method) const{ return g->not_support(method); }
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	FilterGraphics(Graphics* _g) : g(_g) { }
	virtual ~FilterGraphics(){ }
	//�͈�
	virtual math::CBoxSize GetSize() const{ return g->GetSize(); }
	//�t�H���g
	virtual math::CBoxSize fontSize(const wchar_t* str,size_t str_len){ return g->fontSize(str,str_len); }
	virtual void setFont(const FontInfo& font_info){ return g->setFont(font_info); }
	//�F
	virtual void SetColor(const CColor& color){ return g->SetColor(color); }
	//�C���[�W
	virtual void drawImage(const t_Point& ptDst,const gr::CImage& imgSrc,const t_Rect& rcSrc){ return g->drawImage(ptDst,imgSrc,rcSrc); }
	//�N���b�v
	virtual void setClip(const t_Rect& rc){ return g->setClip(rc); }
	virtual void removeClip(){ return g->removeClip(); }
	//����
	virtual void DrawLine(const t_Point& pt0,const t_Point& pt1){ return g->DrawLine(pt0,pt1); }
	//��`
	virtual void FillRect(const t_Rect& rc){ return g->FillRect(rc); }
	virtual void drawRect(const t_Rect& rc){ return g->drawRect(rc); }
	//�~
	virtual void fillEllipse(const t_Rect& rc){ return g->fillEllipse(rc); }
	virtual void drawEllipse(const t_Rect& rc){ return g->drawEllipse(rc); }
	//������
	virtual void drawString(const wchar_t* str,const t_Point& pos){ return g->drawString(str,pos); }
	//###HDC
	virtual void drawHDC(HDC hdc,const t_Point& dst,const t_Rect& src){ return g->drawHDC(hdc,dst,src); }
private:
	Graphics* g;
};

} //namespace gr
