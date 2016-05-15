class ColorBox;

#ifndef DEF_COLOR_BOX
#define DEF_COLOR_BOX

#include "c_Window.h"

class ColorBox : public Window{
protected:
//	HWND hwnd;
	COLORREF color;
//	WNDPROC WndProcDef;
	HBRUSH hbrColor;
public:
	ColorBox(int x,int y,int w,int h,Window *_parent,int _option=0,int _id=UNIQUE_ID);
	virtual ~ColorBox();
	virtual LRESULT onPaint(UINT msg,WPARAM wParam,LPARAM lParam);
	void SetColor(COLORREF _color,BOOL redraw);
	//friend LRESULT CALLBACK WndProcColorBox(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
public:
	//void move(int x,int y,int w,int h);
	void setFont(HFONT hFont);
};

#endif
