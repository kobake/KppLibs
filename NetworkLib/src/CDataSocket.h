#pragma once

#include "FilterSocket.h"

//!CSocketに機能を追加した版。
class CDataSocket : public FilterSocket{
public:
	CDataSocket(CSocket* s) : FilterSocket(s) { }
	//インターフェース
	int sendf(const char* str,...);                   //!<文字列送信
	int recv_smart(void* buf,uint len,int flags=0);   //!<ちょっとずつ受信
	std::string recv_string();                        //!<文字列を受信できるだけ受信
};
