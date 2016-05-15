#include "common/_required.h"
#include <windows.h>
#include <wininet.h>
#include "c_Ftp.h"

Ftp::Ftp()
{
	hHost=NULL;
}

Ftp::~Ftp()
{
	ftpDisconnect();
}

bool Ftp::ftpConnect(wchar *host,wchar *user,wchar *pass)
{
	internetConnect();
	if(internetIsConnect()){
		hHost=InternetConnect(hInternet,host,INTERNET_INVALID_PORT_NUMBER,user,pass,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0);
		return hHost!=NULL;
	}else{
		return false;
	}
}

bool Ftp::ftpIsConnect()
{
	return hHost!=NULL;
}

bool Ftp::ftpSetDir(wchar *dir)
{
	if(hHost==NULL)return false;
	return FtpSetCurrentDirectory(hHost,dir)!=0;
}

bool Ftp::ftpDownload(wchar *downpath,wchar *savepath)
{
	if(hHost==NULL)return false;
	return FtpGetFile(hHost,downpath,savepath,FALSE,FILE_ATTRIBUTE_NORMAL,FTP_TRANSFER_TYPE_BINARY,0)!=0;
}

bool Ftp::ftpDisconnect()
{
	if(hHost!=NULL){
		if(InternetCloseHandle(hHost)){
			hHost=NULL;
			internetDisconnect();
			return true;
		}else{
			return false;
		}
	}else{
		return true; //êÿífçœÇ›
	}
}
