#include <BaseLib.h>
#include <windows.h>
#include "c_VirtualMap.h"

#if 0
//�`��
void drawMap(CDcGraphics *g,RECT *_rcUpdate,VirtualMap *map,int map_x,int map_y,const CColor& side_color)
{
	RECT rcUpdate=*_rcUpdate;
	int px=map->getPixelsPerX();
	int py=map->getPixelsPerY();
	rcUpdate.left=rcUpdate.left/px*px; //�J�艺��
	rcUpdate.right=(rcUpdate.right+px-1)/px*px; //�J��グ
	rcUpdate.top=rcUpdate.top/py*py; //�J�艺��
	rcUpdate.bottom=(rcUpdate.bottom+py-1)/py*py; //�J��グ
	::drawMap(g,rcUpdate.left,rcUpdate.top,rcUpdate.right-rcUpdate.left,rcUpdate.bottom-rcUpdate.top,
		map,map_x+rcUpdate.left/px,map_y+rcUpdate.top/py,side_color);
}

void drawMap(CDcGraphics *g,int _dst_x,int _dst_y,int _dst_w,int _dst_h,VirtualMap *map,int src_x,int src_y,const CColor& side_color)
{
	int dst_x=_dst_x;
	int dst_y=_dst_y;
	int dst_w=_dst_w;
	int dst_h=_dst_h;
	int src_w=dst_w/map->getPixelsPerX();
	int src_h=dst_h/map->getPixelsPerY();
	//�f�[�^�l �����I�[�o�[
	if(src_x<0){
		src_w-=-src_x; //�E�[��ێ������܂܁Asrc_x��0�ɋl�߂�
		dst_x+=-src_x*map->getPixelsPerX();
		src_x=0;
	}
	//�f�[�^�l �㑤�I�[�o�[
	if(src_y<0){
		src_h-=-src_y; //���[��ێ������܂܁Asrc_y��0�ɋl�߂�
		dst_y+=-src_y*map->getPixelsPerY();
		src_y=0;
	}
	//�f�[�^�l �E���I�[�o�[
	if(src_x+src_w>map->getDataWidth()){
		src_w=map->getDataWidth()-src_x; //���[��ێ������܂܁Asrc_w���l�߂�
	}
	//�f�[�^�l �����I�[�o�[
	if(src_y+src_h>map->getDataHeight()){
		src_h=map->getDataHeight()-src_y; //��[��ێ������܂܁Asrc_h���l�߂�
	}
	if(GetAsyncKeyState('A')){
		int n=0;
		n=1;
	}
	//�{�̕`��
	if(src_w>0 && src_h>0){
		map->draw(g,dst_x,dst_y,src_x,src_y,src_w,src_h);
	}else{
		src_w=src_h=0;
	}
	//�����`��
	dst_w=src_w*map->getPixelsPerX();
	dst_h=src_h*map->getPixelsPerY();
	CRect rcLeft(0,0,0,0),rcRight(0,0,0,0);
	CRect rcTop(0,0,0,0),rcBottom(0,0,0,0);
//	g->setBrushColor(side_color);
	g->SetColor(side_color);
	if(dst_x>_dst_x){
		//���̓h��Ԃ�
//		SetRect(&rcLeft,_dst_x,_dst_y,dst_x,_dst_y+_dst_h);
		rcLeft.set_ltrb(_dst_x,_dst_y,dst_x,_dst_y+_dst_h);
		g->FillRect(rcLeft);
	}
	if(dst_x+dst_w<_dst_x+_dst_w){
		//�E�̓h��Ԃ�
		rcRight.set_ltrb(dst_x+dst_w,_dst_y,_dst_x+_dst_w,_dst_y+_dst_h);
		g->FillRect(rcRight);
	}
	if(dst_y>_dst_y){
		//��̓h��Ԃ�
		rcTop.set_ltrb(_dst_x,_dst_y,_dst_x+_dst_w,dst_y);
		if(rcLeft.right()>0)rcTop.setLeft(rcLeft.right());
		if(rcRight.left()>0)rcTop.setRight(rcRight.left());
		g->FillRect(rcTop);
	}
	if(dst_y+dst_h<_dst_y+_dst_h){
		//���̓h��Ԃ�
		rcBottom.set_ltrb(_dst_x,dst_y+dst_h,_dst_x+_dst_w,_dst_y+_dst_h);
		if(rcLeft.right()>0)rcTop.setLeft(rcBottom.right());
		if(rcRight.left()>0)rcTop.setRight(rcBottom.left());
		g->FillRect(rcBottom);
	}

}
#endif
