#include <windows.h>
#include <string>
#include "common/_required.h"
using namespace std;
#include "c_ScrollBar.h"
#include "c_CustomWindow.h"
#include "id/class.h"
#include "c_App.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void ScrollBar::_init_v()
{
}

ScrollBar::ScrollBar(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_CANVAS,0,_parent,_option | WINDOWF_TABSTOP,_id)
{
	_init_v();
	convertXYWH(&x,&y,&w,&h);

	//スタイルの算出
	int kind=SCROLLBARF_KIND(_option);
	DWORD ex_style=0;
	DWORD style=0;
	if(kind==SCROLLBARF_KIND_HORZ){
		style|=SBS_HORZ;
	}else if(kind==SCROLLBARF_KIND_VERT){
		style|=SBS_VERT;
	}
	//
	style|=GET_WIN_STYLE(_option);
	ex_style|=GET_WIN_EX_STYLE(_option);
	//作成
	hwnd=CreateWindowEx(ex_style,L"SCROLLBAR",L"",WS_VISIBLE | WS_CHILD | style,x,y,w,h,getParent()->getHWND(),NULL,app->getInstance(),NULL);
	_afterCreate();
	//
	messageNotify(true);
}
ScrollBar::~ScrollBar()
{
}
