#pragma once

#include "FilterGraphics.h"
#include "Type.h"

namespace gr{

class OffsetGraphics : public FilterGraphics{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	OffsetGraphics(Graphics* g,const t_Point& off=t_Point(0,0)) : FilterGraphics(g), offset(off) { }
	virtual ~OffsetGraphics(){ }
	//�ŗL�C���^�[�t�F�[�X
	void setOffset(const t_Point& off){ offset=off; }
	t_Point getOffset() const{ return offset; }
	//�C���[�W
	virtual void drawImage(const t_Point& ptDst,const gr::CImage& imgSrc,const t_Rect& rcSrc){ return FilterGraphics::drawImage(ptDst+offset,imgSrc,rcSrc); }
	//�N���b�v
	virtual void setClip(const t_Rect& rc){ return FilterGraphics::setClip(rc+offset); }
	//����
	virtual void DrawLine(const t_Point& pt0,const t_Point& pt1){ return FilterGraphics::DrawLine(pt0+offset,pt1+offset); }
	//��`
	virtual void FillRect(const t_Rect& rc){ return FilterGraphics::FillRect(rc+offset); }
	virtual void drawRect(const t_Rect& rc){ return FilterGraphics::drawRect(rc+offset); }
	//�~
	virtual void fillEllipse(const t_Rect& rc){ return FilterGraphics::fillEllipse(rc+offset); }
	virtual void drawEllipse(const t_Rect& rc){ return FilterGraphics::drawEllipse(rc+offset); }
	//������
	virtual void drawString(const wchar_t* str,const t_Point& pos){ return FilterGraphics::drawString(str,pos+offset); }
	//##HDC
	virtual void drawHDC(HDC hdc,const t_Point& dst,const t_Rect& src){ return FilterGraphics::drawHDC(hdc,dst,src); }
private:
	t_Point offset;
};


} //namespace gr
