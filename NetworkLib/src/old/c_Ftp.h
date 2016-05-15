#pragma once

#include "c_Internet.h"

class Ftp : public Internet{
protected:
	HINTERNET hHost;
public:
	Ftp();
	virtual ~Ftp();
	bool ftpConnect(wchar *host,wchar *user,wchar *pass);
	bool ftpIsConnect();
	bool ftpDownload(wchar *downpath,wchar *savepath);
	bool ftpDisconnect();
	bool ftpSetDir(wchar *dir);
};


