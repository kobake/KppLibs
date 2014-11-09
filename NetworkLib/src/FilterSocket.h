#pragma once

#include "CSocket.h"
#include "CIpAddress.h"

//! CSocketをラップするクラス
class FilterSocket{
public:
	//コンストラクタ・デストラクタ
	FilterSocket(CSocket* _s) : s(_s) { }

	//状態
	bool valid() const{ return s->valid(); }

	//クライアントインターフェース
	void Connect(const CSocketAddress& addr){ return s->Connect(addr); }

	//サーバインターフェース
	void bind(ushort port){ return s->bind(port); }
	void listen(){ return s->listen(); }
	CSocket* accept(){ return s->accept(); }

	//共通インターフェース
	int send(const void* data,uint len,int flags=0){ return s->send(data,len,flags); }
	int recv(void* buf,uint len,int flags=0){ return s->recv(buf,len,flags); }
	void Close(){ return s->Close(); }
	const CSocketAddress& getAddress() const{ return s->getAddress(); }

	//その他
	void shutdown(){ return s->shutdown(); }

	//機能
	CIpAddress getLocalAddress() const{ return s->getLocalAddress(); }
	CIpAddress getRemoteAddress() const{ return s->getRemoteAddress(); }

private:
	CSocket* s; //!<ラップするソケットへの参照
};
