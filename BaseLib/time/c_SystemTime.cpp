#include <windows.h>
#include <cstdio>
#include "common/_required.h"
#include "c_SystemTime.h"
#include <StringLib.h>

void ZeroSYSTEMTIME(SYSTEMTIME *time)
{
	memset(time,0,sizeof(SYSTEMTIME));
}

SystemTime::SystemTime()
{
	ZeroSYSTEMTIME(this);
}

SystemTime::SystemTime(FILETIME *filetime)
{
	setFileTime(filetime);
}

void SystemTime::setFileTime(FILETIME *filetime)
{
	FileTimeToSystemTime(filetime,this);
}


wchar *SystemTime::toString()
{
	static wchar tmp[64];
	swprintf(tmp,L"%04d/%02d/%02d %02d:%02d",wYear,wMonth,wDay,wHour,wMinute);
	return tmp;
}

int SystemTime::compare(SystemTime *time)
{
	int ret;
	ret=(wYear)-(time->wYear); if(ret!=0)return ret;
	ret=(wMonth*40+wDay)-(time->wMonth*40+time->wDay); if(ret!=0)return ret;
	ret=(wHour*60+wMinute)-(time->wHour*60+time->wMinute); if(ret!=0)return ret;
	ret=(wSecond*1000+wMilliseconds)-(time->wSecond*1000+time->wMilliseconds); if(ret!=0)return ret;
	return ret;
}

