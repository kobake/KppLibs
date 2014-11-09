/*
	遅延コピーのテスト
*/
#include "common/_required.h"
#include "common/sys.h"
#include <windows.h>
#include <vector>
#include <string>
#include <shlobj.h>
#include <WinSystemLib.h>
#include "../resource.h"
#include "CDataObject.h"
#pragma comment(lib, "comctl32.lib")
#define WM_MYTRAY (WM_APP + 1)

HWND g_hwndMain;

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static IDataObject* pDataObject = NULL;

	switch(msg){
	case WM_CREATE:
		::OleInitialize(NULL);
		pDataObject = new CDataObject();
		break;
	case WM_DESTROY:
		pDataObject->Release();
		::OleUninitialize();
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		{
			
		}
		break;
	case WM_RBUTTONUP:
		{
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case MNU_FILE_EXIT:
			break;
		case MNU_EDIT_COPY:
			{
				if(!::OpenClipboard(hwnd))break;
				HGLOBAL hMem = ::GlobalAlloc(GHND, 1024);
				void* pMem = ::GlobalLock(hMem);
				DROPFILES* df = (DROPFILES*)pMem;
				df->pFiles = sizeof(*df);
				df->pt.x = 0;
				df->pt.y = 0;
				df->fNC = FALSE;
				df->fWide = TRUE;
				wchar_t* p = (wchar_t*)&df[1];
				wcscpy(p, L"D:\\_tmp\\abc.txt"); p += wcslen(p) + 1;
				wcscpy(p, L"D:\\_tmp\\def.txt"); p += wcslen(p) + 1;
				*p = 0;
				::GlobalUnlock(hMem);

				::EmptyClipboard();
				::SetClipboardData(CF_HDROP, hMem);
				::CloseClipboard();
			}
			break;
		case MNU_EDIT_CUT:
			{
				::OleSetClipboard(pDataObject);
			}
			break;
		}
		break;
	case WM_MYTRAY:
		//トレイアイコン右クリック対応
		switch(lParam){
		case WM_RBUTTONUP:
			if(wParam == 100){
				POINT pt;
				GetCursorPos(&pt);

				//ポップアップメニュー
				HMENU hMenu = ::LoadMenu(c2lib::GetInstance(), MAKEINTRESOURCE(MNU_MAIN));
				::SetForegroundWindow(g_hwndMain); //※Q135788
				int nRet = ::TrackPopupMenu(
					GetSubMenu(hMenu, 1),
					0
					| TPM_LEFTALIGN | TPM_BOTTOMALIGN	//マウス位置から右上に向けてメニューが出るようにする
					| TPM_RETURNCMD		//選択されるまで制御を返さない (これ無いほうがスマートかなぁ)
					| TPM_RIGHTBUTTON	//右ボタンでも選択できるようにする
					,
					pt.x,
					pt.y,
					0,
					hwnd,
					NULL
				);
				::PostMessage(g_hwndMain, WM_NULL, 0, 0); //※Q135788

				//結果
//				if(nRet==IDM_TRAY_EXIT){
//					//終了
//					this->PostMessage(WM_CLOSE,0,0);
//				}
			}
			break;
		}
		break;
	default:
		return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	return 0L;
}


int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine,int nCmdShow)
{
	c2lib::Init(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
	debugInit(DEBUG_OUT_DEBUGGER);

//	void test_dlg(HINSTANCE);
//	test_dlg(hInstance);

//	return 0;


	//ウィンドウクラス登録
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra=0;
	wndclass.cbSize=sizeof(WNDCLASSEX);
	wndclass.cbWndExtra=0;
	wndclass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wndclass.hIcon=NULL;
	wndclass.hIconSm=NULL;
	wndclass.hInstance=hInstance;
	wndclass.lpfnWndProc=WndProc;
	wndclass.lpszClassName=_T("winMain");
	wndclass.lpszMenuName=MAKEINTRESOURCE(MNU_MAIN);
	wndclass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wndclass);

	//ウィドウ作成
	g_hwndMain=CreateWindowEx(0,_T("winMain"),_T("WinSample"),WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,700,400,
		NULL,NULL,hInstance,NULL);

	// タスクトレイアイコン //
	CTrayIcon cTrayIcon(g_hwndMain, 100, WM_MYTRAY, ::LoadIcon(c2lib::GetInstance(), MAKEINTRESOURCE(ICO_MAIN)), L"WinSampleTray");

	//ウィンドウ表示
	ShowWindow(g_hwndMain,nCmdShow);
	UpdateWindow(g_hwndMain);

	//メッセージ処理
	MSG msg;
	while(GetMessage(&msg,NULL,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

