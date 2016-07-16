#include <BaseLib.h>
#include <BaseLibOld.h>
#include "c_Toolbar.h"
#include <GraphicsLib.h>
#include "src/CBitmap.h"
#include "../_old_app/c_App.h"

Toolbar::Toolbar(TBBUTTON *button_param,int button_num,int bmp_id,int bmp_num,Window *_parent,int _option,int _id)
: Window(CID_TOOLBAR,0,_parent,_option,_id)
{
	//作成
	InitCommonControls();
	hwnd=CreateToolbarEx(
		getParent()->getHWND(), //親ウィンドウ
		WS_CHILD | WS_VISIBLE, //ウィンドウスタイル
		getID(), // コントロールID
		bmp_num, //イメージの数
		g_app->getInstance(),
		bmp_id,
		button_param,
		button_num, // ボタンの数
		0, //ボタンの幅
		0, //ボタンの高さ
		16, //イメージの幅
		15, //イメージの高さ
		sizeof(TBBUTTON)
		);
	_afterCreate();
}

Toolbar::Toolbar(TBBUTTON *button_param,int button_num,CBitmap *bmp,int bmp_num,Window *_parent,int _option,int _id)
: Window(CID_TOOLBAR,0,_parent,_option,_id)
{
	//作成
	InitCommonControls();
	hwnd=CreateToolbarEx(
		getParent()->getHWND(), //親ウィンドウ
		WS_CHILD | WS_VISIBLE, //ウィンドウスタイル
		getID(), // コントロールID
		bmp_num, //イメージの数
		NULL,
		(UINT)bmp->getHBITMAP(),
		button_param,
		button_num, // ボタンの数
		0, //ボタンの幅
		0, //ボタンの高さ
		16, //イメージの幅
		15, //イメージの高さ
		sizeof(TBBUTTON)
		);
	_afterCreate();
}

Toolbar::~Toolbar()
{
}

void Toolbar::setCheckByID(int id,bool check)
{
	BYTE state=(BYTE)SendMessage(hwnd,TB_GETSTATE,id,0);
	BYTE old_state=state;
	if(check){
		state|=TBSTATE_CHECKED; //### PUSHEDでも良い
	}else{
		state|=TBSTATE_CHECKED;
		state^=TBSTATE_CHECKED;
	}
	if(state!=old_state){
		SendMessage(hwnd,TB_SETSTATE,id,(LPARAM)MAKELONG(state,0));
	}
}

void Toolbar::setEnabledByID(int id,bool e)
{
	BYTE state=(BYTE)SendMessage(hwnd,TB_GETSTATE,id,0);
	BYTE old_state=state;
	if(e){
		state|=TBSTATE_ENABLED;
	}else{
		state|=TBSTATE_ENABLED;
		state^=TBSTATE_ENABLED;
	}
	if(state!=old_state){
		SendMessage(hwnd,TB_SETSTATE,id,(LPARAM)MAKELONG(state,0));
	}
}

