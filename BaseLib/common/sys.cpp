#include "_required.h"
#include "sys.h"
#include <DebugLib.h>
#include <locale.h>
#include <objbase.h> //CoInitialize
#include <commctrl.h> //InitCommonControls
#include "common/CAppInfo.h"
#include <LogLib.h>
#include <StringLib.h>
#include <Windows.h>
#include <mmsystem.h>
#include <time.h>

static HINSTANCE g_hInst;
static bool g_bInit;

//コンソール、ウィンドウズベース、共通の初期化処理
static void _init()
{
	//ロケール初期化
	//setlocale(LC_ALL,"Japanese");
	setlocale(LC_ALL,"");

	// timeGetTimeの分解能 //
	timeBeginPeriod(1);

#ifdef _DEBUG
	//メモリリーク検出システム
//	DebugSystem::memoryCheckBegin();
#endif

	//コモンコントロール初期化 ※comctl32.libが必要
	InitCommonControls();
	INITCOMMONCONTROLSEX ic;
	ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ic.dwICC = ICC_COOL_CLASSES; //レバー
	InitCommonControlsEx(&ic); 

	// 乱数初期化
	srand(timeGetTime());

	// ログ初期化
//	CLogManager::Instance()->Init(*CAppInfo::Instance());
}

//main用
void BaseLib::Init(int argc,wchar_t* argv[],ASSERT_HANDLE pfAssertHandle)
{
	if(g_bInit)return;
	g_bInit = true;

	_init();
	debugInit(DEBUG_OUT_STDOUT, pfAssertHandle);
}

//WinMain用
void BaseLib::Init(HINSTANCE hInst,HINSTANCE hPrevInst,LPWSTR lpCmdLine,int nCmdShow,ASSERT_HANDLE pfAssertHandle)
{
	if(g_bInit)return;
	g_bInit = true;

	_init();
	g_hInst=hInst;
	debugInit(DEBUG_OUT_DEBUGGER, pfAssertHandle);
}

//DllMain用
void BaseLib::Init(HINSTANCE hInst, DWORD fdwReason, PVOID pvReserved,ASSERT_HANDLE pfAssertHandle)
{
	if(g_bInit)return;
	g_bInit = true;

	_init();
	g_hInst=hInst;
	debugInit(DEBUG_OUT_MSGBOX, pfAssertHandle);
}

void BaseLib::InitCom()
{
	//COM初期化
	HRESULT ret=CoInitialize(NULL);
	assert(ret==S_OK);
}


HINSTANCE BaseLib::GetInstance()
{
	return g_hInst;
}


