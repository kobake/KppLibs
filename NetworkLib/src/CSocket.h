#pragma once

class CSocketAddress;
class CIpAddress;

#include "_head.h"
#include "std/myexception.h"
#include "CSocketAddress.h"

class CSocket{
public:
	GEN_MYEXCEPTION(CSocketException);
	GEN_ARGSEXCEPTION(init_failed,			CSocketException);
	GEN_ARGSEXCEPTION(connect_failed,		CSocketException);
	GEN_ARGSEXCEPTION(send_failed,			CSocketException);
	GEN_ARGSEXCEPTION(recv_failed,			CSocketException);
	GEN_ARGSEXCEPTION(close_failed,			CSocketException);
	GEN_ARGSEXCEPTION(getsockname_failed,	CSocketException);
	GEN_ARGSEXCEPTION(getpeername_failed,	CSocketException);
	GEN_ARGSEXCEPTION(shutdown_failed,		CSocketException);
public:
	static void s_setup();
private:
	//代入禁止
	CSocket(const CSocket&);
	CSocket& operator = (const CSocket&);
public:
	//コンストラクタ・デストラクタ
	CSocket();
	explicit CSocket(SOCKET _s,const sockaddr_in& _saddr); //!< SOCKETをラップする版。デストラクタでSOCKETはクローズされる。
	~CSocket();

	//状態
	bool valid() const;       //!< SOCKETハンドルが有効ならtrue
	bool isConnected() const; //!< 接続しているホストがあればtrue

	//クライアントインターフェース
	void Connect(const CSocketAddress& addr);          //!<サーバに接続

	//サーバインターフェース
	void bind(ushort port);                     //!< ポートにバインド
	void listen();                              //!< リスンモードへ
	CSocket* accept();                           //!< クライアント受付。新しいCSocketインスタンスを返すので、使い終わったらdeleteする必要がある。

	//共通インターフェース
	int send(const void* data,uint len,int flags=0);  //!<送信
	int recv(void* buf,uint len,int flags=0);         //!<受信
	void Close();                                     //!<接続終了
	const CSocketAddress& getAddress() const{ return current_address; } //!< アドレスを取得

	//その他
	void shutdown();

	//機能
	CIpAddress getLocalAddress() const;  //!<自分のIPアドレスを取得。接続が確立していないとエラー。
	CIpAddress getRemoteAddress() const; //!<相手のIPアドレスを取得。接続が確立していないとエラー。

	//生値
	SOCKET _GetHandle() const{ return m_s; }
private:
	SOCKET m_s;
	CSocketAddress current_address;
};

class CSocketSender{
public:
	CSocketSender(CSocket& cSocket) : m_pcSocketRef(&cSocket) { }
	void SendTextW(const wchar_t* pText, int nLen);
private:
	CSocket* m_pcSocketRef;
};

#include "_lib_wsock32.h"
