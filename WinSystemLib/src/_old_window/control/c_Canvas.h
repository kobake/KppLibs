class Canvas;

#ifndef _Canvas_
#define _Canvas_

#include "c_Window.h"
#include "c_Graphics.h"

typedef void (*MOUSEPROC)(Window*,UINT,int,int);

class Canvas : public Window{
private:
	PAINTPROC paintproc;
	MOUSEPROC mouseproc;
public:
	//コンストラクタ・デストラクタ
	void _init_v();
	Canvas(int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	~Canvas();
	//描画
	void setPaintProc(PAINTPROC _paintproc);
	PAINTPROC getPaintProc();
	virtual LRESULT onPaint(CDcGraphics *g);
	//
	void setMouseProc(MOUSEPROC _mouseproc);
	virtual LRESULT onMouse(UINT msg,WPARAM wParam,LPARAM lParam);
};

#endif
