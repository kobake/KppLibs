#include <BaseLib.h>
#include <BaseLibOld.h>
#include "c_CustomWindow.h"
#include "../f_window.h"
#include "../../_old_app/c_App.h"

CustomWindow::CustomWindow(const wstring& caption,int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_CUSTOM_WINDOW,0,_parent,_option,_id)
{
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
	DWORD style=WS_OVERLAPPEDWINDOW;
	if(kind==CUSTOMWINDOWF_KIND_TOOL){
		ex_style|=WS_EX_TOOLWINDOW;
		style=WS_SYSMENU | WS_SIZEBOX;
	}else if(kind==CUSTOMWINDOWF_KIND_POPUP){
		ex_style=0;
		style=WS_POPUP;
	}else if(kind==CUSTOMWINDOWF_KIND_DIALOG){
		ex_style=0;
//		style=WS_CAPTION | WS_SYSMENU;
		style=DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU;
	}else if(kind==CUSTOMWINDOWF_KIND_CANVAS){
		ex_style=0;
		style=0;
	}
	//
	style|=GET_WIN_STYLE(_option);
	ex_style|=GET_WIN_EX_STYLE(_option);
	//作成
	hwnd=CreateWindowEx(ex_style,L"CustomWindow",caption.c_str(),style,x,y,w,h,getParent()->getHWND(),NULL,app->getInstance(),NULL);
	_afterCreate();
}
CustomWindow::~CustomWindow()
{
}

/*
HWND CreateAWindow(HINSTANCE hInst,HINSTANCE hPrevInst,char *classname,WNDPROC wndproc,HBRUSH hBrush,HICON hIcon,char *szMenu,DWORD ex_style,DWORD style,const wchar* caption,int x,int y,int w,int h,HWND parent,HMENU hMenu)
{
	//デフォルト値の設定
	if(x==-1)x=CW_USEDEFAULT;
	if(y==-1)y=CW_USEDEFAULT;
	if(w==-1)w=CW_USEDEFAULT;
	if(h==-1)h=CW_USEDEFAULT;
	//ウィンドウクラスの登録
	if(hPrevInst==NULL){
		WNDCLASSEX wndclass;
		wndclass.cbSize=sizeof(WNDCLASSEX);
		wndclass.style=CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc=wndproc;
		wndclass.cbClsExtra=0;
		wndclass.cbWndExtra=0;
		wndclass.hInstance=hInst;
		wndclass.hIcon=hIcon;
		wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
		wndclass.hbrBackground=hBrush;
		wndclass.lpszMenuName=szMenu;
		wndclass.lpszClassName=classname;
		wndclass.hIconSm=hIcon;
		RegisterClassEx(&wndclass);
	}
	return CreateWindowEx(ex_style,classname,caption,style,x,y,w,h,parent,hMenu,hInst,NULL);
}
*/
