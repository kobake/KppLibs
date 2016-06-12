#include <BaseLib.h>
#include <windows.h>
#include <string>
using namespace std;
#include "../../_old_app/c_App.h"
#include "c_Button.h"

Button::Button(const wchar* caption,int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_BUTTON,0,_parent,_option | WINDOWF_TABSTOP,_id)
{
	convertXYWH(&x,&y,&w,&h);
	//ƒXƒ^ƒCƒ‹
	int style;
	style=WS_VISIBLE | WS_CHILD;
	if(BUTTONF_KIND(_option)==BUTTONF_KIND_DEFAULTBUTTON)style|=BS_DEFPUSHBUTTON;
	//ì¬
	hwnd=CreateWindowEx(0,L"BUTTON",caption,
		style,x,y,w,h,getParent()->getHWND(),(HMENU)getID(),app->getInstance(),NULL);
	_afterCreate();
}

Button::~Button()
{
}




