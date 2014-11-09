#include "_required.h"
#include "CDialogPanel.h"
#include "CWindowClass.h"
#include "../CWinError.h"
#include "../Cast.h"
#include "Screen.h"
#include "../../../GraphicsLib/src/CFont.h"
#include "../Cast.h"
#include <DebugLib.h>


//WM_INITDIALOG時にインスタンスポインタ値を提供する
CDialogPanel* g_instant_pCreatingInstance;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  ウィンドウプロシージャ                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

INT_PTR CDialogPanel::_CallProcedures(UINT msg,WPARAM wparam,LPARAM lparam)
{
	for(int i=0;i<(int)this->m_vHookProcs.size();i++){
		LRESULT nResult = 0;
		if(this->m_vHookProcs[i](this,msg,wparam,lparam)){
			return TRUE;
		}
	}
	return this->OnMessage(msg,wparam,lparam);
}

/*
通常、ダイアログボックスプロシージャは
メッセージを処理した場合に 0 以外の値（TRUE）を、
処理しなかった場合に 0（FALSE）を返すべきです。
ダイアログボックスプロシージャが 0（FALSE）を返した場合、
ダイアログマネージャはそのメッセージに応答して既定のダイアログ処理を実行します。
*/
INT_PTR CALLBACK CDialog_DialogProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	CDialogPanel* dialog=LongToPointer<CDialogPanel*>(GetWindowLong(hwnd,GWL_USERDATA));
		if(msg==WM_CREATE){
			int n;
			n=0;
		}
	if(dialog){
		LRESULT ret=dialog->_CallProcedures(msg,wparam,lparam);
		if(msg==WM_DESTROY){ //ここは、「X」ボタンで閉じた場合だけ、呼ばれる。
			//hwndは無効になるので、クリアしておく
			dialog->SetHWND(NULL);
		}
		return ret;
	}
	else{
		if(msg==WM_INITDIALOG){
			//CDialogPanel*を保持
			CDialogPanel* dialog = g_instant_pCreatingInstance;
			::SetWindowLong(hwnd,GWL_USERDATA,PointerToLong(dialog));
			g_instant_pCreatingInstance = NULL;

			if(dialog){
				//HWNDをここで保持
				dialog->SetHWND(hwnd);

				//WM_INITDIALOGを送りなおす
				dialog->_CallProcedures(msg,wparam,lparam);
			}
			return TRUE;
		}
	}
	return FALSE;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                       初期化と終了                          //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

CDialogPanel::CDialogPanel()
: m_dlgprocSubclass(NULL)
{
}

CDialogPanel::~CDialogPanel()
{
	Destroy();
}

//#######未試験
void CDialogPanel::Attach(HWND hwnd)
{
	assert(GetHWND()==NULL);
	this->SetHWND(hwnd);

	//Window*設定
	LONG nOld = ::SetWindowLong(GetHWND(),GWL_USERDATA,PointerToLong(this));

	//ウィンドウプロシージャ設定
	m_dlgprocSubclass = LongToPointer<DLGPROC>(::SetWindowLong(GetHWND(), DWL_DLGPROC, PointerToLong(CDialog_DialogProc)));
}

void CDialogPanel::Create(int nResourceId, HWND hwndParent)
{
	if(GetHWND()==NULL){
		//ウィンドウ作成
		g_instant_pCreatingInstance = this; //###ちょっと汚いけど、一時的にグローバル変数に this を保存
		HWND hwnd_tmp = ::CreateDialog(
			c2lib::GetInstance(),
			MAKEINTRESOURCE(nResourceId),
			hwndParent,
			CDialog_DialogProc
		);
		SetHWND(hwnd_tmp);
		//ウィンドウ表示 (Panelなので、即表示しちゃう)
		this->Show();
	}
}

void CDialogPanel::Destroy()
{
	if(GetHWND()){
		//CDialogPanel*は無効になるので、ユーザーデータを削除しておく
		::SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)0);

		//明示的に通知を送る
		this->_CallProcedures(WM_DESTROY,0,0);

		//本当に削除
		::DestroyWindow(GetHWND());                      //これにより、通知は送られない
		SetHWND(NULL);
	}
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                  自家製インターフェース                     //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

void CDialogPanel::SetExLong(long n)
{
	memcpy(freedata,&n,sizeof(n));
}

long CDialogPanel::GetExLong() const
{
	long n;
	memcpy(&n,freedata,sizeof(n));
	return n;
}



