#pragma once

#include <BaseLib.h>
#include "../../MathLib/MathLib.h"
#include <windows.h>


//!イメージクラス。
/*!
これを継承するクラスは、イメージ転送インターフェースを最低1つはオーバーライドする必要がある。
*/
class CImage{
public:
	//各種転送プロトコル
	struct DibCarrier{
		const BITMAPINFO* bmpinfo;
		const void* bits;

		DibCarrier() : bmpinfo(NULL), bits(NULL) { }
		DibCarrier(const BITMAPINFO* _bmpinfo,const void* _bits) : bmpinfo(_bmpinfo), bits(_bits) { }
		bool isValid() const{ return bmpinfo!=NULL && bits!=NULL; }
	};
	struct DcCarrier{
		HDC hdc;

		DcCarrier(){ hdc=NULL; }
		DcCarrier(HDC _hdc) : hdc(_hdc) { }
		bool isValid() const{ return hdc!=NULL; }
	};
	struct BmpCarrier{
		HBITMAP hbmp;

		BmpCarrier(){ hbmp=NULL; }
		BmpCarrier(HBITMAP _hbmp) : hbmp(_hbmp) { }
		bool isValid() const{ return hbmp!=NULL; }
	};
private:
	//代入禁止
	CImage(const CImage&);
	CImage& operator = (const CImage&);
public:
	//コンストラクタ・デストラクタ
	CImage(){}
	virtual ~CImage(){}
	//基本情報取得
	virtual math::CBoxSize GetSize() const=0;
	//イメージ転送インターフェース
	virtual DibCarrier GetBitmapCarrier()	const{ return DibCarrier(); }
	virtual DcCarrier GetDcCarrier()		const{ return DcCarrier(); }
	virtual BmpCarrier GetBmpCarrier()		const{ return BmpCarrier(); }
};

class DcImage{
};
