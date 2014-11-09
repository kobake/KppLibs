#include "_required.h"
#include "CWinSystem.h"
#include "mymsg.h"
#include <assert.h>
using namespace std;
HACCEL CWinSystem::m_hAccel;
std::set<HWND> CWinSystem::m_setDialogs;
HWND CWinSystem::m_hwndMain;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     ウィンドウクラス                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
ATOM CWinSystem::RegisterWindowClass(
	const wchar_t* classname,
	WNDPROC wndproc,
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance)
{
	ATOM ret=0;
	if(hPrevInstance==NULL){
		WNDCLASSEX wndclass;
		wndclass.cbSize=sizeof(WNDCLASSEX);
		wndclass.style=CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc=wndproc;
		wndclass.cbClsExtra=0;
		wndclass.cbWndExtra=0;
		wndclass.hInstance=hInstance;
		wndclass.hIcon=NULL;
		wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
		wndclass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
		wndclass.lpszMenuName=NULL;
		wndclass.lpszClassName=classname;
		wndclass.hIconSm=NULL;
		ret=RegisterClassEx(&wndclass);
	}
	return ret;
}

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                     メッセージループ                        //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

int CWinSystem::DoMessageLoop_Simple(VOID_FUNC PreIdle)
{
	assert(CWinSystem::GetMainWindow());

	MSG msg;
	while(GetMessage(&msg,NULL,0,0)){
		//準アイドル処理 (#######FsWalker専用。要検証)
		if(PreIdle){
			PreIdle();
		}

		//アクセラレータ処理
		if(m_hAccel && m_hwndMain){
			if(TranslateAccelerator(m_hwndMain,m_hAccel,&msg))continue;
		}

		//ダイアログ処理
		set<HWND>::const_iterator p = m_setDialogs.begin(), q = m_setDialogs.end();
		while(p!=q){
			if(::IsDialogMessage(*p, &msg))goto next;
			p++;
		}

		//通常処理
		TranslateMessage(&msg);
		DispatchMessage(&msg);
next:
		;
	}
	return (int)msg.wParam;
}

int CWinSystem::DoMessageLoop_Modal(HWND hwndDialog,HWND hwndOwner)
{
	//呼び出し元無効化
	HWND hwndRoot=GetAncestor(hwndOwner,GA_ROOT);
	EnableWindow(hwndRoot,FALSE);

	//※ホントはここでダイアログ作成

	//ポップアップが閉じられるまで待つ
	MSG msg;
	while(1){
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			//アプリ終了通知
			if(msg.message==WM_QUIT)goto next;

			//ダイアログ終了通知
			if(msg.message==WM_APP_DIALOG_END){
				goto next;
			}

			//メッセージ転送
			if(!IsDialogMessage(hwndDialog,&msg)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		//予定外：ダイアログが破棄された
		if(!IsWindow(hwndDialog))break;

		Sleep(10);
	}
next:

	//呼び出し元有効化
	EnableWindow(hwndRoot,TRUE);
	SetForegroundWindow(hwndRoot);
	SetActiveWindow(hwndRoot);

	//ダイアログ破棄
	if(IsWindow(hwndDialog)){
		DestroyWindow(hwndDialog);
	}

	//###今のところは常に0を返す
	return 0;
}

