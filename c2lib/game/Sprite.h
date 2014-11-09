#pragma once

#if 0

struct XYWH{
	int x;
	int y;
	int w;
	int h;
};

class Sprite : public TransBitmap256{
public:
	//コンストラクタ・デストラクタ
	Sprite();
	virtual ~Sprite();
	//スプライトの追加
	int putSprite(int x,int y,int w,int h);
	int putSprite(XYWH *rc);
	void putSpriteList(XYWH *spritedef);
	//取得（使用）
	XYWH getXYWH(int id);
protected:
	XYWH *table;
	int ntable;
};


#endif
