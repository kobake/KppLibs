#include <BaseLib.h>
#include <windows.h>
#include <cstdio>
#include "f_window.h"

HWND CreateAWindow(HINSTANCE hInst,HINSTANCE hPrevInst,const wchar* classname,WNDPROC wndproc,HBRUSH hBrush,HICON hIcon,const wchar* szMenu,DWORD ex_style,DWORD style,const wchar *caption,int x,int y,int w,int h,HWND parent,HMENU hMenu)
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

wchar *tmp_GetWindowText(HWND hwnd)
{
	static wchar tmp[256];
	GetWindowText(hwnd,tmp,255);
	return tmp;
}


int MessageBoxF(HWND hwnd,LPCTSTR lpCaption,UINT uType,LPCTSTR lpText,...)
{
	static wchar tmp[1024];
	va_list mark;
	va_start(mark,lpText);
	vswprintf(tmp,lpText,mark);
	va_end(mark);
	return MessageBox(hwnd,tmp,lpCaption,uType);
}

BOOL vSetWindowTextF(HWND hwnd,LPCTSTR lpString,va_list mark)
{
	static wchar tmp[1024];
	vswprintf(tmp,lpString,mark);
	return SetWindowText(hwnd,tmp);
}

BOOL SetWindowTextF(HWND hwnd,LPCTSTR lpString,...)
{
	va_list mark;
	va_start(mark,lpString);
	BOOL ret=vSetWindowTextF(hwnd,lpString,mark);
	va_end(mark);
	return ret;
}

void SetFont(HWND hwnd,HFONT hFont)
{
	SendMessage(hwnd,WM_SETFONT,(WPARAM)hFont,MAKELPARAM(TRUE,0));
}


void EnableMenuItemID(HMENU hMenu,int id,bool enable)
{
	EnableMenuItem(hMenu,id,MF_BYCOMMAND | (enable?MF_ENABLED:MF_GRAYED));
}

void CheckMenuItemID(HMENU hMenu,int id,bool checked)
{
	CheckMenuItem(hMenu,id,MF_BYCOMMAND | (checked?MF_CHECKED:MF_UNCHECKED));
}

