#include <windows.h>
#include <string>
using namespace std;
#include "../../_old_app/c_App.h"
#include "c_Label.h"
#include <BaseLib.h>

Label::Label(const wchar* caption,int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_LABEL,0,_parent,_option,_id)
{
	int exstyle,style;
	// -- -- ウィンドウ全般フラグ -- -- //
	//ボーダースタイル
	style=WS_VISIBLE | WS_CHILD | SS_LEFTNOWORDWRAP | SS_NOTIFY;
	int border=WINDOWF_BORDER(_option);
	if(border==WINDOWF_BORDER_NONE){
		exstyle=0;
	}else if(border==WINDOWF_BORDER_SINGLE){
		exstyle=0;
		style|=WS_BORDER;
	}else if(border==WINDOWF_BORDER_CLIENTEDGE){
		exstyle=WS_EX_CLIENTEDGE;
	}else{
		exstyle=0;
	}
	// -- -- コントロール固有フラグ -- -- //
	_option=WINDOWF_CONTROL(_option);
	//アライン
	int align=LABELF_ALIGN(_option);
	if(align==LABELF_ALIGN_LEFT){
		;
	}else if(align==LABELF_ALIGN_VCENTER){
		style|=SS_CENTERIMAGE;
	}else if(align==LABELF_ALIGN_RIGHT){
		style|=SS_RIGHT;
		style^=SS_LEFTNOWORDWRAP;
	}
	// -- -- ウィンドウ作成 -- -- //
	convertXYWH(&x,&y,&w,&h);
	hwnd=CreateWindowEx(exstyle,L"STATIC",caption,
		style,x,y,w,h,getParent()->getHWND(),(HMENU)getID(),g_app->getInstance(),NULL);
	_afterCreate();
}

Label::~Label()
{
}



