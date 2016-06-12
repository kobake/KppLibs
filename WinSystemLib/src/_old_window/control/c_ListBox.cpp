#include <windows.h>
#include <string>
#include "../../_old_app/c_App.h"
#include "c_ListBox.h"
#include <BaseLib.h>
using namespace std;

ListBox::ListBox(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_LIST_BOX,0,_parent,_option | WINDOWF_TABSTOP,_id)
{
	convertXYWH(&x,&y,&w,&h);
	hwnd=CreateWindowEx(WS_EX_CLIENTEDGE,L"LISTBOX",L"",
		WS_VISIBLE | WS_CHILD | WS_VSCROLL | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY,x,y,w,h,getParent()->getHWND(),(HMENU)getID(),app->getInstance(),NULL);
	_afterCreate();
}

ListBox::~ListBox()
{
}

int ListBox::addString(char *str)
{
	return SendMessage(hwnd,LB_ADDSTRING,0,(LPARAM)str);
}


void ListBox::setCur(int index)
{
	SendMessage(hwnd,LB_SETCURSEL,(WPARAM)index,0);
//	SendMessage(hwnd,LB_SETSEL,(WPARAM)TRUE,(LPARAM)index);
}

int ListBox::getCur()
{
	return SendMessage(hwnd,LB_GETCURSEL,0,0);
}

