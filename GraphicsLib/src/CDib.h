#pragma once

#include <windows.h>
#include <GraphicsLib.h> // Type
#include "std/myexception.h"
#include "CImage.h"
#include "sdk/libfwd_win.h"
#include <IoLib.h> //InputData
#include "std/mem.h"

class CColor;

class CDib : public CImage, public InputData{
public:
	//例外
	class DibException : public myexception{
	public:
		DibException(const wchar_t* msg) : myexception(msg) { }
	};
	class NotDib : public DibException{
	public:
		NotDib(const wchar_t* msg) : DibException(msg) { }
	};
private:
	//コピー禁止
	CDib(const CDib& rhs);
	CDib& operator = (const CDib& rhs);
private:
	//実装補助
	void _construct(const math::CBoxSize& _size,int depth,int size_image);
	void _destroy();
public:
	//コンストラクタ・デストラクタ
	CDib();
	CDib(const BITMAPINFO& bmpinfo, const void* bits);
	CDib(const math::CBoxSize& _size,int depth);
	CDib(const wchar_t* path);
	virtual ~CDib();

	//コピー
	CDib* Clone() const;

	//代入
	void Assign(const BITMAPINFO& bmpinfo, const void* bits);
	void Assign(HDC hdc);

	//変換
	void Resize(const math::CBoxSize& new_size);

	//描画
	void DrawTo(HDC hdc, const math::CPoint& ptDst);

	//色属性
	int GetBitsPerPixel() const{ return m_bmpinfo->bmiHeader.biBitCount; }
	int GetPaletteCount() const;

	//ストリーム
	void Read(CInputStream& in);

	//転送
	void SetBits(const math::CPoint& ptDst,const CDib& dibSrc,const math::CRect& rcSrc);
	void SetPixel(const math::CPoint& pt,const CColor& color);
	CColor GetPixel(const math::CPoint& pt) const;

	//CImageインターフェース
	math::CBoxSize GetSize() const;
	DibCarrier GetBitmapCarrier() const{ return DibCarrier(m_bmpinfo,m_bits); }

	//実装インターフェース
	const tagBITMAPINFO*	GetBitmapInfo() const{ return m_bmpinfo; }
	size_t					GetBitmapInfoSize() const;
	const void*				GetBits() const{ return m_bits; }
	size_t					GetBitsSize() const;
private:
	int			m_wbytes;
	BITMAPINFO*	m_bmpinfo;
	byte*		m_bits;
};

