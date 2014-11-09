#pragma once

#include "Graphics.h"
#include <windef.h>

#include "CDcPen.h"
#include "_fwd.h"
#include "CImage.h"


//フォント
class FontInfo{
public:
	//!フォント名とピクセルサイズで初期化
	/*!
	@param face フォント名
	@param size 全角のピクセルサイズ
	*/
	FontInfo(const wchar_t* face,const math::CBoxSize& size);

	//!フォント名とポイントサイズで初期化
	/*!
	@param face       フォント名
	@param point_size ポイントサイズ
	*/
	FontInfo(const wchar_t* face,int point_size);

	const wchar_t* getFace() const{ return face.c_str(); } //!<フォント名
	math::CBoxSize GetSize() const{ return size; }              //!<全角でのピクセルサイズ

	int _logW() const{ return log_w; }
	int _logH() const{ return log_h; }
private:
	std::wstring face;
	math::CBoxSize size; //全角のピクセルサイズ
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
	//コンストラクタ・デストラクタ
	CDcGraphics2(const math::CBoxSize& _size,const wchar_t* model=L"DISPLAY");
	CDcGraphics2(const math::CBoxSize& _size,HDC hdcSample); //!< hdcSampleと互換性のあるメモリデバイスコンテキストを作成する。
	CDcGraphics2(HDC _hdc,bool _hold);
	CDcGraphics2(HWND _hwnd);
	CDcGraphics2(HWND _hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
	virtual ~CDcGraphics2();

	//特殊
	//原点: キャンバスの中心
	//Ｘ軸: 右方向が正
	//Ｙ軸: 上方向が正
	void SetMathViewport(const math::CBoxSize& size);

	//範囲
	math::CBoxSize GetSize() const;
	//フォント
	math::CBoxSize fontSize(const wchar_t* str,size_t str_len);
	void setFont(const FontInfo& font_info);

	//描画属性
	void SetColor(const CColor& color);
	bool setInvert(bool is_invert);
	Pen* getPen(){ return pen; }
	//イメージ
	void drawImage(const t_Point& ptDst,const CImage& imgSrc,const t_Rect& rcSrc);
	//クリップ
	void setClip(const t_Rect& rc);
	void removeClip();


	//点
	void SetPixel(const t_Point& pt0);
	//直線
	void DrawLine(const t_Point& pt0,const t_Point& pt1);
	//矩形
	void FillRect(const t_Rect& rc);
	void drawRect(const t_Rect& rc);
	//円
	void fillEllipse(const t_Rect& rc);
	void drawEllipse(const t_Rect& rc);
	//文字列
	void drawString(const wchar_t* str,const t_Point& pos);
	//###HDC
	void drawHDC(HDC hdc,const t_Point& dst,const t_Rect& src);


	//ラッパー
	using Graphics::drawImage;
	// -- -- CImage属性 -- -- //
	//転送
	DcCarrier GetDcCarrier() const{ return DcCarrier(m_hdc); }

	// -- -- 特殊 -- -- //
	void maskCreate(int color);
	void maskCreate(const t_Point& pos);
	void maskDispose();
private:
	//実装
	void _init_object();
	void _end_object();
public:
	//開発用
	HDC _getHDC(){ return m_hdc; }
private:
	//構造
	Type			type;
	HWND			hwnd;
	HDC				m_hdc;
	HBITMAP			hbmp;
	HBITMAP			hbmp_org;
	PAINTSTRUCT*	ps;
	bool			hold;
	math::CBoxSize			size;
	//描画オブジェクト
	CDcPen*			pen;
	HBRUSH			hbrSelect;
	HBRUSH			hbrOrg;
	HFONT			hfntSelect;
	HFONT			hfntOrg;
	CColor			m_cSelectedColor;
	//Transfer
//	DcTransfer* transfer;
	//マスク
	HDC				hdcMask;
	HBITMAP			hbmpMask;
	HBITMAP			hbmpMask_org;
};

