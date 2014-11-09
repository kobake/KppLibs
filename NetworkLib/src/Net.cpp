#include "_required.h"
#include "Net.h"
#include "std/myexception.h"
#include <winsock2.h> //windows.h���O�ɂȂ��ƃ_��
#include <windows.h>
#include <DebugLib.h>

static int setuped=0;
static WSADATA wsadata;

//! DLL�������B��{�I�ɂ�c2lib (���p�~�BCSocket�ɕύX) �������ŌĂяo���B
void Net::init()
{
	if(setuped)return;
	int ret=WSAStartup(MAKEWORD(1,1),&wsadata);
	if(ret!=0){
		throw myexception(L"WSAStartup failed");
	}
	setuped=1;

	//##�A�v���I������WSACleanup���K�v�H
}


//! �����R���\�[���ɏo�͂���B
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

//#########WSAGetLastError������
