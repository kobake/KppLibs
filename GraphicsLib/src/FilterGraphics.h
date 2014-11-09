#pragma once

#include "Graphics.h"

namespace gr{

class FilterGraphics : public Graphics{
public:
	//デバッグ時通知
	virtual void not_support(const wchar_t* method) const{ return g->not_support(method); }
public:
	//コンストラクタ・デストラクタ
	FilterGraphics(Graphics* _g) : g(_g) { }
	virtual ~FilterGraphics(){ }
	//範囲
	virtual math::CBoxSize GetSize() const{ return g->GetSize(); }
	//フォント
	virtual math::CBoxSize fontSize(const wchar_t* str,size_t str_len){ return g->fontSize(str,str_len); }
	virtual void setFont(const FontInfo& font_info){ return g->setFont(font_info); }
	//色
	virtual void SetColor(const CColor& color){ return g->SetColor(color); }
	//イメージ
	virtual void drawImage(const t_Point& ptDst,const gr::CImage& imgSrc,const t_Rect& rcSrc){ return g->drawImage(ptDst,imgSrc,rcSrc); }
	//クリップ
	virtual void setClip(const t_Rect& rc){ return g->setClip(rc); }
	virtual void removeClip(){ return g->removeClip(); }
	//直線
	virtual void DrawLine(const t_Point& pt0,const t_Point& pt1){ return g->DrawLine(pt0,pt1); }
	//矩形
	virtual void FillRect(const t_Rect& rc){ return g->FillRect(rc); }
	virtual void drawRect(const t_Rect& rc){ return g->drawRect(rc); }
	//円
	virtual void fillEllipse(const t_Rect& rc){ return g->fillEllipse(rc); }
	virtual void drawEllipse(const t_Rect& rc){ return g->drawEllipse(rc); }
	//文字列
	virtual void drawString(const wchar_t* str,const t_Point& pos){ return g->drawString(str,pos); }
	//###HDC
	virtual void drawHDC(HDC hdc,const t_Point& dst,const t_Rect& src){ return g->drawHDC(hdc,dst,src); }
private:
	Graphics* g;
};

} //namespace gr
