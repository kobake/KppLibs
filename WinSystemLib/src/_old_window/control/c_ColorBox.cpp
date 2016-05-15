#include <windows.h>
#include <string>
using namespace std;
#include "common/_required.h"
#include "c_ColorBox.h"
#include "f_window.h"
#include "c_App.h"
#include "id/class.h"
/*
LRESULT CALLBACK WndProcColorBox(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	ColorBox *colorbox=(ColorBox*)GetWindowLong(hwnd,GWL_USERDATA);
	HDC hdc; PAINTSTRUCT ps;
	switch(msg){
	case WM_PAINT:
		hdc=BeginPaint(hwnd,&ps);
		FillRect(hdc,&ps.rcPaint,colorbox->hbrColor);
		EndPaint(hwnd,&ps);
		return 0L;
	}
	return CallWindowProc(colorbox->WndProcDef,hwnd,msg,wParam,lParam);
}
*/

LRESULT ColorBox::onPaint(UINT msg,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc=BeginPaint(hwnd,&ps);
	FillRect(hdc,&ps.rcPaint,hbrColor);
	EndPaint(hwnd,&ps);
	return 0L;
}

ColorBox::ColorBox(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_COLOR_BOX,0,_parent,_option | WINDOWF_TABSTOP,_id)
{
	convertXYWH(&x,&y,&w,&h);
	hbrColor=NULL;
	SetColor(RGB(0,0,0),FALSE);
	hwnd=CreateWindowEx(WS_EX_CLIENTEDGE,L"STATIC",L"",WS_VISIBLE | WS_CHILD,x,y,w,h,getParent()->getHWND(),(HMENU)getID(),app->getInstance(),NULL);
	_afterCreate();
	messageNotify(true);
	//WndProcDef=(WNDPROC)SetWindowLong(hwnd,GWL_WNDPROC,(LONG)WndProcColorBox);
}

ColorBox::~ColorBox()
{
	DeleteObject(hbrColor);
	//SetWindowLong(hwnd,GWL_WNDPROC,(LONG)WndProcDef);
	//DestroyWindow(hwnd);
}

void ColorBox::SetColor(COLORREF _color,BOOL redraw)
{
	DeleteObject(hbrColor);
	color=_color;
	hbrColor=CreateSolidBrush(color);
	if(redraw)
		repaint(false);
}
