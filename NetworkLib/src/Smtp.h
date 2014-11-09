#pragma once

#include "CSocket.h"
#include "CDataSocket.h"
#include <string>
#include <LogLib.h>
class CIpAddress;

class Smtp{
public:
	//コンストラクタ・デストラクタ
	Smtp(bool _log_print);
	~Smtp();
	//インターフェース
	void Connect(const wchar_t* _hostname); //!< 接続
	void quit();                          //!< 切断
	void sendmail(
		const char* to,
		const char* subject,
		const char* body,
		const char* from
	);
private:
	Log log;
	std::wstring hostname;
	CSocket s2;
	CDataSocket s;
};

