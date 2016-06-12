#include <BaseLib.h>
#include <windows.h>
#include <string>
using namespace std;
#include "c_Canvas.h"
#include "../custom/c_CustomWindow.h"
#include "../../_old_app/c_App.h"


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void Canvas::_init_v()
{
	paintproc=NULL;
	mouseproc=NULL;
}

Canvas::Canvas(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_CANVAS,0,_parent,_option | WINDOWF_TABSTOP,_id)
{
	_init_v();
	convertXYWH(&x,&y,&w,&h);

	//ウィンドウクラスの登録
	if(app->getPrevInstance()==NULL){
		WNDCLASSEX wndclass;
		wndclass.cbSize=sizeof(WNDCLASSEX);
		wndclass.style=CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc=DefWindowProc;
		wndclass.cbClsExtra=0;
		wndclass.cbWndExtra=0;
		wndclass.hInstance=app->getInstance();
		wndclass.hIcon=NULL;
		wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
		wndclass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
		wndclass.lpszMenuName=NULL;
		wndclass.lpszClassName=L"CustomWindow";
		wndclass.hIconSm=NULL;
		RegisterClassEx(&wndclass);
	}
	//スタイルの算出
	int kind=CUSTOMWINDOWF_KIND(_option);
	DWORD ex_style=0;
	DWORD style=0;
	//
	style|=GET_WIN_STYLE(_option);
	ex_style|=GET_WIN_EX_STYLE(_option);
	//作成
	hwnd=CreateWindowEx(ex_style,L"CustomWindow",L"",WS_VISIBLE | WS_CHILD | style,x,y,w,h,getParent()->getHWND(),NULL,app->getInstance(),NULL);
	_afterCreate();


//	hwnd=CreateWindowEx(GET_WIN_EX_STYLE(_option),L"STATIC",L"",
//		WS_VISIBLE | WS_CHILD | GET_WIN_STYLE(_option),
//		x,y,w,h,getParent()->getHWND(),(HMENU)getID(),app->getInstance(),NULL);
//	_afterCreate();
	messageNotify(true);
	paintByGraphics(true);
}
Canvas::~Canvas()
{
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- 描画  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
void Canvas::setPaintProc(PAINTPROC _paintproc)
{
	paintproc=_paintproc;
}

PAINTPROC Canvas::getPaintProc()
{
	return paintproc;
}

LRESULT Canvas::onPaint(CDcGraphics *g)
{
	if(paintproc!=NULL){
		paintproc(g);
	}
	return 0L;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- --  マウス  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void Canvas::setMouseProc(MOUSEPROC _mouseproc)
{
	mouseproc=_mouseproc;
}
LRESULT Canvas::onMouse(UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(mouseproc!=NULL){
		mouseproc(this,msg,(short)LOWORD(lParam),(short)HIWORD(lParam));
	}
	return 0L;
}

