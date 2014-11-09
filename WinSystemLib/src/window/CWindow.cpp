#include "_required.h"
#include "CWindow.h"
#include "CWindowClass.h"
#include "../CWinError.h"
#include "../Cast.h"
#include "Screen.h"
#include "../../../GraphicsLib/src/CFont.h"
#include <DebugLib.h>

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  ウィンドウプロシージャ                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

LRESULT CWindow::_CallProcedures(UINT msg,WPARAM wparam,LPARAM lparam)
{
	for(int i=0;i<(int)this->m_vHookProcs.size();i++){
		LRESULT nResult = 0;
		if(this->m_vHookProcs[i](this,msg,wparam,lparam,&nResult)){
			return nResult;
		}
	}
	return this->OnMessage(msg,wparam,lparam);
}

LRESULT CALLBACK WndProcWindow(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	CWindow* window=LongToPointer<CWindow*>(GetWindowLong(hwnd,GWL_USERDATA));
	if(window){
		LRESULT ret=window->_CallProcedures(msg,wparam,lparam);
		if(msg==WM_DESTROY){ //ここは、「X」ボタンで閉じた場合だけ、呼ばれる。
			//hwndは無効になるので、クリアしておく
			window->SetHWND(NULL);
		}
		return ret;
	}
	else{
		if(msg==WM_CREATE){
			//CWindow*を保持
			CREATESTRUCT* cs=(CREATESTRUCT*)lparam;
			CWindow* window=(CWindow*)cs->lpCreateParams;
			::SetWindowLong(hwnd,GWL_USERDATA,PointerToLong(window));

			if(window){
				//HWNDをここで保持
				window->SetHWND(hwnd);

				//WM_CREATEを送りなおす
				window->_CallProcedures(msg,wparam,lparam);
			}
		}
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       初期化と終了                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

//###暫定フォント
static void _SetTmpFont(HWND hwnd)
{
	static CFont cFont(L"ＭＳ ゴシック", CPointSize(9));
	::SendMessage(hwnd, WM_SETFONT, (WPARAM)cFont.GetHFONT(), TRUE);
}

CWindow::CWindow()
: m_wndprocSubclass(NULL)
{
	//設定のデフォルト値
	m_clrTransparent=0x000000;
	m_btAlpha=255;
}

CWindow::~CWindow()
{
	Destroy();
}

void CWindow::Attach(HWND hwnd)
{
	assert(GetHWND()==NULL);
	this->SetHWND(hwnd);

	//Window*設定
	LONG nOld = ::SetWindowLong(GetHWND(),GWL_USERDATA,PointerToLong(this));

	//ウィンドウプロシージャ設定
	m_wndprocSubclass = LongToPointer<WNDPROC>(::SetWindowLong(GetHWND(), GWL_WNDPROC, PointerToLong(WndProcWindow)));

	//##暫定フォント
	_SetTmpFont(this->GetHWND());
}

void CWindow::Create(const SWindowInfo& info)
{
	if(info.valid() && GetHWND()==NULL){
		//ウィンドウクラス登録
		bool bRegisted = info.wclass.Regist(WndProcWindow);

		//ウィンドウ作成
		HWND hwnd_tmp=CreateWindowEx(
			info.ex_style,
			info.wclass.GetName(),
			info.caption,
			info.style,
			def_adjust<int>(info.rc.Left(),  CW_USEDEFAULT),	//座標
			def_adjust<int>(info.rc.Top(),   CW_USEDEFAULT),
			def_adjust<int>(info.rc.Width(), CW_USEDEFAULT),
			def_adjust<int>(info.rc.Height(),CW_USEDEFAULT),
			info.hwndParent,									//親ウィンドウ
			info.hMenu,											//メニュー
			info.wclass.GetModule(),							//HINSTANCE
			this												//lpParam
		);

		//ウィンドウクラスが既に登録済みのものでり、
		//そのウィンドウプロシージャが WndProcWindow でない場合は、(つまり、Windows標準クラス(EDITなど)である可能性が高い)
		//サブクラス化を行う
		if(!bRegisted && (WNDPROC)(intptr_t)::GetWindowLong(hwnd_tmp,GWL_WNDPROC)!=WndProcWindow){
			Attach(hwnd_tmp);
		}
		else{
			SetHWND(hwnd_tmp);
		}

		//エラーチェック
		if(GetHWND()==NULL){
			int test=1;
			test=0;
	//		WinError().show_error();
		}

		//##暫定フォント
		_SetTmpFont(this->GetHWND());
	}
}

void CWindow::Destroy()
{
	if(GetHWND()){
		//CWindow*は無効になるので、ユーザーデータを削除しておく
		::SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)0);

		//明示的に通知を送る
		this->_CallProcedures(WM_DESTROY,0,0);

		//本当に削除
		::DestroyWindow(GetHWND());                      //これにより、通知は送られない
		SetHWND(NULL);
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     インターフェース                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //


void CWindow::SetTransparentColor(COLORREF _transparent_color)
{
	m_clrTransparent=_transparent_color;
	SetLayeredWindowAttributes(GetHWND(),m_clrTransparent,m_btAlpha,LWA_COLORKEY | LWA_ALPHA);
}

void CWindow::SetAlpha(uchar _alpha)
{
	m_btAlpha=_alpha;
	SetLayeredWindowAttributes(GetHWND(),m_clrTransparent,m_btAlpha,LWA_COLORKEY | LWA_ALPHA);
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  自家製インターフェース                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CWindow::SetExLong(long n)
{
	memcpy(m_freedata,&n,sizeof(n));
}

long CWindow::GetExLong() const
{
	long n;
	memcpy(&n,m_freedata,sizeof(n));
	return n;
}

