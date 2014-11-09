#pragma once

class CIpAddress;
#include "CDataSocket.h"
#include <string>
#include <LogLib.h>

class Pop3{
private:
	typedef std::string mystring;
public:
	//コンストラクタ・デストラクタ
	Pop3(bool log_print); //!< log_print: ログをコンソールに出力するかどうか
	~Pop3();
	//インターフェース
	void Connect(const CIpAddress& ip);               //!< サーバに接続
	void login(const wchar_t* user,const wchar_t* pass); //!< ログイン
	void quit();                                     //!< 通信終了
	mystring getMail(int no);
private:
	Log log;
	CSocket s2;
	CDataSocket s;
	int nmessage;
};
