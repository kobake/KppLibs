#include <BaseLib.h>
#include "Sprite.h"

#if 0



// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//               �R���X�g���N�^�E�f�X�g���N�^                  //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
Sprite::Sprite() : TransBitmap256(1,1)
{
	table=NULL;
	ntable=0;
}
Sprite::~Sprite()
{
	if(table!=NULL){
		free(table);
		table=NULL;
	}
	ntable=0;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- �X�v���C�g�̒ǉ�   -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
int Sprite::putSprite(int x,int y,int w,int h)
{
	XYWH tmp={x,y,w,h};
	return putSprite(&tmp);
}
int Sprite::putSprite(XYWH *rc)
{
	ntable++;
	table=(XYWH*)realloc(table,sizeof(XYWH)*ntable);
	table[ntable-1]=*rc;
	return ntable-1;
}
void Sprite::putSpriteList(XYWH *spritedef)
{
	for(XYWH *p=spritedef;p->x!=-1;p++){
		putSprite(p);
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- �擾�i�g�p�j   -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
XYWH Sprite::getXYWH(int id)
{
	return table[id];
}
#endif
