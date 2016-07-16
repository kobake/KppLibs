#include <BaseLib.h>
#include <BaseLibOld.h>
#include "../_old_app/c_App.h"
#include "c_Window.h"
#include "f_window.h"
#include <StringLib.h>
#include <DebugLib.h>
#include <ShellAPI.h>
#include <GraphicsLib.h>
#include <Windows.h>
#include <MMSystem.h>
#include "scroll/c_ScrollManager.h"
#include <GraphicsLib.h> // Icon

/*
-- -- -- -- -- -- -- -- -- -- -- -- -- メ モ -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

++ 破棄に関して ++

ユーザーがＸボタンを押すなどして、ウィンドウが破棄された場合、
HWND-Window関連付けが解かれ、onDestroyが呼ばれ、hwndメンバがNULLになる。
Windowオブジェクトは破棄されない。

deleteによりWindowオブジェクトを破棄する場合、
HWND-Window関連付けが解かれ、onDestroyが呼ばれ、DestroyWindow(hwnd)を実行する。
その後、Windowオブジェクト破棄完了。

-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
*/

void Window::dragAcceptFiles(bool accept, bool notify)
{
	DragAcceptFiles(hwnd, accept?TRUE:FALSE);
	notify_drop=notify;
}


void SimpleList_newValue(CHILD &dst)
{
	memset(&dst,0,sizeof(CHILD));
}

void SimpleList_copyValue(CHILD &dst,CHILD &src)
{
	dst=src;
}

void SimpleList_deleteValue(CHILD &dst)
{
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- Windowクラス共有設定 & ツール -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

static HFONT hfntOnCreate=NULL;

void Window::setFontOnCreate(HFONT hFont)
{
	hfntOnCreate=hFont;
}
HFONT Window::getFontOnCreate()
{
	return hfntOnCreate;
}

static Window nullwindow(-1,0,NULL,0,UNIQUE_ID);
Window *Window::getNullWindow()
{
	return &nullwindow;
}
void Window::convertXYWH(int *x,int *y,int *w,int *h)
{
	if(*x==-1)*x=CW_USEDEFAULT;
	if(*y==-1)*y=CW_USEDEFAULT;
	if(*w==-1)*w=CW_USEDEFAULT;
	if(*h==-1)*h=CW_USEDEFAULT;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- --  コンストラクタ・デストラクタ  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


Window::Window(int _type,LPARAM _lParam,Window *_parent,int _option,int _id)
{
	option=_option;
	g_paint=false;
	notify_drop=false;
	notify_message=false;
	window_type=_type;
	userdata=_lParam;
	hwnd=NULL;
	parent=_parent; if(parent==NULL)parent=Window::getNullWindow();
	if(!parent->isNullWindow()){
		if(_id==UNIQUE_ID){
			id=parent->_addChild(this);     //-2以下のIDが指定されたら、UNIQUEなIDを自動で割り当てる
		}else{
			id=parent->_addChild(this,_id); //-1、または0以上のIDはそのまま割り当てる
		}
	}else{
		//親がいない場合は何を指定してもID=-1
		id=-1;
	}
	SetRect(&rcWindow,0,0,0,0);
	SetRect(&rcClient,0,0,0,0);
	//スクロール管理
	hscroll=vscroll=NULL;
	if(WINDOWF_SCROLL(_option) & WINDOWF_SCROLL_MANAGE){
		if(WINDOWF_SCROLL(_option) & WINDOWF_SCROLL_HORZ){
			hscroll=new ScrollManager(this,SB_HORZ);
		}
		if(WINDOWF_SCROLL(_option) & WINDOWF_SCROLL_VERT){
			vscroll=new ScrollManager(this,SB_VERT);
		}
	}
}

Window::~Window()
{
	if(hwnd!=NULL){
		SetWindowLong(hwnd,GWL_WNDPROC,(LONG)WndProcWindowDef);
		onDestroy(WM_DESTROY,0,0); //↑WNDPROCを元に戻したため、Window::WndProcWindowは呼ばれない
		DestroyWindow(hwnd);
		hwnd=NULL;
	}
	m_delete(hscroll);
	m_delete(vscroll);
}

//ウィンドウハンドルにポインタthisを関連付ける
//ウィンドウプロシージャをWindow::WndProcWindowにする
//標準でTAB移動ができるようにする
//標準フォントを関連付ける
//RECT構造体を更新する
void Window::_afterCreate()
{
	SetWindowLong(hwnd,GWL_USERDATA,(LONG)this);
	WndProcWindowDef=(WNDPROC)SetWindowLong(hwnd,GWL_WNDPROC,(LONG)WndProcWindow);
	int tab=WINDOWF_TAB(option);
	if(tab==WINDOWF_NOTABSTOP){
	}else if(tab==WINDOWF_TABSTOP){
		SetWindowLong(hwnd,GWL_STYLE,(LONG)(GetWindowLong(hwnd,GWL_STYLE) | WS_TABSTOP));
	}
	HFONT hFont=getFontOnCreate();
	if(hFont!=NULL)setFont(hFont);
	onWindowSize(-1,-1,-1,-1);
}

//IDテーブルを更新・新しいIDを返す
int Window::_addChild(Window *_child,int _id)
{
	CHILD child; child.id=_id; child.window=_child;
	children.put(-1,child); children.sort((COMPPROC)CHILD::comp);
	return _id;
}
int Window::_addChild(Window *_child)
{
	int i,n=children.size();
	int _id=-1,id0,id1;
	if(n>0){
		id0=id1=children.get(0).id;
		for(i=1;i<n;i++){
			id1=children.get(i).id;
			if(id0!=-1 && id1-id0>1){
				_id=id0+1;
				break;
			}
			id0=id1;
		}
		if(_id==-1)
			_id=id1+1;
	}else{
		_id=IDC_BEGIN;
	}
	return _addChild(_child,_id);
}







// ** ** ** ** ウィンドウ操作 ** ** ** ** //

int Window::messageBox(const wstring& text,const wstring& _caption,int type)
{
	wstring caption=_caption;
	if(type==0){
		type=MB_OK;
	}
	if(caption.size()==0){
		caption=g_app->getSoftName();
	}
	return MessageBox(hwnd,text.c_str(),caption.c_str(),type);
}
int Window::messageBoxF(const wstring& caption,int type,const wchar* text,...)
{
	static wchar tmp[1024];
	va_list mark;
	va_start(mark,text);
	vswprintf(tmp,text,mark);
	va_end(mark);
	return MessageBox(hwnd,tmp,caption.c_str(),type);
}

void Window::onWindowSize(int x,int y,int w,int h)
{
	if(w==-1 || h==-1){
		GetWindowRect(hwnd,&rcWindow);
	}else{
		SetRect(&rcWindow,x,y,x+w,y+h);
	}
	GetClientRect(hwnd,&rcClient);
}
void Window::onClientSize(int w,int h)
{
	if(w==-1 || h==-1){
		GetClientRect(hwnd,&rcClient);
	}else{
		SetRect(&rcClient,0,0,w,h);
	}
	GetWindowRect(hwnd,&rcWindow);
}


// ** ** ** ** ウィンドウ操作 ** ** ** ** //

#include "c_Screen.h"
#include "../_old_app/c_App.h"
void Window::setClientSize(int w,int h)
{
	int ww=(w-rcClient.right);
	int hh=(h-rcClient.bottom);
	WINDOWPLACEMENT wp;
	wp.length=sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwnd,&wp);
	wp.rcNormalPosition.right+=ww;
	wp.rcNormalPosition.bottom+=hh;
	SetWindowPlacement(hwnd,&wp);
}
void Window::moveInDesktopClient()
{
	int left,top,right,bottom;
	int xx=0,yy=0;
	//デスクトップ
	left=screen->getClientLeft();
	top=screen->getClientTop();
	right=screen->getRight();
	bottom=screen->getClientBottom();
	//現在のウィンドウ
	WINDOWPLACEMENT wp;
	wp.length=sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hwnd,&wp);
	//左上座標
	if(wp.rcNormalPosition.left<left){
		xx=left-wp.rcNormalPosition.left;
		wp.rcNormalPosition.left=left;
	}
	if(wp.rcNormalPosition.top<top){
		yy=top-wp.rcNormalPosition.top;
		wp.rcNormalPosition.top=top;
	}
	wp.rcNormalPosition.right+=xx;
	wp.rcNormalPosition.bottom+=yy;
	//右上座標
	if(wp.rcNormalPosition.right>right){
		xx=wp.rcNormalPosition.right-right;
		wp.rcNormalPosition.right=right;
	}
	if(wp.rcNormalPosition.bottom>bottom){
		yy=wp.rcNormalPosition.bottom-bottom;
		wp.rcNormalPosition.bottom=bottom;
	}
	wp.rcNormalPosition.left-=xx; if(wp.rcNormalPosition.left<left)wp.rcNormalPosition.left=left;
	wp.rcNormalPosition.top-=yy;  if(wp.rcNormalPosition.top<top)wp.rcNormalPosition.top=top;
	SetWindowPlacement(hwnd,&wp);
}


void Window::hide()
{
	ShowWindow(hwnd,SW_HIDE);
}

bool Window::isVisible()
{
	return IsWindowVisible(hwnd)!=FALSE;
}


void Window::show(int nCmdShow)
{
	ShowWindow(hwnd,nCmdShow);
}

void Window::showTop(int nCmdShow)
{
	ShowWindow(hwnd,nCmdShow);
	BringWindowToTop(hwnd);
}

void Window::move(int x,int y,int w,int h)
{
	MoveWindow(hwnd,x,y,w,h,TRUE);
	onWindowSize(x,y,w,h);
}

void Window::setFont(HFONT hFont)
{
	SendMessage(hwnd,WM_SETFONT,(WPARAM)hFont,MAKELPARAM(TRUE,0));
}

void Window::setText(const wstring& text)
{
	SetWindowText(hwnd,text.c_str());
}

void Window::setTextF(const wchar* text,...)
{
	va_list mark;
	va_start(mark,text);
	vSetWindowTextF(hwnd,text,mark);
	va_end(mark);
}

wchar *Window::getTextTemp()
{
	return tmp_GetWindowText(hwnd);
}

wchar *Window::getTextAlloc()
{
	int n=GetWindowTextLength(hwnd);
	wchar *buf=(wchar*)calloc(n+2,sizeof(wchar));
	GetWindowText(hwnd,buf,n+1);
	return buf;
}




// ** ** ** ** スクロール ** ** ** ** //

ScrollManager *Window::_getScrollManager(int sb_kind)
{
	if(sb_kind==SB_HORZ){
		return hscroll;
	}else if(sb_kind==SB_VERT){
		return vscroll;
	}else{
		return NULL;
	}
}

void Window::setScrollRange(int range_min,int range_max,int sb_kind)
{
	ScrollManager *scroll=_getScrollManager(sb_kind);
	if(scroll!=NULL)
		scroll->setRange(range_min,range_max);
}
void Window::setScrollPos(int pos,int sb_kind)
{
	ScrollManager *scroll=_getScrollManager(sb_kind);
	if(scroll!=NULL)
		scroll->setPos(pos);
}
int Window::getScrollPos(int sb_kind)
{
	ScrollManager *scroll=_getScrollManager(sb_kind);
	if(scroll!=NULL)
		return scroll->getPos();
	else
		return 0;
}
void Window::enableScroll(bool e,int sb_kind)
{
	ScrollManager *scroll=_getScrollManager(sb_kind);
	if(scroll!=NULL)
		scroll->enable(e);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- メッセージ処理  -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

#include <GraphicsLib.h>

LRESULT CALLBACK Window::WndProcWindow(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	Window *window; BOOL processed=FALSE; LRESULT ret=0;
	// Windowインスタンスを取得
	window=(Window*)GetWindowLong(hwnd,GWL_USERDATA);
	// 継承先にメッセージを通知するか
	if(!window->notify_message){
		return CallWindowProc(window->WndProcWindowDef,hwnd,msg,wparam,lparam);
	}
	// 総合通知プロシージャを呼び出す - processed が TRUE ならば、処理は完了したとみなす
	processed=FALSE;
	ret=window->onMessage(msg,wparam,lparam,&processed);
	if(processed!=FALSE)return ret;
	// それぞれのメッセージを継承先に通知する
	switch(msg){
	case WM_DROPFILES:
		if(window->notify_drop && window->parent!=NULL){
			window->parent->WndProcWindow(window->parent->getHWND(),KM_NOTIFY_DROPFILES,wparam,window->id);
			return 0L;
		}else{
			return window->onDropFiles(msg,wparam,lparam);
		}
		break;
	case WM_PAINT:
		/*if(window->g_paint==0){
			CDcGraphics g(1,1);
			onPaint_onWindow=0;
			window->onPaint(&g);
			if(onPaint_onWindow==0){
				window->g_paint=PAINT_GRAPHICS;
			}else{
				window->g_paint=PAINT_WNDPROC;
			}
		}*/
		if(window->g_paint){
			CDcGraphics g(hwnd,msg,wparam,lparam);

//			PAINTSTRUCT ps;
//			HDC hdc;
//			hdc=BeginPaint(hwnd,&ps);
//			CDcGraphics g(hdc,window->getClientWidth(),window->getClientHeight());
//			g.setInvalidRect(&ps.rcPaint);
			ret=window->onPaint(&g);
//			EndPaint(hwnd,&ps);
			return ret;
		}else{
			return window->onPaint(msg,wparam,lparam);
		}
	case WM_CTLCOLORBTN:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORMSGBOX:
		return window->onCtlColor(msg,wparam,lparam);
	case WM_DESTROY:
		SetWindowLong(window->hwnd,GWL_WNDPROC,(LONG)window->WndProcWindowDef); //関連付けを解く
		ret=window->onDestroy(msg,wparam,lparam);
		window->hwnd=NULL;
		return ret;
	case MM_MIXM_CONTROL_CHANGE:
	case MM_MIXM_LINE_CHANGE:
		return window->onMixer(msg,wparam,lparam);
	case WM_TIMER:
		return window->onTimer(msg,wparam,lparam);
	case WM_COMMAND:
		return window->onCommand(msg,wparam,lparam);
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		return window->onFocus(msg,wparam,lparam);
	case WM_NOTIFY:
		return window->onNotify(msg,wparam,lparam);
	case WM_SIZE:
		return window->onSize(msg,wparam,lparam);
	case WM_MOVE:
		return window->onMove(msg,wparam,lparam);
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
//		if(window->testflag==123)
//			MessageBox(NULL,tmp_itoa(processed),L"",MB_OK);
		return window->onMouse(msg,wparam,lparam);
	case WM_HSCROLL:
	case WM_VSCROLL:
		return window->onScroll(msg,wparam,lparam);
	case WM_ACTIVATE:
		return window->onActivate(msg,wparam,lparam);
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
		return window->onKey(msg,wparam,lparam);
	case WM_INITMENU:
		return window->onMenu(msg,wparam,lparam);
	case WM_CLOSE:
		return window->onClose(msg,wparam,lparam);
	case WM_TASKTRAY:
		return window->onTaskTray(msg,wparam,lparam);
	}
	return CallWindowProc(window->WndProcWindowDef,hwnd,msg,wparam,lparam);
}


LRESULT Window::onFocus(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onSize(UINT msg,WPARAM wparam,LPARAM lparam)
{
	onClientSize(LOWORD(lparam),HIWORD(lparam));
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onMove(UINT msg,WPARAM wparam,LPARAM lparam)
{
	onWindowSize(-1,-1,-1,-1);
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onScroll(UINT msg,WPARAM wparam,LPARAM lparam)
{
	if(scroll_list.size()){
		for(int i=0;i<(int)scroll_list.size();i++){
			scroll_list[i]->onScroll(msg,wparam,lparam);
		}
		repaint(false); //仮
		return 0L;
	}
	if(WINDOWF_SCROLL(option) & WINDOWF_SCROLL_MANAGE){
		if(msg==WM_HSCROLL){
			if(hscroll!=NULL)
				hscroll->onScroll(msg,wparam,lparam);
			return 0L;
		}else if(msg==WM_VSCROLL){
			if(vscroll!=NULL)
				vscroll->onScroll(msg,wparam,lparam);
			return 0L;
		}else{
			return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
		}
	}else{
		return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
	}
}

LRESULT Window::onActivate(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onPaint(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onPaint(CDcGraphics *g)
{
	return 0L;
}

LRESULT Window::onKey(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onClose(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onDestroy(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onMouse(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onNotify(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onCommand(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onTimer(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onDropFiles(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onMenu(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onTaskTray(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onMixer(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}


LRESULT Window::onCtlColor(UINT msg,WPARAM wparam,LPARAM lparam)
{
	return CallWindowProc(WndProcWindowDef,hwnd,msg,wparam,lparam);
}

LRESULT Window::onMessage(UINT msg,WPARAM wparam,LPARAM lparam,BOOL *processed)
{
	//### ここで *process を 0 にする処理を加えれば、引数の *process はいらないんじゃないか
	//### つまり、あらかじめ *process を 1 にしておく
	//### 継承先で onMessage が定義されていれば、そちらが呼ばれ、*process は 1 のままである
	//### 定義されていなければ、Window::onMessage(つまりここ)が呼ばれ、*process が 0 になる
	//### 継承先からデフォルトの処理をさせたい場合は、Window::onMessage を明示的に呼び出せばよい
	return 0L;
}




void Window::setIcon(Icon *icon)
{
	SendMessage(hwnd,WM_SETICON,ICON_BIG,(LPARAM)icon->hIcon);
}

LRESULT Window::sendSize()
{
	//### SIZE_RESTOREDは仮
	onClientSize(-1,-1);
	return sendMessage(WM_SIZE,SIZE_RESTORED,MAKELPARAM(rcClient.right-rcClient.left,rcClient.bottom-rcClient.top));
}

LRESULT Window::postSize()
{
	//### SIZE_RESTOREDは仮
	onClientSize(-1,-1);
	return postMessage(WM_SIZE,SIZE_RESTORED,MAKELPARAM(rcClient.right-rcClient.left,rcClient.bottom-rcClient.top));
}

LRESULT Window::sendMenuClick(int id)
{
	return sendMessage(WM_COMMAND,MAKEWPARAM(id,0),NULL);
}
LRESULT Window::postMenuClick(int id)
{
	return postMessage(WM_COMMAND,MAKEWPARAM(id,0),NULL);
}

