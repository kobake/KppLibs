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
	//�R���X�g���N�^�E�f�X�g���N�^
	Sprite();
	virtual ~Sprite();
	//�X�v���C�g�̒ǉ�
	int putSprite(int x,int y,int w,int h);
	int putSprite(XYWH *rc);
	void putSpriteList(XYWH *spritedef);
	//�擾�i�g�p�j
	XYWH getXYWH(int id);
protected:
	XYWH *table;
	int ntable;
};


#endif
