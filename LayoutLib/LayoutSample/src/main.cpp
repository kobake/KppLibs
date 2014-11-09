/*
	遅延コピーのテスト
*/
#include "common/_required.h"
#include "common/sys.h"
#include <windows.h>
HWND hwndMain;
#include <vector>
#include <string>
#include <shlobj.h>
using namespace std;
#pragma comment(lib, "comctl32.lib")
#include "../../src/LayoutLib.h"
#include "../resource.h"

CHorzLayout cHorzLayout;

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static HWND hwndButton;

	switch(msg){
	case WM_CREATE:
		{
			//レイアウトベースの作成
			std::vector<LayoutPartInfo> vInfos;
			vInfos.push_back(LayoutPartInfo(100,200,300));
			vInfos.push_back(LayoutPartInfo(100,200,-1));
			vInfos[0].strCaption = L"検索";
			vInfos[0].bCloseButton = true;
			cHorzLayout.Create(hwnd, vInfos);
			//ボタンの作成
			hwndButton = ::CreateWindowEx(
				0,
				L"BUTTON",
				L"てすと",
				WS_VISIBLE | WS_CHILD,
				0, 0, 100, 100,
				hwnd,
				NULL,
				c2lib::GetInstance(),
				NULL
			);
			//アタッチ
			cHorzLayout.AttachToCol(1, hwndButton);

		}
		break;
	case WM_SIZE:
		{
			cHorzLayout.Move(CRect(0, 0, LOWORD(lParam), HIWORD(lParam)));
		}
		break;
	case WM_COMMAND:
		{
			WORD wId = LOWORD(wParam);
			switch(wId){
			case IDM_FILE_EXIT:
				::PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;
			case IDM_COL_0:
				cHorzLayout.ShowCol(0, !cHorzLayout.IsColVisible(0));
				break;
			case IDM_COL_1:
				cHorzLayout.ShowCol(1, !cHorzLayout.IsColVisible(1));
				break;
			}
		}
		break;
	case WM_INITMENUPOPUP:
		{
			HMENU hMenu = (HMENU)wParam;
			UINT uId = LOWORD(lParam);
			BOOL fSysMenu = HIWORD(lParam);
			if(fSysMenu)return 0L;
			if(uId!=1)return 0L;
			::CheckMenuItem(hMenu, IDM_COL_0, MF_BYCOMMAND | cHorzLayout.IsColVisible(0)?MF_CHECKED:MF_UNCHECKED);
			::CheckMenuItem(hMenu, IDM_COL_1, MF_BYCOMMAND | cHorzLayout.IsColVisible(1)?MF_CHECKED:MF_UNCHECKED);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
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
	hwndMain=CreateWindowEx(0,_T("winMain"),_T("LayoutSample"),WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,700,400,
		NULL,NULL,hInstance,NULL);

	//ウィンドウ表示
	ShowWindow(hwndMain,nCmdShow);
	UpdateWindow(hwndMain);

	//メッセージ処理
	MSG msg;
	while(GetMessage(&msg,NULL,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

