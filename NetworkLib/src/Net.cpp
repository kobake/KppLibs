#include "_required.h"
#include "Net.h"
#include "std/myexception.h"
#include <winsock2.h> //windows.hより前にないとダメ
#include <windows.h>
#include <DebugLib.h>

static int setuped=0;
static WSADATA wsadata;

//! DLL初期化。基本的にはc2lib (→廃止。CSocketに変更) が自動で呼び出す。
void Net::init()
{
	if(setuped)return;
	int ret=WSAStartup(MAKEWORD(1,1),&wsadata);
	if(ret!=0){
		throw myexception(L"WSAStartup failed");
	}
	setuped=1;

	//##アプリ終了時にWSACleanupが必要？
}


//! 情報をコンソールに出力する。
void Net::print_info()
{
	assert(setuped);

	wprintf(L"wVersion = %d.%d\n",LOBYTE(wsadata.wVersion),HIBYTE(wsadata.wVersion));
	wprintf(L"wHighVersion = %d.%d\n",LOBYTE(wsadata.wHighVersion),HIBYTE(wsadata.wHighVersion));
	wprintf(L"szDescription = %hs\n",wsadata.szDescription);
	wprintf(L"szSystemStatus = %hs\n",wsadata.szSystemStatus);
	wprintf(L"iMaxSockets = %d\n",wsadata.iMaxSockets);
	wprintf(L"iMaxUdpDg= %d\n",wsadata.iMaxUdpDg);
	wprintf(L"lpVenderInfo= %hs\n",wsadata.lpVendorInfo);
}

//#########WSAGetLastErrorを実装
