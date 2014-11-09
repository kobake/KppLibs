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
#include "../resource.h"

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg){
	case WM_PAINT:
		{
			
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
	hwndMain=CreateWindowEx(0,_T("winMain"),_T("GraphicsSample"),WS_OVERLAPPEDWINDOW,
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

