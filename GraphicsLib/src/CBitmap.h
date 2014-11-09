#pragma once

#include <windows.h> //HBITMAP
#include "CImage.h"
#include <MathLib.h> //BoxSize

class CBitmap : public CImage{
public:
	//コンストラクタ・デストラクタ
	CBitmap(HINSTANCE hInst,int id);
	CBitmap(HBITMAP _hBitmap,bool _hold);
	CBitmap();
	virtual ~CBitmap();
	//インターフェース
	HBITMAP getHBITMAP(){ return m_hbmp; }

	// -- -- CImageインターフェース -- -- //
	//基本情報取得
	math::CBoxSize GetSize() const{ return m_size; }
	//イメージ転送インターフェース
	BmpCarrier GetBmpCarrier() const{ return BmpCarrier(m_hbmp); }

protected:
	//実装補助
	void _calcSize(); //!< サイズを取得して保持
private:
	HBITMAP m_hbmp;
	math::CBoxSize m_size;
	bool m_hold;
};




