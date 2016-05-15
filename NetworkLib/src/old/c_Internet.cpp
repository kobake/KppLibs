#include "common/_required.h"
#include <windows.h>
#include <wininet.h>
#include "c_Internet.h"
#include <StringLib.h>
using namespace util;

Internet::Internet()
{
	hInternet=NULL;
	internetSetAgent(L"HogeAgent");
}

Internet::~Internet()
{
	internetDisconnect();
}

bool Internet::internetConnect()
{
	if(hInternet!=NULL){
		return true; // Ú‘±Ï‚İ
	}
	hInternet=InternetOpen(agent,INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0);
	return hInternet!=NULL;
}

bool Internet::internetDisconnect()
{
	if(hInternet!=NULL){
		if(InternetCloseHandle(hInternet)){
			hInternet=NULL;
			return true;
		}else{
			return false;
		}
	}else{
		return true; //Ø’fÏ‚İ
	}
}

bool Internet::internetIsConnect()
{
	return hInternet!=NULL;
}

void Internet::internetSetAgent(wchar *_agent)
{
	wcssafecpy(agent,_agent,countof(agent));
}
