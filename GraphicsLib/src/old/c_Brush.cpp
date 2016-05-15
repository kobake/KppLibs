#include <windows.h>
#include "c_Brush.h"

static int brush_stock_table[]=BRUSH_STOCK_TABLE;

void Brush::_init_v()
{
	if(!brush_param.isStock()){
		UINT style=PS_SOLID;
		COLORREF color=0x000000;
		LONG hatch=0;
		//スタイル
		int f=brush_param.flag;
		if(f==BRUSH_HATCHED){
			style=BS_HATCHED;
			hatch=HS_CROSS;
			/* ###
			HS_BDIAGONAL
			HS_CROSS
			HS_DIAGCROSS
			HS_FDIAGONAL
			HS_HORIZONTAL
			HS_VERTICAL
			*/
		}else if(f==BRUSH_NULL){
			style=BS_NULL;
		}else{
			style=BS_SOLID;
		}
		//色
		color=brush_param.color;
		//作成
		LOGBRUSH lb;
		lb.lbStyle=style;
		lb.lbColor=color;
		lb.lbHatch=hatch;
		hBrush=CreateBrushIndirect(&lb);
	}else{
		int stock=brush_param.stock;
		if(stock<1 || stock>=(sizeof(brush_stock_table)/sizeof(int)))stock=brush_param.stock=1;
		//作成
		hBrush=(HBRUSH)GetStockObject(brush_stock_table[stock]);
	}
}


Brush::Brush(int color,int flag)
{
	brush_param.set(color,flag);
	_init_v();
}
Brush::Brush(int stock)
{
	brush_param.set(stock);
	_init_v();
}
Brush::Brush(BRUSH_PARAM *_brush_param)
{
	brush_param=*_brush_param;
	_init_v();
}
Brush::Brush(Brush &right)
{
	brush_param=right.brush_param;
	_init_v();
}
Brush *Brush::clone()
{
	return new Brush(*this);
}
Brush::~Brush()
{
	if(hBrush!=NULL){
		if(!brush_param.isStock()){
			DeleteObject(hBrush);
		}
		hBrush=NULL;
	}
}


bool Brush::eqParam(BRUSH_PARAM *_brush_param)
{
	return memcmp(&brush_param,_brush_param,sizeof(BRUSH_PARAM))==0;
}
