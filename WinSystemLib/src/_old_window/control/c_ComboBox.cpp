#include <BaseLib.h>
#include <windows.h>
#include <string>
using namespace std;
#include "../../_old_app/c_App.h"
#include "c_ComboBox.h"

ComboBox::ComboBox(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_COMBO_BOX,0,_parent,_option | WINDOWF_TABSTOP,_id)
{
	convertXYWH(&x,&y,&w,&h);
	drop_h=100;
	h=drop_h;
	hwnd=CreateWindowEx(WS_EX_CLIENTEDGE,L"COMBOBOX",L"",
		WS_VISIBLE | WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,x,y,w,h,getParent()->getHWND(),(HMENU)getID(),app->getInstance(),NULL);
	SendMessage(hwnd,CB_SETDROPPEDWIDTH,100,0);
	_afterCreate();
}

ComboBox::~ComboBox()
{
}

int ComboBox::addString(const wchar *str)
{
	return SendMessage(hwnd,CB_ADDSTRING,0,(LPARAM)str);
}


