#include <BaseLib.h>
#include <BaseLibOld.h>
#include <StringLib.h>
#include "../../_old_app/c_App.h"
#include "c_EditBase.h"
using namespace util;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- --  コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void EditBase::_refresh_v()
{
	ctrl_on=(GetAsyncKeyState(VK_CONTROL) & 0x8000)!=0;
}

void EditBase::_init_v()
{
	_refresh_v();
}

EditBase::EditBase(int x,int y,int w,int h,Window *_parent,int _option,int _id)
: Window(CID_EDIT_BASE,0,_parent,_option | WINDOWF_TABSTOP,_id)
{
	_init_v();
	convertXYWH(&x,&y,&w,&h);
	edit_kind=EDITF_KIND(_option);
	int flag_style=0;
	if(EDITF_FLAG(_option) & EDITF_FLAG_READONLY){
		flag_style=ES_READONLY;
	}
	if(EDITF_FLAG(_option) & EDITF_FLAG_SELECTONFOCUS){
		select_on_focus=true;
	}else{
		select_on_focus=false;
	}
	if(edit_kind==EDITF_KIND_FIELD){
field:
		//作成
		hwnd=CreateWindowEx(WS_EX_CLIENTEDGE,L"Edit",L"",
			WS_VISIBLE | WS_CHILD | 
			ES_AUTOHSCROLL | flag_style,
			x,y,w,h,getParent()->getHWND(),(HMENU)getID(),g_app->getInstance(),NULL);
	}else if(edit_kind==EDITF_KIND_BOX){
		//スクロール属性
		int scroll_style=0;
		if(WINDOWF_SCROLL(_option) & WINDOWF_SCROLL_HORZ){
			scroll_style|=WS_HSCROLL | ES_AUTOHSCROLL;
		}
		if(WINDOWF_SCROLL(_option) & WINDOWF_SCROLL_VERT){
			scroll_style|=WS_VSCROLL | ES_AUTOVSCROLL;
		}
		//作成
		hwnd=CreateWindowEx(WS_EX_CLIENTEDGE,L"Edit",L"",
			WS_VISIBLE | WS_CHILD |
			ES_MULTILINE | ES_WANTRETURN | ES_NOHIDESEL | scroll_style | flag_style,
			x,y,w,h,getParent()->getHWND(),(HMENU)getID(),g_app->getInstance(),NULL);
	}else{
		edit_kind=EDITF_KIND_FIELD;
		goto field;
	}
	_afterCreate();
	messageNotify(true);
}


EditBase::~EditBase()
{
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- メッセージ処理  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

LRESULT EditBase::onMessage(UINT msg,WPARAM wParam,LPARAM lParam,BOOL *processed)
{
	switch(msg){
	case WM_KEYDOWN:
		if(wParam==VK_RETURN){
			if(edit_kind==EDITF_KIND_FIELD){
				NMHDR nm;
				nm.hwndFrom=hwnd;
				nm.idFrom=getID();
				nm.code=EN_ENTER;
				SendMessage(getParent()->getHWND(),WM_NOTIFY,getID(),(LPARAM)&nm);
				return 0L;
			}
		}else if(wParam==VK_CONTROL){
			ctrl_on=true;
		}else if(ctrl_on && wParam==L'A'){
			setSelAll();
			return 0L;
		}
		return Window::onMessage(msg,wParam,lParam,processed);
	case WM_KEYUP:
		if(wParam==VK_CONTROL){
			ctrl_on=false;
		}
		return Window::onMessage(msg,wParam,lParam,processed);
	case WM_CHAR:
		//ビープ音の除去
		if(wParam==1){
			//Ctrl+A時
			return 0L;
		}else if(wParam==VK_RETURN){
			//FieldでEnter時
			if(edit_kind==EDITF_KIND_FIELD){
				return 0L;
			}
		}
		return Window::onMessage(msg,wParam,lParam,processed);
	case WM_SETFOCUS:
		_refresh_v();
		if(select_on_focus){
			setSelAll();
		}
		return Window::onMessage(msg,wParam,lParam,processed);
	default:
		return Window::onMessage(msg,wParam,lParam,processed);
	}
	*processed=TRUE;
	return 0L;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- 操作  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


void EditBase::replaceSel(const wchar *text,bool can_undo)
{
	SendMessage(hwnd,EM_REPLACESEL,(WPARAM)can_undo,(LPARAM)text);
}

void EditBase::setSel(int begin,int end)
{
	SendMessage(hwnd,EM_SETSEL,begin,end);
	SendMessage(hwnd,EM_SCROLLCARET,0,0);
}

void EditBase::setSelEnd()
{
	int n=this->getTextLength();
	SendMessage(hwnd,EM_SETSEL,n,n);
	SendMessage(hwnd,EM_SCROLLCARET,0,0);
}

void EditBase::setSelAll()
{
	int n=this->getTextLength();
	SendMessage(hwnd,EM_SETSEL,0,n);
	SendMessage(hwnd,EM_SCROLLCARET,0,0);
}

int EditBase::getSelStart()
{
	DWORD start,end;
	sendMessage(EM_GETSEL,(WPARAM)&start,(LPARAM)&end);
	return start;
}
int EditBase::getSelEnd()
{
	DWORD start,end;
	sendMessage(EM_GETSEL,(WPARAM)&start,(LPARAM)&end);
	return end;
}

bool EditBase::findTextNext(const wchar *word)
{
	bool ret;
	int begin,end;
	//###EM_GETHANDLE
	wchar *p,*text=this->getTextAlloc();
	//検索開始位置
	begin=getSelEnd();
	if(begin==-1)begin=0;
	//検索
	p=wcsiwcs(&text[begin],word);
	if(p!=NULL){
		begin=p-text;
		end=begin+wcslen(word);
		setSel(begin,end);
		ret=true;
	}else{
		ret=false;
	}
	//後始末
	free(text);
	return ret;
}

bool EditBase::findTextLast(const wchar *word)
{
	bool ret;
	int begin,end;
	//###EM_GETHANDLE
	wchar *p,*text=this->getTextAlloc();
	//検索開始位置
	begin=getSelStart();
	if(begin==-1)begin=wcslen(text);
	//検索
	p=wcsriwcs(text,word,begin);
	if(p!=NULL){
		begin=p-text;
		end=begin+wcslen(word);
		setSel(begin,end);
		ret=true;
	}else{
		ret=false;
	}
	//後始末
	free(text);
	return ret;
}

