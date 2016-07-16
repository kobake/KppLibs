#include <BaseLib.h>
#include <BaseLibOld.h>
#include "c_App.h"
#include "c_ProgressBar.h"

ProgressBar::ProgressBar(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_PROGRESS_BAR,0,_parent,_option,_id)
{
	convertXYWH(&x,&y,&w,&h);
	hwnd=CreateWindowEx(0,PROGRESS_CLASS,L"",
		WS_VISIBLE | WS_CHILD,x,y,w,h,getParent()->getHWND(),(HMENU)getID(),g_app->getInstance(),NULL);
	_afterCreate();
}

ProgressBar::~ProgressBar()
{
}



void ProgressBar::setRange(int min,int max)
{
	SendMessage(hwnd,PBM_SETRANGE,0,MAKELPARAM(min,max));
}

void ProgressBar::setPos(int pos)
{
	SendMessage(hwnd,PBM_SETPOS,pos,0);
}

