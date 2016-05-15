#include <windows.h>
#include <string>
using namespace std;
#include "c_App.h"
#include "c_CheckBox.h"
#include "id/class.h"

CheckBox::CheckBox(const wchar* caption,int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_CHECK_BOX,0,_parent,_option | WINDOWF_TABSTOP,_id)
{
	convertXYWH(&x,&y,&w,&h);
	hwnd=CreateWindowEx(0,L"BUTTON",caption,
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX,x,y,w,h,getParent()->getHWND(),(HMENU)getID(),app->getInstance(),NULL);
	_afterCreate();
}

CheckBox::~CheckBox()
{
}

void CheckBox::setCheck(int c)
{
	if(c==1){
		SendMessage(hwnd,BM_SETCHECK,BST_CHECKED,0);
	}else if(c==0){
		SendMessage(hwnd,BM_SETCHECK,BST_UNCHECKED,0);
	}else if(c==-1){
		SendMessage(hwnd,BM_SETCHECK,BST_INDETERMINATE,0);
	}
}

int CheckBox::getCheck()
{
	int c=SendMessage(hwnd,BM_GETCHECK,0,0);
	if(c==BST_CHECKED)return 1;
	if(c==BST_UNCHECKED)return 0;
	if(c==BST_INDETERMINATE)return -1;
	return 0;
}


