#pragma once

#include "FilterGraphics.h"
#include "Type.h"

namespace gr{

class OffsetGraphics : public FilterGraphics{
public:
	//コンストラクタ・デストラクタ
	OffsetGraphics(Graphics* g,const t_Point& off=t_Point(0,0)) : FilterGraphics(g), offset(off) { }
	virtual ~OffsetGraphics(){ }
	//固有インターフェース
	void setOffset(const t_Point& off){ offset=off; }
	t_Point getOffset() const{ return offset; }
	//イメージ
	virtual void drawImage(const t_Point& ptDst,const gr::CImage& imgSrc,const t_Rect& rcSrc){ return FilterGraphics::drawImage(ptDst+offset,imgSrc,rcSrc); }
	//クリップ
	virtual void setClip(const t_Rect& rc){ return FilterGraphics::setClip(rc+offset); }
	//直線
	virtual void DrawLine(const t_Point& pt0,const t_Point& pt1){ return FilterGraphics::DrawLine(pt0+offset,pt1+offset); }
	//矩形
	virtual void FillRect(const t_Rect& rc){ return FilterGraphics::FillRect(rc+offset); }
	virtual void drawRect(const t_Rect& rc){ return FilterGraphics::drawRect(rc+offset); }
	//円
	virtual void fillEllipse(const t_Rect& rc){ return FilterGraphics::fillEllipse(rc+offset); }
	virtual void drawEllipse(const t_Rect& rc){ return FilterGraphics::drawEllipse(rc+offset); }
	//文字列
	virtual void drawString(const wchar_t* str,const t_Point& pos){ return FilterGraphics::drawString(str,pos+offset); }
	//##HDC
	virtual void drawHDC(HDC hdc,const t_Point& dst,const t_Rect& src){ return FilterGraphics::drawHDC(hdc,dst,src); }
private:
	t_Point offset;
};


} //namespace gr
